/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Fri Feb 07 2003
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


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#define DISABLE_DEBUGLOG


#include "bufferedio_p.h"
#include <gwenhywfar/misc.h>
#include <gwenhywfar/text.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <gwenhywfar/debug.h>


GWEN_INHERIT_FUNCTIONS(GWEN_BUFFEREDIO);


const char *GWEN_BufferedIO_ErrorString(int c){
  const char *s;

  switch(c) {
  case GWEN_BUFFEREDIO_ERROR_READ:
    s="Error on read";
    break;
  case GWEN_BUFFEREDIO_ERROR_WRITE:
    s="Error on write";
    break;
  case GWEN_BUFFEREDIO_ERROR_CLOSE:
    s="Error on write";
    break;
  case GWEN_BUFFEREDIO_ERROR_TIMEOUT:
    s="Transaction timout";
    break;
  case GWEN_BUFFEREDIO_ERROR_EOF:
    s="EOF met";
    break;
  case GWEN_BUFFEREDIO_ERROR_NO_DATA:
    s="Could not read/write data";
    break;
  default:
    s=0;
  } /* switch */

  return s;
}



static int gwen_bufferedio_is_initialized=0;
static GWEN_ERRORTYPEREGISTRATIONFORM *gwen_bufferedio_errorform=0;



GWEN_ERRORCODE GWEN_BufferedIO_ModuleInit(){
  if (!gwen_bufferedio_is_initialized) {
    GWEN_ERRORCODE err;

    gwen_bufferedio_errorform=GWEN_ErrorType_new();
    GWEN_ErrorType_SetName(gwen_bufferedio_errorform,
                           GWEN_BUFFEREDIO_ERROR_TYPE);
    GWEN_ErrorType_SetMsgPtr(gwen_bufferedio_errorform,
                             GWEN_BufferedIO_ErrorString);
    err=GWEN_Error_RegisterType(gwen_bufferedio_errorform);
    if (!GWEN_Error_IsOk(err))
      return err;
    gwen_bufferedio_is_initialized=1;
  }
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_ModuleFini(){
  if (gwen_bufferedio_is_initialized) {
    GWEN_ERRORCODE err;

    err=GWEN_Error_UnregisterType(gwen_bufferedio_errorform);
    GWEN_ErrorType_free(gwen_bufferedio_errorform);
    if (!GWEN_Error_IsOk(err))
      return err;
    gwen_bufferedio_is_initialized=0;
  }
  return 0;
}



GWEN_BUFFEREDIO *GWEN_BufferedIO_new(){
  GWEN_BUFFEREDIO *bt;

  GWEN_NEW_OBJECT(GWEN_BUFFEREDIO, bt);
  GWEN_INHERIT_INIT(GWEN_BUFFEREDIO, bt);
  bt->lineMode=GWEN_LineModeUnix;
  return bt;
}



void GWEN_BufferedIO_free(GWEN_BUFFEREDIO *bt){
  if (bt) {
    GWEN_INHERIT_FINI(GWEN_BUFFEREDIO, bt);
    free(bt->readerBuffer);
    bt->readerBuffer=0;
    free(bt->writerBuffer);
    bt->writerBuffer=0;
    free(bt);
  }
}



void GWEN_BufferedIO_SetReadBuffer(GWEN_BUFFEREDIO *bt,
                                   char *buffer, int len){
  assert(bt);
  free(bt->readerBuffer);
  bt->readerBuffer=0;
  if (buffer==0) {
    if (len>0) {
      bt->readerBuffer=malloc(len);
      assert(bt->readerBuffer);
    }
  }
  else
    bt->readerBuffer=buffer;

  bt->readerBufferLength=len;
  bt->readerBufferFilled=0;
  bt->readerBufferPos=0;
}



void GWEN_BufferedIO_SetWriteBuffer(GWEN_BUFFEREDIO *bt, char *buffer, int len){
  assert(bt);
  free(bt->writerBuffer);
  bt->writerBuffer=0;
  if (buffer==0) {
    if (len>0) {
      bt->writerBuffer=malloc(len);
      assert(bt->writerBuffer);
    }
  }
  else
    bt->writerBuffer=buffer;

  bt->writerBufferLength=len;
  bt->writerBufferFilled=0;
  bt->writerBufferPos=0;
}



int GWEN_BufferedIO_CheckEOF(GWEN_BUFFEREDIO *bt){
  return (GWEN_BufferedIO_PeekChar(bt)==GWEN_BUFFEREDIO_CHAR_EOF);
}



int GWEN_BufferedIO_PeekChar(GWEN_BUFFEREDIO *bt){
  assert(bt);
  assert(bt->readerBuffer);

  /* do some fast checks */
  if (bt->readerError) {
    DBG_DEBUG(0, "Error flagged");
    return GWEN_BUFFEREDIO_CHAR_ERROR;
  }
  if (bt->readerEOF) {
    DBG_DEBUG(0, "EOF flagged");
    return GWEN_BUFFEREDIO_CHAR_EOF;
  }

  if (bt->readerBufferPos>=bt->readerBufferFilled) {
    /* buffer empty, no EOF met, so fill it */
    GWEN_ERRORCODE err;
    int i;

    assert(bt->readPtr);
    i=bt->readerBufferLength;
    err=bt->readPtr(bt,
		    bt->readerBuffer,
		    &i,
		    bt->timeout);
    if (!GWEN_Error_IsOk(err)) {
      if (GWEN_Error_GetType(err)==
          GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE) &&
          GWEN_Error_GetCode(err)==GWEN_BUFFEREDIO_ERROR_NO_DATA) {
        DBG_INFO(0, "Could not fill input buffer, no data");
        return GWEN_BUFFEREDIO_CHAR_NO_DATA;
      }
      DBG_ERROR_ERR(0, err);
      bt->readerError=1;
      return GWEN_BUFFEREDIO_CHAR_ERROR;
    }
    bt->readerBufferFilled=i;
    bt->readerBufferPos=0;
    bt->readerEOF=(i==0);
  }
  if (bt->readerEOF) {
    DBG_DEBUG(0, "EOF now met");
    return GWEN_BUFFEREDIO_CHAR_EOF;
  }
  return (unsigned char)(bt->readerBuffer[bt->readerBufferPos]);
}



