/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Sat Nov 08 2003
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


#ifndef GWEN_IPCXMLCONNLAYER_H
#define GWEN_IPCXMLCONNLAYER_H


#include <gwenhywfar/servicelayer.h>
#include <gwenhywfar/connlayer.h>
#include <gwenhywfar/transportlayer.h>
#include <gwenhywfar/msgengine.h>
#include <gwenhywfar/crypt.h>
#include <gwenhywfar/keymanager.h>
#include <gwenhywfar/hbcimsg.h>

#include <gwenhywfar/ipcxml.h>

/** @defgroup MOD_IPCXML_CONNLAYER IPCXML Connection Layer
 * @ingroup MOD_IPC_MIDLEVEL
 * @brief Extends @ref MOD_IPC_CONNLAYER
 *
 * This module works on top of @ref MOD_IPCXML_MSGLAYER.
 */
/*@{*/
typedef struct GWEN_IPCXMLCONNLAYERDATA GWEN_IPCXMLCONNLAYERDATA;


#define GWEN_IPCXMLCONNLAYER_FLAGS_VERIFIED     0x00010000
#define GWEN_IPCXMLCONNLAYER_FLAGS_ALLOW_ADDCTX 0x00020000


typedef void
  (*GWEN_IPCXMLCONNLAYER_CONNECTED_FN)(GWEN_SERVICELAYER *sl,
                                       GWEN_IPCCONNLAYER *cl);

typedef void
  (*GWEN_IPCXMLCONNLAYER_DISCONNECTED_FN)(GWEN_SERVICELAYER *sl,
                                          GWEN_IPCCONNLAYER *cl);



GWEN_IPCXMLCONNLAYERDATA *GWEN_IPCXMLConnLayerData_new();
void GWEN_IPCXMLConnLayerData_free(GWEN_IPCXMLCONNLAYERDATA *ccd);


GWEN_IPCCONNLAYER *GWEN_IPCXMLConnLayer_new(GWEN_SERVICELAYER *sl,
                                            GWEN_MSGENGINE *msgEngine,
                                            const char *localName,
                                            GWEN_SECCTX_MANAGER *scm,
                                            GWEN_IPCMSGLAYER *ml,
                                            int active);

GWEN_MSGENGINE *GWEN_IPCXMLConnLayer_GetMsgEngine(GWEN_IPCCONNLAYER *cl);

GWEN_ERRORCODE GWEN_IPCXMLConnLayer_Flush(GWEN_IPCCONNLAYER *cl);

GWEN_IPCXMLREQUEST *GWEN_IPCXMLConnLayer_AddRequest(GWEN_IPCCONNLAYER *cl,
                                                    GWEN_XMLNODE *node,
                                                    GWEN_DB_NODE *db,
                                                    unsigned int flags);

GWEN_ERRORCODE GWEN_IPCXMLConnLayer_AddResponse(GWEN_IPCCONNLAYER *cl,
                                                GWEN_IPCXMLREQUEST *rq,
                                                GWEN_XMLNODE *node,
                                                GWEN_DB_NODE *db,
                                                unsigned int flags);

GWEN_HBCIMSG *GWEN_IPCXMLConnLayer_IPC2HBCI(GWEN_IPCCONNLAYER *cl,
                                            GWEN_IPCMSG *msg);

const char *GWEN_IPCXMLConnLayer_GetLocalName(GWEN_IPCCONNLAYER *cl);

const char *GWEN_IPCXMLConnLayer_GetRemoteName(GWEN_IPCCONNLAYER *cl);
void GWEN_IPCXMLConnLayer_SetRemoteName(GWEN_IPCCONNLAYER *cl,
                                        const char *s);

const char *GWEN_IPCXMLConnLayer_GetServiceCode(GWEN_IPCCONNLAYER *cl);


unsigned int GWEN_IPCXMLConnLayer_GetSecurityFlags(GWEN_IPCCONNLAYER *cl);

GWEN_ERRORCODE GWEN_IPCXMLConnLayer_SetSecurityFlags(GWEN_IPCCONNLAYER *cl,
                                                     unsigned int flags);

unsigned int GWEN_IPCXMLConnLayer_GetSessionId(GWEN_IPCCONNLAYER *cl);
void GWEN_IPCXMLConnLayer_SetSessionId(GWEN_IPCCONNLAYER *cl,
                                       unsigned int id);


void GWEN_IPCXMLConnLayer_Connected(GWEN_IPCCONNLAYER *cl);
void GWEN_IPCXMLConnLayer_Disconnected(GWEN_IPCCONNLAYER *cl);

void GWEN_IPCXMLConnLayer_SetConnectedFn(GWEN_IPCCONNLAYER *cl,
                                         GWEN_IPCXMLCONNLAYER_CONNECTED_FN f);
void GWEN_IPCXMLConnLayer_SetDisconnectedFn(GWEN_IPCCONNLAYER *cl,
       GWEN_IPCXMLCONNLAYER_DISCONNECTED_FN f);


const GWEN_CRYPTKEY*
  GWEN_IPCXMLConnLayer_GetSignKey(GWEN_IPCCONNLAYER *cl);

const GWEN_CRYPTKEY*
  GWEN_IPCXMLConnLayer_GetCryptKey(GWEN_IPCCONNLAYER *cl);

void GWEN_IPCXMLConnLayer_SetSignKey(GWEN_IPCCONNLAYER *cl,
                                     const GWEN_CRYPTKEY *key);
void GWEN_IPCXMLConnLayer_SetCryptKey(GWEN_IPCCONNLAYER *cl,
                                      const GWEN_CRYPTKEY *key);


/*@}*/

#endif /* GWEN_IPCXMLCONNLAYER_H */



