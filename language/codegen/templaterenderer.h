/*
 * This file is part of KDevelop
 * Copyright 2012 Miha Čančula <miha@noughmad.eu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KDEVELOP_TEMPLATERENDERER_H
#define KDEVELOP_TEMPLATERENDERER_H

#include <QVariantHash>
#include <QStringList>

#include "../languageexport.h"

class KUrl;
class KArchiveDirectory;

namespace KDevelop
{

class SourceFileTemplate;

class DocumentChangeSet;

/**
 * @brief Convenience class for rendering multiple templates with the same context
 *
 * The TemplateRenderer provides easy access to common template operations.
 * Internally, it encapsulates a Grantlee::Engine and a Grantlee::Context.
 *
 * It is used by adding a set of variables, and then renderering a template string
 * @code
 * TemplateRenderer renderer;
 * renderer.addVariable("greeting", "Hello");
 * renderer.addVariable("target", "World");
 * QString text = renderer.render("{{ greeting }}, {{ target }}!");
 * // text == "Hello, World!"
 * @endcode
 *
 * If you wish to include other templates using the Grantlee {% include %} tag,
 * make sure TemplateRenderer can find those template by using
 * addTemplateDirectories() and addArchive(). This adds everything in the specified
 * directories or archive files to the list of files search for inclusion.
 *
 * Directories named "kdevcodegen/templates" in the "data" resource type are always included in the search path,
 * there is no need to add them explicitely. Additionally, TemplateRenderer adds the "lib" resource directories
 * to the Grantlee plugin search path, so plugins installed there will be available to templates.
 *
 **/
class KDEVPLATFORMLANGUAGE_EXPORT TemplateRenderer
{
public:
    /**
     * Policy for working with empty lines
     **/
    enum EmptyLinesPolicy
    {
        /**
         * Keep empty lines as they are in the rendered output.
         * The output from the template is returned unmodified.
         */
        KeepEmptyLines,
        /**
         * If the template output has more than one line, the renderer
         * performs a smart trim on the rendered output.
         * @li single empty lines are removed
         * @li two or more consecutive empty lines are compressed into a single empty line
         * @li a single empty line is kept at the end
         */
        TrimEmptyLines,
        /**
         * Removes all empty lines from the template output, and appends a newline at the end if needed.
         */
        RemoveEmptyLines
    };

    TemplateRenderer();
    virtual ~TemplateRenderer();

    /**
     * Adds @p directories to the list of directories searched for templates
     *
     **/
    void addTemplateDirectories(const QStringList& directories);
    /**
     * Adds the archive @p directory to the list of places searched for templates
     *
     **/
    void addArchive(const KArchiveDirectory* directory);

    /**
     * Adds @p variables to the Grantlee::Context passed to each template.
     *
     * If the context already contains a variables with the same name as a key in @p variables,
     * it is overwritten.
     *
     **/
    void addVariables(const QVariantHash& variables);

    /**
     * Adds variable with name @p name and value @p value to the Grantlee::Context passed to each template.
     *
     * If the context already contains a variables with the same @p name, it is overwritten.
     *
     **/
    void addVariable(const QString& name, const QVariant& value);

    /**
     * Returns the current variables defined for this renderer
     *
     * @sa addVariable(), addVariables()
     */
    QVariantHash variables() const;

    /**
     * @brief Renders a single template
     *
     * @param content the template content
     * @param name (optional) the name of this template
     * @return the rendered template
     **/
    QString render(const QString& content, const QString& name = QString()) const;

    /**
     * @brief Renders a single template from a file
     *
     * @param url the URL of the file from which to load the template
     * @param name (optional) the name of this template
     * @return the rendered template
     **/
    QString renderFile(const KUrl& url, const QString& name = QString()) const;

    /**
     * @brief Renders a list of templates
     *
     * This is a convenience method, suitable if you have to render a large number of templates
     * with the same context.
     *
     * @param content the template contents
     * @return the rendered templates
     **/
    QStringList render(const QStringList& contents) const;

    /**
     * @brief Sets the policy for empty lines in the rendered output
     *
     * The default is KeepEmptyLines, where the template output is return unmodified.
     *
     * @param policy policy for empty lines in the rendered output
     * @sa EmptyLinesPolicy
     */
    void setEmptyLinesPolicy(EmptyLinesPolicy policy);

    /**
     * Returns the currently set policy for empty lines in the rendered output
     * @sa EmptyLinesPolicy, setEmptyLinesPolicy()
     */
    EmptyLinesPolicy emptyLinesPolicy() const;

    /**
     * @brief Renders all templates in the archive represented by @p fileTemplate
     *
     * Output files are saved to corresponding URLs in @p fileUrls
     *
     * For each output file, TemplateRenderer add two variables named @c output_file_x
     * and @c output_file_x_absolute, where @c x is replaced
     * with the file name specified in the template description file.
     * The variable name is entirely lowercase and cleaned by replacing
     * all non-alphanumerical characters with underscores.
     * For example, if the file is named "Public Header" in
     * the description file, the variable will be @c output_file_public_heder.
     *
     * As their name suggests, @c output_file_x contains the relative path from baseUrl() to the URL of the
     * x's output location, while @c output_file_x_absolute contains x's absolute output URL.
     * Both are avaliable to templates as strings.
     *
     * @param fileTemplate the source file template to render
     * @param baseUrl the base URL used for calculating relative output file URLs
     * @param fileUrls destination URLs of the output files
     * @return KDevelop::DocumentChangeSet
     */
    DocumentChangeSet renderFileTemplate(const KDevelop::SourceFileTemplate& fileTemplate,
                                         const KUrl& baseUrl, const QHash<QString, KUrl>& fileUrls);

private:
    class TemplateRendererPrivate* const d;
};

}

#endif // KDEVELOP_TEMPLATERENDERER_H