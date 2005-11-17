/* This file is auto-generated from "ssl_cipher.xml" by the typemaker
   tool of Gwenhywfar. 
   Do not edit this file -- all changes will be lost! */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ssl_cipher_p.h"
#include <gwenhywfar/misc.h>
#include <gwenhywfar/db.h>
#include <gwenhywfar/debug.h>
#include <assert.h>
#include <stdlib.h>
#include <strings.h>



GWEN_LIST_FUNCTIONS(GWEN_SSLCIPHER, GWEN_SslCipher)
GWEN_LIST2_FUNCTIONS(GWEN_SSLCIPHER, GWEN_SslCipher)


GWEN_SSLCIPHER *GWEN_SslCipher_new() {
  GWEN_SSLCIPHER *st;

  GWEN_NEW_OBJECT(GWEN_SSLCIPHER, st)
  st->_usage=1;
  GWEN_LIST_INIT(GWEN_SSLCIPHER, st)
  return st;
}


void GWEN_SslCipher_free(GWEN_SSLCIPHER *st) {
  if (st) {
    assert(st->_usage);
    if (--(st->_usage)==0) {
  GWEN_LIST_FINI(GWEN_SSLCIPHER, st)
  GWEN_FREE_OBJECT(st);
    }
  }

}


GWEN_SSLCIPHER *GWEN_SslCipher_dup(const GWEN_SSLCIPHER *d) {
  GWEN_SSLCIPHER *st;

  assert(d);
  st=GWEN_SslCipher_new();
  if (d->name)
    st->name=strdup(d->name);
  if (d->version)
    st->version=strdup(d->version);
  if (d->description)
    st->description=strdup(d->description);
  st->bits=d->bits;
  return st;
}


int GWEN_SslCipher_toDb(const GWEN_SSLCIPHER *st, GWEN_DB_NODE *db) {
  assert(st);
  assert(db);
  if (st->name)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "name", st->name))
      return -1;
  if (st->version)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "version", st->version))
      return -1;
  if (st->description)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "description", st->description))
      return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "bits", st->bits))
    return -1;
  return 0;
}


GWEN_SSLCIPHER *GWEN_SslCipher_fromDb(GWEN_DB_NODE *db) {
GWEN_SSLCIPHER *st;

  assert(db);
  st=GWEN_SslCipher_new();
  GWEN_SslCipher_SetName(st, GWEN_DB_GetCharValue(db, "name", 0, 0));
  GWEN_SslCipher_SetVersion(st, GWEN_DB_GetCharValue(db, "version", 0, 0));
  GWEN_SslCipher_SetDescription(st, GWEN_DB_GetCharValue(db, "description", 0, 0));
  GWEN_SslCipher_SetBits(st, GWEN_DB_GetIntValue(db, "bits", 0, 0));
  st->_modified=0;
  return st;
}


const char *GWEN_SslCipher_GetName(const GWEN_SSLCIPHER *st) {
  assert(st);
  return st->name;
}


void GWEN_SslCipher_SetName(GWEN_SSLCIPHER *st, const char *d) {
  assert(st);
  if (d)
    st->name=strdup(d);
  else
    st->name=0;
  st->_modified=1;
}




const char *GWEN_SslCipher_GetVersion(const GWEN_SSLCIPHER *st) {
  assert(st);
  return st->version;
}


void GWEN_SslCipher_SetVersion(GWEN_SSLCIPHER *st, const char *d) {
  assert(st);
  if (d)
    st->version=strdup(d);
  else
    st->version=0;
  st->_modified=1;
}




const char *GWEN_SslCipher_GetDescription(const GWEN_SSLCIPHER *st) {
  assert(st);
  return st->description;
}


void GWEN_SslCipher_SetDescription(GWEN_SSLCIPHER *st, const char *d) {
  assert(st);
  if (d)
    st->description=strdup(d);
  else
    st->description=0;
  st->_modified=1;
}




int GWEN_SslCipher_GetBits(const GWEN_SSLCIPHER *st) {
  assert(st);
  return st->bits;
}


void GWEN_SslCipher_SetBits(GWEN_SSLCIPHER *st, int d) {
  assert(st);
  st->bits=d;
  st->_modified=1;
}




int GWEN_SslCipher_IsModified(const GWEN_SSLCIPHER *st) {
  assert(st);
  return st->_modified;
}


void GWEN_SslCipher_SetModified(GWEN_SSLCIPHER *st, int i) {
  assert(st);
  st->_modified=i;
}


void GWEN_SslCipher_Attach(GWEN_SSLCIPHER *st) {
  assert(st);
  st->_usage++;
}
GWEN_SSLCIPHER *GWEN_SslCipher_List2__freeAll_cb(GWEN_SSLCIPHER *st, void *user_data) {
  GWEN_SslCipher_free(st);
return 0;
}


void GWEN_SslCipher_List2_freeAll(GWEN_SSLCIPHER_LIST2 *stl) {
  if (stl) {
    GWEN_SslCipher_List2_ForEach(stl, GWEN_SslCipher_List2__freeAll_cb, 0);
    GWEN_SslCipher_List2_free(stl); 
  }
}




GWEN_SSLCIPHER_LIST *GWEN_SslCipher_List_dup(const GWEN_SSLCIPHER_LIST *stl) {
  if (stl) {
    GWEN_SSLCIPHER_LIST *nl;
    GWEN_SSLCIPHER *e;

    nl=GWEN_SslCipher_List_new();
    e=GWEN_SslCipher_List_First(stl);
    while(e) {
      GWEN_SSLCIPHER *ne;

      ne=GWEN_SslCipher_dup(e);
      assert(ne);
      GWEN_SslCipher_List_Add(ne, nl);
      e=GWEN_SslCipher_List_Next(e);
    } /* while (e) */
    return nl;
  }
  else
    return 0;
}