int GWEN_BufferedIO_ReadChar(GWEN_BUFFEREDIO *bt){
  int i;

  i=GWEN_BufferedIO_PeekChar(bt);
  if (i>=0)
    bt->readerBufferPos++;
  return i;
}



GWEN_ERRORCODE GWEN_BufferedIO_Flush(GWEN_BUFFEREDIO *bt){
  GWEN_ERRORCODE err;
  int i;
  int written;

  assert(bt);
  if (bt->writerBufferFilled==0) {
    DBG_DEBUG(0, "WriteBuffer empty, nothing to flush.");
    return 0;
  }
  assert(bt->writerBuffer);
  assert(bt->writePtr);
  written=bt->writerBufferFlushPos;
  DBG_DEBUG(0, "Flushing %d bytes", bt->writerBufferFilled);
  while(written<bt->writerBufferFilled) {
    i=bt->writerBufferFilled-written;
    err=bt->writePtr(bt,
		     &(bt->writerBuffer[written]),
		     &i,
		     bt->timeout);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      return err;
    }
    written+=i;
  } /* while */

  bt->writerBufferPos=0;
  bt->writerBufferFilled=0;
  bt->writerBufferFlushPos=0;
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_ShortFlush(GWEN_BUFFEREDIO *bt){
  GWEN_ERRORCODE err;
  int i;

  assert(bt);
  if (bt->writerBufferFilled==0) {
    DBG_DEBUG(0, "WriteBuffer empty, nothing to flush.");
    return 0;
  }
  assert(bt->writerBuffer);
  assert(bt->writePtr);
  i=bt->writerBufferFilled-bt->writerBufferFlushPos;
  DBG_DEBUG(0, "Flushing %d bytes", i);
  err=bt->writePtr(bt,
		   &(bt->writerBuffer[bt->writerBufferFlushPos]),
		   &i,
		   bt->timeout);
  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }
  if (i<bt->writerBufferFilled-bt->writerBufferFlushPos) {
    /* partial flush */
    bt->writerBufferFlushPos+=i;
    return GWEN_Error_new(0,
			  GWEN_ERROR_SEVERITY_WARN,
			  GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE),
			  GWEN_BUFFEREDIO_ERROR_PARTIAL);
  }
  else {
    /* all bytes written, flush finished */
    bt->writerBufferFlushPos=0;
    bt->writerBufferPos=0;
    bt->writerBufferFilled=0;
    return 0;
  }
}



int GWEN_BufferedIO_ReadBufferEmpty(GWEN_BUFFEREDIO *bt) {
  assert(bt);
  return ((bt->readerBuffer==0) ||
          !bt->readerBufferFilled ||
          bt->readerBufferPos>=bt->readerBufferFilled);
}



int GWEN_BufferedIO_WriteBufferEmpty(GWEN_BUFFEREDIO *bt) {
  assert(bt);
  return ((bt->writerBuffer==0)  ||
          !bt->writerBufferFilled ||
          bt->writerBufferPos>=bt->writerBufferFilled);
}



