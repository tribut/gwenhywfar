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


#ifndef GWEN_KEYSPEC_P_H
#define GWEN_KEYSPEC_P_H

#include <gwenhywfar/keyspec.h>

struct GWEN_KEYSPEC {
  GWEN_INHERIT_ELEMENT(GWEN_KEYSPEC)
  GWEN_LIST_ELEMENT(GWEN_KEYSPEC)
  int status;
  char *keyType;
  char *keyName;
  char *owner;
  unsigned int number;
  unsigned int version;
  unsigned int keyLength;
};


GWEN_KEYSPEC *GWEN_KeySpec_List2__freeAll_cb(GWEN_KEYSPEC *st,
                                             void *user_data);




#endif /* GWEN_KEYSPEC_P_H */


