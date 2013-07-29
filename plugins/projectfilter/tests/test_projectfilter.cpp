/*
 * This file is part of KDevelop
 * Copyright 2013 Milian Wolff <mail@milianw.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test_projectfilter.h"

#include <qtest_kde.h>

#include <QDebug>
#include <KConfigGroup>

#include <tests/testcore.h>
#include <tests/autotestshell.h>
#include <tests/testproject.h>

#include "../projectfilter.h"

QTEST_KDEMAIN(TestProjectFilter, NoGUI);

using namespace KDevelop;

typedef QSharedPointer<ProjectFilter> Filter;

Q_DECLARE_METATYPE(Filter)

namespace {

const bool Invalid = false;
const bool Valid = true;
const bool Folder = true;
const bool File = false;

struct MatchTest
{
    QString path;
    bool isFolder;
    bool shouldMatch;
};

class FilterTestProject : public TestProject
{
public:
    FilterTestProject(const QStringList& includes, const QStringList& excludes)
    {
        KConfigGroup filters = projectConfiguration()->group("Filters");
        filters.writeEntry("Includes", includes);
        filters.writeEntry("Excludes", excludes);
    }
    virtual ~FilterTestProject()
    {
        projectConfiguration()->deleteGroup("Filters");
    }
};

void addTests(const QString& tag, const FilterTestProject& project, const Filter& filter, MatchTest* tests, uint numTests)
{
    for (uint i = 0; i < numTests; ++i) {
        const MatchTest& test = tests[i];
        QTest::newRow(qstrdup(qPrintable(tag + ':' + test.path)))
            << filter
            << KUrl(project.folder(), test.path)
            << test.isFolder
            << test.shouldMatch;

        if (test.isFolder) {
            // also test folder with trailing slash - should not make a difference
            QTest::newRow(qstrdup(qPrintable(tag + ':' + test.path + '/')))
                << filter
                << KUrl(project.folder(), test.path + '/')
                << test.isFolder
                << test.shouldMatch;
        }
    }
}

///FIXME: remove once we can use c++11
#define ADD_TESTS(tag, project, filter, tests) addTests(tag, project, filter, tests, sizeof(tests) / sizeof(tests[0]))

}

void TestProjectFilter::initTestCase()
{
    AutoTestShell::init();
    TestCore::initialize(Core::NoUi);
    qRegisterMetaType<Filter>();
}

void TestProjectFilter::cleanupTestCase()
{
    TestCore::shutdown();
}

void TestProjectFilter::match()
{
    QFETCH(Filter, filter);
    QFETCH(KUrl, path);
    QFETCH(bool, isFolder);
    QFETCH(bool, expectedIsValid);

    QCOMPARE(filter->isValid(path, isFolder), expectedIsValid);
}

void TestProjectFilter::match_data()
{
    QTest::addColumn<Filter>("filter");
    QTest::addColumn<KUrl>("path");
    QTest::addColumn<bool>("isFolder");
    QTest::addColumn<bool>("expectedIsValid");

    {
        // test default filters
        FilterTestProject project((QStringList()), QStringList());
        // delete group to get explicit defaults
        project.projectConfiguration()->deleteGroup("Filters");
        Filter filter(new ProjectFilter(&project));

        QTest::newRow("projectRoot") << filter << project.folder() << Folder << Valid;
        QTest::newRow("project.kdev4") << filter << project.projectFileUrl() << File << Invalid;

        MatchTest tests[] = {
            //{path, isFolder, isValid}
            {".kdev4", Folder, Invalid},

            {"folder", Folder, Valid},
            {"folder/folder", Folder, Valid},
            {"file", File, Valid},
            {"folder/file", File, Valid},
            {".file", File, Invalid},
            {".folder", Folder, Invalid},
            {"folder/.folder", Folder, Invalid},
            {"folder/.file", File, Invalid}
        };
        ADD_TESTS("default", project, filter, tests);
    }
    {
        // test includes
        FilterTestProject project(QStringList() << "*.cpp", QStringList());
        Filter filter(new ProjectFilter(&project));

        QTest::newRow("projectRoot") << filter << project.folder() << Folder << Valid;
        QTest::newRow("project.kdev4") << filter << project.projectFileUrl() << File << Invalid;

        MatchTest tests[] = {
            //{path, isFolder, isValid}
            {".kdev4", Folder, Invalid},

            {"folder", Folder, Valid},
            {"file", File, Invalid},
            {"file.cpp", File, Valid},
            {".file.cpp", File, Valid},
            {"folder/file.cpp", File, Valid},
            {"folder/.file.cpp", File, Valid}
        };
        ADD_TESTS("include*.cpp", project, filter, tests);
        project.projectConfiguration();
    }
    {
        // test excludes
        FilterTestProject project(QStringList(), QStringList() << "*.cpp");
        Filter filter(new ProjectFilter(&project));

        QTest::newRow("projectRoot") << filter << project.folder() << Folder << Valid;
        QTest::newRow("project.kdev4") << filter << project.projectFileUrl() << File << Invalid;

        MatchTest tests[] = {
            //{path, isFolder, isValid}
            {".kdev4", Folder, Invalid},

            {"folder", Folder, Valid},
            {"file", File, Valid},
            {"file.cpp", File, Invalid},
            {"folder.cpp", Folder, Valid},
            {"folder/file.cpp", File, Invalid},
            {"folder/folder.cpp", Folder, Valid}
        };
        ADD_TESTS("exclude*.cpp", project, filter, tests);
    }
}

#include "test_projectfilter.moc"