GWEN_ERRORCODE GWEN_BufferedIO_WriteChar(GWEN_BUFFEREDIO *bt, char c){
  assert(bt);
  assert(bt->writerBuffer);

  /* flush buffer if needed (only needed if last flush attempt failed) */
  if (bt->writerBufferFilled>=bt->writerBufferLength) {
    GWEN_ERRORCODE err;

    err=GWEN_BufferedIO_Flush(bt);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      return err;
    }
  }

  /* write char to buffer */
  bt->writerBuffer[bt->writerBufferPos++]=c;
  if (bt->writerBufferPos>bt->writerBufferFilled)
    bt->writerBufferFilled=bt->writerBufferPos;

  /* flush buffer if needed */
  if (bt->writerBufferFilled>=bt->writerBufferLength) {
    GWEN_ERRORCODE err;

    err=GWEN_BufferedIO_Flush(bt);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      return err;
    }
  }
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_Close(GWEN_BUFFEREDIO *bt){
  GWEN_ERRORCODE err, err2;

  assert(bt);
  assert(bt->closePtr);
  err=GWEN_BufferedIO_Flush(bt);
  if (bt->flags & GWEN_BUFFEREDIO_FLAGS_CLOSE)
    err2=bt->closePtr(bt);
  else
    err2=0;

  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }
  if (!GWEN_Error_IsOk(err2)) {
    DBG_ERROR_ERR(0, err2);
    return err2;
  }
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_Abandon(GWEN_BUFFEREDIO *bt){
  GWEN_ERRORCODE err;

  assert(bt);
  assert(bt->closePtr);
  err=bt->closePtr(bt);
  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_ReadLine(GWEN_BUFFEREDIO *bt,
                                        char *buffer,
                                        unsigned int s){
  int c;
  int pos;

  assert(s);
  pos=0;
  /* now read */
  while(s>1) {
    if (GWEN_BufferedIO_CheckEOF(bt)) {
      buffer[pos]=0;
      break;
    }
    c=GWEN_BufferedIO_ReadChar(bt);
    if (c==GWEN_BUFFEREDIO_CHAR_NO_DATA) {
      DBG_INFO(0, "No more data for now");
      break;
    }
    if (c<0) {
      DBG_ERROR(0, "Error while reading");
      return GWEN_Error_new(0,
                            GWEN_ERROR_SEVERITY_ERR,
                            GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE),
                            GWEN_BUFFEREDIO_ERROR_READ);
    }

    if (c==GWEN_BUFFEREDIO_LF) {
      /* LF ends every line */
      buffer[pos]=0;
      break;
    }

    if (c!=GWEN_BUFFEREDIO_CR || bt->lineMode==GWEN_LineModeUnix) {
      buffer[pos]=(unsigned char)c;
      pos++;
      s--;
    }
  } /* while */

  /* add terminating null */
  if (s)
    buffer[pos]=0;

  /* reading done */
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_Write(GWEN_BUFFEREDIO *bt,
                                     const char *buffer){
  GWEN_ERRORCODE err;

  assert(bt);
  assert(buffer);
  while(*buffer) {
    err=GWEN_BufferedIO_WriteChar(bt, *buffer);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      return err;
    }
    buffer++;
  } /* while */
  return 0;
}



GWEN_ERRORCODE GWEN_BufferedIO_WriteLine(GWEN_BUFFEREDIO *bt,
                                         const char *buffer){
  GWEN_ERRORCODE err;

  assert(bt);
  assert(buffer);
  err=GWEN_BufferedIO_Write(bt, buffer);
  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }
  if (bt->lineMode==GWEN_LineModeDOS) {
    err=GWEN_BufferedIO_WriteChar(bt, GWEN_BUFFEREDIO_CR);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      return err;
    }
  }
  err=GWEN_BufferedIO_WriteChar(bt, GWEN_BUFFEREDIO_LF);
  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }

  err=GWEN_BufferedIO_Flush(bt);
  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }

  return 0;
}



void GWEN_BufferedIO_SetLineMode(GWEN_BUFFEREDIO *dm,
                                 GWEN_BUFFEREDIOLINEMODE lm){
  assert(dm);
  dm->lineMode=lm;
}



GWEN_BUFFEREDIOLINEMODE GWEN_BufferedIO_GetLineMode(GWEN_BUFFEREDIO *dm){
  assert(dm);
  return dm->lineMode;
}



void GWEN_BufferedIO_SetTimeout(GWEN_BUFFEREDIO *dm, int timeout){
  assert(dm);
  dm->timeout=timeout;
}



int GWEN_BufferedIO_GetTimeout(GWEN_BUFFEREDIO *dm){
  assert(dm);
  return dm->timeout;
}



