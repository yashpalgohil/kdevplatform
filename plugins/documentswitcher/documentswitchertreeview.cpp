/***************************************************************************
 *   Copyright 2009 Andreas Pakulat <apaku@gmx.de>                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
#include "documentswitchertreeview.h"

#include <QKeyEvent>

#include <kdebug.h>

#include "documentswitcherplugin.h"

DocumentSwitcherTreeView::DocumentSwitcherTreeView(DocumentSwitcherPlugin* plugin_, QWidget* parent ) 
    : QListView( parent ), plugin( plugin_ )
{
}

void DocumentSwitcherTreeView::keyPressEvent( QKeyEvent* event )
{
    QListView::keyPressEvent(event);
}

void DocumentSwitcherTreeView::keyReleaseEvent( QKeyEvent* event ) 
{
    if( event->key() == Qt::Key_Control ) 
    {
        plugin->switchToView( selectionModel()->currentIndex() );
        event->accept();
        hide();
    } else 
    {
        QListView::keyReleaseEvent(event);
    }
}

