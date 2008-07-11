/***************************************************************************
 *   This file was partly taken from KDevelop's cvs plugin                 *
 *   Copyright 2007 Robert Gruber <rgruber@users.sourceforge.net>          *
 *                                                                         *
 *   Adapted for DVCS (added templates)                                    *
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

#ifndef DVCS_PLUGIN_CC
#define DVCS_PLUGIN_CC

#include "dvcsplugin.h"

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QFileInfo>
#include <QString>

#include <KParts/PartManager>
#include <KParts/Part>

#include <KActionCollection>
#include <KMessageBox>
#include <KAction>

#include <interfaces/iproject.h>
#include <projectmodel.h>
#include <iuicontroller.h>
#include <icore.h>
#include <context.h>
#include <vcsmapping.h>

#include<interfaces/contextmenuextension.h>

#include "idvcsexecutor.h"
#include "vcsjob.h"
#include "dvcsjob.h"
#include "dvcsmainview.h"
#include "commitdialog.h"
#include "importdialog.h"
#include "importmetadatawidget.h"

using KDevelop::DistributedVersionControlPlugin;


DistributedVersionControlPlugin::DistributedVersionControlPlugin(QObject *parent, KComponentData compData)
    :IPlugin(compData, parent)
{
    d = new DVCSpluginPrivate();
    d->m_factory = new KDevDVCSViewFactory(this);
}

// Begin:  KDevelop::IBasicVersionControl

// QString DistributedVersionControlPlugin::name() const
// {
//     return d->m_exec->name();
// }

bool DistributedVersionControlPlugin::isVersionControlled(const KUrl& localLocation)
{
    //TODO: some files from repository location can be not version controlled
    return d->m_exec->isValidDirectory(localLocation);
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::repositoryLocation(const KUrl & localLocation)
{
    return NULL;
}

//Note: recursion is not used

KDevelop::VcsJob*
        DistributedVersionControlPlugin::add(const KUrl::List & localLocations,
                                             IBasicVersionControl::RecursionMode recursion)
{
    return d->m_exec->add(localLocations[0].path(), localLocations);
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::remove(const KUrl::List & localLocations)
{
    QFileInfo info(localLocations[0].toLocalFile() );

    return d->m_exec->remove(info.absolutePath(), localLocations);
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::status(const KUrl::List & localLocations,
                                                IBasicVersionControl::RecursionMode recursion)
{
    QFileInfo info( localLocations[0].toLocalFile() );

    return d->m_exec->status(info.absolutePath(),
                              localLocations,
                              (recursion == IBasicVersionControl::Recursive) ? true:false);
}

///Not used in DVCS;
KDevelop::VcsJob*
        DistributedVersionControlPlugin::copy(const KUrl& localLocationSrc,
                                              const KUrl& localLocationDstn) 
{
    return d->m_exec->empty_cmd();
}

///Not used in DVCS;
KDevelop::VcsJob*
        DistributedVersionControlPlugin::move(const KUrl& localLocationSrc,
                                              const KUrl& localLocationDst ) 
{
    return d->m_exec->empty_cmd();
}


KDevelop::VcsJob*
        DistributedVersionControlPlugin::revert(const KUrl::List & localLocations, 
                                                IBasicVersionControl::RecursionMode recursion)
{
    return d->m_exec->empty_cmd();
}


KDevelop::VcsJob*
        DistributedVersionControlPlugin::update(const KUrl::List & localLocations, const VcsRevision & rev,
                                                IBasicVersionControl::RecursionMode recursion)
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::commit(const QString & message, const KUrl::List & localLocations,
                                                IBasicVersionControl::RecursionMode recursion)
{
    QString msg = message;
    if( msg.isEmpty() )
    {
        CommitDialog dlg;
        if( dlg.exec() == QDialog::Accepted )
        {
            msg = dlg.message();
        }
    }
    QFileInfo info(localLocations[0].toLocalFile() );

    return d->m_exec->commit(info.absolutePath(), msg, localLocations);
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::diff(const VcsLocation & localOrRepoLocationSrc,
                                              const VcsLocation & localOrRepoLocationDst,
                                              const VcsRevision & srcRevision,
                                              const VcsRevision & dstRevision,
                                              VcsDiff::Type, IBasicVersionControl::RecursionMode)
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::log(const KUrl& localLocation,
                                             const VcsRevision& rev,
                                             unsigned long limit )
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::log(const KUrl& localLocation,
                                             const VcsRevision& rev,
                                             const VcsRevision& limit )
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::annotate(const KUrl& localLocation,
                                                  const VcsRevision& rev )
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::merge(const VcsLocation& localOrRepoLocationSrc,
                                               const VcsLocation& localOrRepoLocationDst,
                                               const VcsRevision& srcRevision,
                                               const VcsRevision& dstRevision,
                                               const KUrl& localLocation )
{
    return d->m_exec->empty_cmd();
}


KDevelop::VcsJob*
        DistributedVersionControlPlugin::resolve(const KUrl::List& localLocations,
                                                 IBasicVersionControl::RecursionMode recursion )
{
    return d->m_exec->empty_cmd();
}

// End:  KDevelop::IBasicVersionControl


// Begin:  KDevelop::IDistributedVersionControl

KDevelop::VcsJob*
        DistributedVersionControlPlugin::init(const KUrl& localRepositoryRoot)
{
    return d->m_exec->init(localRepositoryRoot);
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::clone(const QString& repositoryLocationSrc,
                                               const KUrl& localRepositoryRoot)
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::pull(const QString& repositoryLocation,
                                              const KUrl& localRepositoryLocation)
{
    return d->m_exec->empty_cmd();
}

KDevelop::VcsJob*
        DistributedVersionControlPlugin::push(const KUrl& localRepositoryLocation,
                                              const QString& repositoryLocation)
{
    return d->m_exec->empty_cmd();
}

// End:  KDevelop::IDistributedVersionControl


KDevelop::IDVCSexecutor* DistributedVersionControlPlugin::proxy()
{
    return d->m_exec;
}


KDevelop::VcsImportMetadataWidget* 
        DistributedVersionControlPlugin::createImportMetadataWidget(QWidget* parent)
{
    return new ImportMetadataWidget(parent);
}

const KUrl DistributedVersionControlPlugin::urlFocusedDocument() const
{
    KParts::ReadOnlyPart *plugin =
            dynamic_cast<KParts::ReadOnlyPart*>( core()->partManager()->activePart() );
    if ( plugin ) {
        if (plugin->url().isLocalFile() ) {
            return plugin->url();
        }
    }
    return KUrl();
}

KDevelop::ContextMenuExtension
        DistributedVersionControlPlugin::contextMenuExtension(Context* context)
{
    KUrl::List ctxUrlList;
    if( context->type() == Context::ProjectItemContext )
    {
        ProjectItemContext *itemCtx = dynamic_cast<ProjectItemContext*>(context);
        if( itemCtx )
        {
            QList<ProjectBaseItem *> baseItemList = itemCtx->items();

            // now general case
            foreach( ProjectBaseItem* _item, baseItemList )
            {
                if( _item->folder() ){
                    ProjectFolderItem *folderItem = dynamic_cast<ProjectFolderItem*>(_item);
                    ctxUrlList << folderItem->url();
                }
                else if( _item->file() ){
                    ProjectFileItem *fileItem = dynamic_cast<ProjectFileItem*>(_item);
                    ctxUrlList << fileItem->url();
                }
            }
        }
    }
    else if( context->type() == Context::EditorContext )
    {
        EditorContext *itemCtx = dynamic_cast<EditorContext*>(context);
        ctxUrlList << itemCtx->url();
    }
    else if( context->type() == Context::FileContext )
    {
        FileContext *itemCtx = dynamic_cast<FileContext*>(context);
        ctxUrlList += itemCtx->urls();
    }

    ContextMenuExtension menuExt;

    bool hasVersionControlledEntries = false;
    foreach(KUrl url, ctxUrlList)
    {
        if(isVersionControlled( url ) )
        {
            hasVersionControlledEntries = true;
            break;
        }
    }
    if(ctxUrlList.isEmpty() )
        return IPlugin::contextMenuExtension(context);

    DistributedVersionControlPlugin::d->m_ctxUrlList = ctxUrlList;
    QList<QAction*> actions;
    KAction *action;
    kDebug() << "version controlled?" << hasVersionControlledEntries;
    if(hasVersionControlledEntries)
    {
        action = new KAction(i18n("Commit..."), this);
        connect( action, SIGNAL(triggered()), this, SLOT(ctxCommit()) );
        menuExt.addAction( ContextMenuExtension::VcsGroup, action );

        action = new KAction(i18n("Add"), this);
        connect( action, SIGNAL(triggered()), this, SLOT(ctxAdd()) );
        menuExt.addAction( ContextMenuExtension::VcsGroup, action );

        action = new KAction(i18n("Remove"), this);
        connect( action, SIGNAL(triggered()), this, SLOT(ctxRemove()) );
        menuExt.addAction( ContextMenuExtension::VcsGroup, action );
    }
    else
    {
        QMenu* menu = new QMenu("Git");
        action = new KAction(i18n("Init..."), this);
        connect( action, SIGNAL(triggered()), this, SLOT(slotInit()) );
        menu->addAction(action);
        menuExt.addAction(ContextMenuExtension::ExtensionGroup, menu->menuAction() );
    }

    return menuExt;

}

void DistributedVersionControlPlugin::setupActions()
{
    KAction *action;

    action = actionCollection()->addAction("git_init");
    action->setText(i18n("Init Directory..."));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slotInit()));

//     action = actionCollection()->addAction("git_clone");
//     action->setText(i18n("Clone..."));
//     connect(action, SIGNAL(triggered(bool)), this, SLOT(slotCheckout()));

//     action = actionCollection()->addAction("git_status");
//     action->setText(i18n("Status..."));
//     connect(action, SIGNAL(triggered(bool)), this, SLOT(slotStatus()));
}

void DistributedVersionControlPlugin::slotInit()
{
    KUrl url = urlFocusedDocument();

    ImportDialog dlg(this, url);
    dlg.exec();
}

//     void DistributedVersionControlPlugin::slotStatus()
//     {
//         KUrl url = urlFocusedDocument();
//         KUrl::List urls;
//         urls << url;
// 
//         KDevelop::VcsJob* j = status(url, KDevelop::IBasicVersionControl::Recursive);
//         DVCSjob* job = dynamic_cast<DVCSjob*>(j);
//         if (job) {
//             GitGenericOutputView* view = new GitGenericOutputView(this, job);
//             emit addNewTabToMainView( view, i18n("Status") );
//             job->start();
//         }
//     }

void DistributedVersionControlPlugin::ctxCommit()
{
    VcsJob* j = commit("", DistributedVersionControlPlugin::d->m_ctxUrlList, IBasicVersionControl::Recursive);
    DVCSjob* job = dynamic_cast<DVCSjob*>(j);
    if (job) {
        connect(job, SIGNAL(result(KJob*) ),
                this, SIGNAL(jobFinished(KJob*) ));
        job->start();
    }
}

void DistributedVersionControlPlugin::ctxAdd()
{
    VcsJob* j = add(DistributedVersionControlPlugin::d->m_ctxUrlList, IBasicVersionControl::Recursive);
    DVCSjob* job = dynamic_cast<DVCSjob*>(j);
    if (job) {
        connect(job, SIGNAL(result(KJob*) ),
                this, SIGNAL(jobFinished(KJob*) ));
        job->start();
    }
}

void DistributedVersionControlPlugin::ctxRemove()
{
    VcsJob* j = remove(DistributedVersionControlPlugin::d->m_ctxUrlList);
    DVCSjob* job = dynamic_cast<DVCSjob*>(j);
    if (job) {
        connect(job, SIGNAL( result(KJob*) ),
                this, SIGNAL( jobFinished(KJob*) ));
        job->start();
    }
    connect(job, SIGNAL( result(KJob*) ),
            this, SIGNAL( jobFinished(KJob*) ));
    job->start();
}
//-----------------------------------------------------------------------------------


//class KDevDVCSViewFactory
QWidget* KDevDVCSViewFactory::create(QWidget *parent)
{
         return new DVCSmainView(m_plugin, parent);
}

Qt::DockWidgetArea KDevDVCSViewFactory::defaultPosition()
{
         return Qt::BottomDockWidgetArea;
}

QString KDevDVCSViewFactory::id() const
{
         return "org.kdevelop.DVCSview";
}

#endif