/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Tue Sep 16 2003
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


#ifndef GWENHYFWAR_CONNLAYER_P_H
#define GWENHYFWAR_CONNLAYER_P_H

#define GWEN_IPCCONNLAYER_MAXINCOMING_MSGS 32
#define GWEN_IPCCONNLAYER_MAXOUTGOING_MSGS 32


#include <gwenhyfwar/connlayer.h>


struct GWEN_IPCCONNLAYER {
  GWEN_IPCCONNLAYER *next;

  GWEN_IPCMSGLAYER *msgLayer;
  GWEN_IPCCONNLAYER_STATE state;
  GWEN_IPCMSG *incomingMsgs;
  unsigned int nIncomingMsgs;
  unsigned int maxIncomingMsgs;
  GWEN_IPCMSG *outgoingMsgs;
  unsigned int nOutgoingMsgs;
  unsigned int maxOutgoingMsgs;

  unsigned int userMark;
  unsigned int libMark;
  unsigned int flags;

  char *info;
  void *data;

  GWEN_IPCCONNLAYER_FREE freeDataFn;
  GWEN_IPCCONNLAYER_OPEN openFn;
  GWEN_IPCCONNLAYER_CLOSE closeFn;
  GWEN_IPCCONNLAYER_WORK workFn;
  GWEN_IPCCONNLAYER_ACCEPT acceptFn;
};


#endif /* GWENHYFWAR_CONNLAYER_P_H */




