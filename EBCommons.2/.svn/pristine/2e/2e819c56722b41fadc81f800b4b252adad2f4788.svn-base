//
// TODO : fusionner avec Erreur.c ?
//
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "exploit.h"

#include "inout.h"
#include "Erreur.h"
#include "Message.h"
#include "LRUBuf.h"

// pour éviter de déclarer la variable comme extern ici
#define __BatchHelper_C__
#include "BatchHelper.h"
void (*BatchHelper_hook_setFlagKill)(int);

//************************************************************************
// Piège à signaux
//************************************************************************
static void BatchHelper_trapSignal(int sig)
{
  IN();
  Message_print (ERR, "%s(), Signal %d recu, arret du programme...", (char*)__func__,sig);
  if (__Erreur_for_SIGUSR1_flag__ != 0) {
    Message_print (ERR, __Erreur_for_SIGUSR1_msg__);
    Erreur_criticalPrint();
  }
  if (__LRUBuf_for_SIGUSR1_flag__ != 0) {
    Message_print (ERR, __LRUBuf_for_SIGUSR1_msg__);
  }
  // appel de la fonction callback propre au module qui correspond au processus à arrêter
  // c'est la plupart du temps un batch
  if (BatchHelper_hook_setFlagKill != NULL) {
    BatchHelper_hook_setFlagKill(sig);
  }
  else {
    Message_print (ERR, "%s(), Aucune fonction callback pour positionner flagKill, ARRET brutal sur signal [%d]"
                   ,nomFonction
                   ,sig);
    FIN_ERREUR_APPLICATIVE;
  }
  OUT();
}

//************************************************************************
// Piège à signaux
//************************************************************************
void BatchHelper_setSignalstraps(void)
{
  IN();
  signal (SIGUSR1, BatchHelper_trapSignal);
  signal (SIGUSR2, BatchHelper_trapSignal);
  signal (SIGSEGV, BatchHelper_trapSignal);
  OUT();
}

