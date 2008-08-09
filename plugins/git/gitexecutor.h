/***************************************************************************
 *   This file was partly taken from KDevelop's cvs plugin                 *
 *   Copyright 2007 Robert Gruber <rgruber@users.sourceforge.net>          *
 *                                                                         *
 *   Adapted for Git                                                       *
 *   Copyright 2008 Evgeniy Ivanov <powerfox@kde.ru>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef GIT_EXECUTOR_H
#define GIT_EXECUTOR_H

#include <KUrl>
#include <KJob>
#include <QStringList>

#include <vcs/dvcs/idvcsexecutor.h>
#include <vcs/vcsrevision.h>

class DVCSjob;

namespace KDevelop
{
    class IPlugin;
    class VcsStatusInfo;
}

/**
 * This proxy acts as a single point of entry for most of the common git commands.
 * It is very easy to use, as the caller does not have to deal which the DVCSjob class directly.
 * All the command line generation and job handling is done internally. The caller gets a DVCSjob
 * object returned from the proxy and can then call it's start() method.
 *
 * Here is and example of how to user the proxy:
 * @code
 * DVCSjob* job = proxy->editors( repo, urls );
 * if ( job ) {
 *     connect(job, SIGNAL( result(KJob*) ),
 *             this, SIGNAL( jobFinished(KJob*) ));
 *     job->start();
 * }
 * @endcode
 *
 * @note All actions that take a KUrl::List also need an url to the repository which
 *       must be a common base directory to all files from the KUrl::List.
 *       Actions that just take a single KUrl don't need a repository, the git command will be
 *       called directly in the directory of the given file
 *
 * @author Robert Gruber <rgruber@users.sourceforge.net>
 * @author Evgeniy Ivanov <powerfox@kde.ru>
 */
class GitExecutor : public QObject, public KDevelop::IDVCSexecutor
{
    Q_OBJECT
public:
    explicit GitExecutor(KDevelop::IPlugin* parent = 0);
    ~GitExecutor();

    bool isValidDirectory(const KUrl &dirPath);
    QString name() const;

    DVCSjob* init(const KUrl & directory);
    DVCSjob* clone(const KUrl &directory, const KUrl repository);
    DVCSjob* add(const QString& repository, const KUrl::List &files);
    DVCSjob* commit(const QString& repository,
                    const QString& message = "KDevelop did not provide any message, it may be a bug",
                    const KUrl::List& args = QStringList());
    DVCSjob* remove(const QString& repository, const KUrl::List& files);
    DVCSjob* status(const QString & repo, const KUrl::List & files,
                    bool recursive=false, bool taginfo=false);
    DVCSjob* log(const KUrl& url);
    DVCSjob* var(const QString &directory);
    DVCSjob* empty_cmd() const;

    DVCSjob* checkout(const QString &repository, const QString &branch);
    DVCSjob* branch(const QString &repository, const QString &basebranch = QString(), const QString &branch = QString(),
                    const QStringList &args = QStringList());
    DVCSjob* reset(const QString &repository, const QStringList &args, const QStringList files);
private:
    //it can be public, but it isn't required now
    DVCSjob* lsFiles(const QString &repository, const QStringList &args);
    DVCSjob* gitRevParse(const QString &repository, const QStringList &args);

public:
    //parsers for branch:
    QString curBranch(const QString &repository);
    QStringList branches(const QString &repository);

    //commit dialog helpers, send to main helper the arg for git-ls-files:
    QList<KDevelop::VcsStatusInfo> getModifiedFiles(const QString &directory);
    QList<KDevelop::VcsStatusInfo> getCachedFiles(const QString &directory);
    QStringList getOtherFiles(const QString &directory);

    void parseOutput(const QString& jobOutput, QList<DVCScommit>& commits) const;

private:
    //commit dialog "main" helper
    QStringList getLsFiles(const QString &directory, const QStringList &args);
    KDevelop::VcsStatusInfo::State charToState(const char ch);

    KDevelop::IPlugin* vcsplugin;

};

#endif
