/*
///////////////////////////////////////////////////////////////////////////
// 2016/11 E.Barillot
//
///////////////////////////////////////////////////////////////////////////
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "test_unit.h"

#include "Erreur.h"
#include "Message.h"
#include "inout.h"
#include "BatchHelper.h"

///////////////////////////////////////////////////////////////////////////////
// Piège à signaux
static void trapSignal (int sig)
{
  switch (sig)
  {
    case SIGUSR1 :
      Message_print (ERR, "%s(), ERREUR UNIX SIGUSR1",__func__);
      break;
    case SIGUSR2 :
      Message_print (ERR, "%s(), ERREUR UNIX SIGUSR2",__func__);
      break;
    case SIGSEGV :
      Message_print (ERR, "%s(), ERREUR UNIX SIGSEGV",__func__);
      break;
    default :
      Message_print (ERR, "%s(), signal reçu non traité: %d",__func__,sig);
      break;
  }
  exit(-1);
}


/************* Fonctions Hook (debut et fin de tests) *******************/

void begin() 
{
  //  Message_print("VERSION","Version [%s]", BATCH_VERSION);
//  BatchHelper_hook_setFlagKill = trapSignal;
  BatchHelper_setSignalstraps();
}

///////////////////////////////////////////////////////////////////////////
void setUp()
{
}

///////////////////////////////////////////////////////////////////////////
void tearDown()
{
}

void finish()
{
}


/************* Definition des Tests **************************************/


///////////////////////////////////////////////////////////////////////////
// que teste on : les fonctions de gestion du buffer des erreurs
///////////////////////////////////////////////////////////////////////////
void test_Erreur(void)
{
#define N_erreurs_test  10
  size_t errt[N_erreurs_test];
  int i;
  for (i=0;i<N_erreurs_test;i++) {
    errt[i] = Erreur_set(CLA_APP,(char*)__func__,"Erreur %d",(i+1));
    char msg[500];
    memset(msg,0,500);
    sprintf(msg,"Erreur %d",(i+1));
    TEST_EQU_STR_MSG(msg,Erreur_getMessage(Default_lastErreur),"Erreur %d filling",(i+1));
  }
  // la pile des erreurs qui sont bien définies
  Erreur_criticalPrint();
  return;
#undef N_erreurs_test
}


///////////////////////////////////////////////////////////////////////////
// que teste on : un nb d'erreurs crées au delà de la limite
///////////////////////////////////////////////////////////////////////////
void test_ErreurMax(void)
{
#define N_erreurs_test  1001
  size_t errt[N_erreurs_test];
  int i;
  for (i=0;i<N_erreurs_test;i++) {
    errt[i] = Erreur_set(CLA_APP,(char*)__func__,"Erreur %d",(i+1));
    char msg[500];
    memset(msg,0,500);
    sprintf(msg,"Erreur %d",(i+1));
    TEST_EQU_STR_MSG(msg,Erreur_getMessage(Default_lastErreur),"Erreur %d filling",(i+1));
  }
  return;
#undef N_erreurs_test
}


/************* Lancement des Tests **************************************/


void test()
{
  RUN(test_Erreur);
//  RUN(test_ErreurMax);  // attention, plante le processus, mais c'est l'effet recherché
}


