/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2007 Andreas Pakulat <apaku@gmx.de>                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef KDEVPLATFORM_PLUGIN_SVNSTATUSJOB_P_H
#define KDEVPLATFORM_PLUGIN_SVNSTATUSJOB_P_H

#include "svninternaljobbase.h"

#include <vcs/vcsstatusinfo.h>
#include <QUrl>

class SvnInternalStatusJob : public SvnInternalJobBase
{
    Q_OBJECT
public:
    explicit SvnInternalStatusJob( SvnJobBase* parent = nullptr );
    void setLocations( const QList<QUrl>& );
    void setRecursive( bool );

    bool recursive() const;
    QList<QUrl> locations() const;
signals:
    void gotNewStatus( const KDevelop::VcsStatusInfo& );
protected:
    void run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread* thread) override;
private:
    QList<QUrl> m_locations;
    bool m_recursive;
};

#endif

