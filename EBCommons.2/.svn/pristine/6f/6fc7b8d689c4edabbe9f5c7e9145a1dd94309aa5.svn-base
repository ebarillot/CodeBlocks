/************************************************************************/
/*  ____________________________________________________________________*/
/* |        |         |            |                                   |*/
/* |  DATE  | Version |  Auteur    | Commentaires                      |*/
/* |--------|---------|------------|-----------------------------------|*/
/* |2012/06 |   01    | E.BARILLOT | Création                          |*/
/*  ------------------------------------------------------------------- */
/*                                                                      */
/************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>	/* pour struct pm */
#include <stdarg.h>	/* pour gérer les va_list */

#include <exploit.h>	/* TestEcriture(), Log(), Print() */

#include "Message.h"
#include "Erreur.h"
#include "inout.h"

// contexte pour utilisation du module
// Evite d'avoir recours à une structure externe
typedef struct _MessageContexte {
  int flagLog;
  int debug;
  int debugMem;
  void (*funLog)();
  void (*funPrint)();
} MessageContexte;


static MessageContexte messageCtx = {0,0,0, NULL, NULL};

// fonction qui encapsule la macro Print de exploit.c pour pouvoir mettre
// un pointeur dessus
static void __funPrintDefault (char* fmt, ...);


//************************************************************************
// Positionnement du contexte
//************************************************************************
void Message_setContexte (int flagLog, int debug, int debugMem)
{
  messageCtx.flagLog  = flagLog;
  messageCtx.debug    = debug;
  messageCtx.debugMem = debugMem;
  if (messageCtx.funLog == NULL) {
    Message_setFunLogDefault ();
  }
  if (messageCtx.funPrint == NULL) {
    Message_setFunPrintDefault ();
  }
  return;
}

///////////////////////////////////////////////////////////////////////////
void Message_setFunLog (void (*funLog)())
{
  messageCtx.funLog = funLog;
  return;
}

///////////////////////////////////////////////////////////////////////////
void Message_setFunLogDefault (void)
{
  messageCtx.funLog = (void (*)())Log;  // fonction de exploit.c
  return;
}

///////////////////////////////////////////////////////////////////////////
void Message_setFunPrint (void (*funPrint)())
{
  messageCtx.funPrint = funPrint;
  return;
}

///////////////////////////////////////////////////////////////////////////
void Message_setFunPrintDefault (void)
{
  messageCtx.funPrint = (void (*)())__funPrintDefault;
  return;
}


///////////////////////////////////////////////////////////////////////////
  // reprend le contenu de la macro Print de exploit.h
static void __funPrintDefault (char* fmt, ...)
{
  va_list ap;
  char avancement[L_messageErreur+1] = "";
  memset (avancement, 0, L_messageErreur+1);
  if (NULL != fmt) {
    va_start (ap, fmt);
    vsnprintf (avancement, L_messageErreur, fmt, ap);
    va_end (ap);
  }
  printf("%s ",DateSystem());
  PrintExploit("%s", avancement);
  return;
}



/************************************************************************/
// Message dans stdout et Log
/************************************************************************/
int Message_print (char* tag, char* fmt, ...)
{
  va_list ap;
  char avancement[L_messageErreur+1] = "";
  memset (avancement, 0, L_messageErreur+1);
  // fonctions par defaut pour compatibilité ascendante
  if (messageCtx.funPrint == NULL) {
    Message_setFunPrintDefault ();
  }
  if (messageCtx.funLog == NULL) {
    Message_setFunLogDefault ();
  }
  // message
  if (NULL != fmt) {
    va_start (ap, fmt);
    vsnprintf (avancement, L_messageErreur, fmt, ap);
    va_end (ap);
  }
  if (messageCtx.funPrint) messageCtx.funPrint ("%s,%s\n", tag, avancement);
  if (messageCtx.flagLog) {
    if (messageCtx.funLog) messageCtx.funLog (tag, "%s", avancement);
  }
  return 0;
}

/************************************************************************/
// Message Debug dans stdout (rien dans Log)
/************************************************************************/
int Message_printDebug (char* where, char* fmt, ...)
{
  va_list ap;
  char avancement[L_messageErreur+1];
  memset (avancement, 0, L_messageErreur+1);
  if (strcmp(where,TAG_MALLOC)==0 || strcmp(where,TAG_FREE)==0) {
    if (messageCtx.debugMem) {
      if (NULL != fmt) {
        va_start (ap, fmt);
        vsnprintf(avancement, L_messageErreur, fmt, ap);
        va_end (ap);
      }
      if (messageCtx.funPrint) messageCtx.funPrint("%s, %s\n", where, avancement);
      if (messageCtx.flagLog) {
        if (messageCtx.funLog) messageCtx.funLog(where, "%s", avancement);
      }
    }
  }
  else
  {
    if (messageCtx.debug) {
      if (NULL != fmt) {
        va_start (ap, fmt);
        vsnprintf(avancement, L_messageErreur, fmt, ap);
        va_end (ap);
      }
      if (messageCtx.funPrint) messageCtx.funPrint("%s, %s\n", where, avancement);
      if (messageCtx.flagLog) {
        if (messageCtx.funLog) messageCtx.funLog(where, "%s", avancement);
      }
    }
  }
  return 0;
}