GWEN_ERRORCODE GWEN_BufferedIO_WriteRaw(GWEN_BUFFEREDIO *bt,
                                        const char *buffer,
                                        unsigned int *bsize){

  GWEN_ERRORCODE err;
  int i;

  assert(bt);
  assert(bsize);
  assert(*bsize);

  if (bt->writerBufferFilled) {
    /* some data in the buffer, this must be flushed first */
    err=GWEN_BufferedIO_ShortFlush(bt);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      return err;
    }
    if (GWEN_Error_GetType(err)==
        GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE) &&
        GWEN_Error_GetCode(err)==GWEN_BUFFEREDIO_ERROR_PARTIAL) {
      /* still some bytes in the buffer, can not write right now */
      *bsize=0;
      return err;
    }
  }

  /* try to write as many bytes as possible */
  i=*bsize;
  err=bt->writePtr(bt,
                   buffer,
                   &i,
                   bt->timeout);
  if (!GWEN_Error_IsOk(err)) {
    DBG_ERROR_ERR(0, err);
    return err;
  }
  *bsize=i;
  return err;
}



GWEN_ERRORCODE GWEN_BufferedIO_ReadRaw(GWEN_BUFFEREDIO *bt,
                                       char *buffer,
                                       unsigned int *bsize){
  assert(bt);

  /* do some fast checks */
  if (bt->readerError) {
    DBG_INFO(0, "Error flagged");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_WARN,
                          GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE),
                          GWEN_BUFFEREDIO_ERROR_READ);
  }
  if (bt->readerEOF) {
    DBG_INFO(0, "EOF flagged");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_WARN,
                          GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE),
                          GWEN_BUFFEREDIO_ERROR_READ);
  }

  if (bt->readerBufferPos<bt->readerBufferFilled) {
    /* buffer not empty, so read from the buffer first */
    unsigned int i;

    i=bt->readerBufferFilled-bt->readerBufferPos;
    if (i>*bsize)
      i=*bsize;
    DBG_DEBUG(0, "Reading rest from buffer (%d at %d of %d)",
             i,bt->readerBufferPos, bt->readerBufferFilled);

    if (i) {
      /* copy as much bytes as needed, advance pointer */
      memmove(buffer, bt->readerBuffer+bt->readerBufferPos, i);
      bt->readerBufferPos+=i;
    }
    *bsize=i;
    DBG_DEBUG(0, "Read %d bytes from buffer", i);
    return 0;
  }
  else {
    /* buffer empty, so read directly from source */
    GWEN_ERRORCODE err;
    int i;

    DBG_DEBUG(0, "Reading directly from source");
    assert(bt->readPtr);
    i=*bsize;
    err=bt->readPtr(bt,
                    buffer,
                    &i,
                    bt->timeout);
    if (!GWEN_Error_IsOk(err)) {
      DBG_ERROR_ERR(0, err);
      bt->readerError=1;
      return err;
    }
    bt->readerEOF=(i==0);
    *bsize=i;
    DBG_DEBUG(0, "Read %d bytes from source", i);
  }
  if (bt->readerEOF) {
    DBG_DEBUG(0, "EOF now met");
    return GWEN_Error_new(0,
                          GWEN_ERROR_SEVERITY_WARN,
                          GWEN_Error_FindType(GWEN_BUFFEREDIO_ERROR_TYPE),
                          GWEN_BUFFEREDIO_ERROR_EOF);
  }
  return 0;
}



GWEN_TYPE_UINT32 GWEN_BufferedIO_GetFlags(const GWEN_BUFFEREDIO *bt) {
  assert(bt);
  return bt->flags;
}



void GWEN_BufferedIO_SetFlags(GWEN_BUFFEREDIO *bt, GWEN_TYPE_UINT32 f) {
  assert(bt);
  bt->flags=f;
}



void GWEN_BufferedIO_AddFlags(GWEN_BUFFEREDIO *bt, GWEN_TYPE_UINT32 f) {
  assert(bt);
  bt->flags|=f;
}



void GWEN_BufferedIO_SubFlags(GWEN_BUFFEREDIO *bt, GWEN_TYPE_UINT32 f) {
  assert(bt);
  bt->flags&=~f;
}



void GWEN_BufferedIO_SetReadFn(GWEN_BUFFEREDIO *dm,
                               GWEN_BUFFEREDIOREADFN fn){
  assert(dm);
  dm->readPtr=fn;
}



void GWEN_BufferedIO_SetWriteFn(GWEN_BUFFEREDIO *dm,
                                GWEN_BUFFEREDIOWRITEFN fn){
  assert(dm);
  dm->writePtr=fn;
}



void GWEN_BufferedIO_SetCloseFn(GWEN_BUFFEREDIO *dm,
                                GWEN_BUFFEREDIOCLOSEFN fn){
  assert(dm);
  dm->closePtr=fn;
}












