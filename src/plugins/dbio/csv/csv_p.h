/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Thu Oct 30 2003
 copyright   : (C) 2003 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GWENHYWFAR_PLUGIN_CSV_H
#define GWENHYWFAR_PLUGIN_CSV_H


#include <gwenhywfar/bufferedio.h>
#include <gwenhywfar/db.h>
#include <gwenhywfar/dbio.h>


GWEN_DBIO *csv_factory();


int GWEN_DBIO_CSV_Export(GWEN_DBIO *dbio,
			 GWEN_BUFFEREDIO *bio,
			 GWEN_TYPE_UINT32 flags,
			 GWEN_DB_NODE *data,
			 GWEN_DB_NODE *cfg);

int GWEN_DBIO_CSV_Import(GWEN_DBIO *dbio,
			 GWEN_BUFFEREDIO *bio,
			 GWEN_TYPE_UINT32 flags,
			 GWEN_DB_NODE *data,
			 GWEN_DB_NODE *cfg);



#endif


