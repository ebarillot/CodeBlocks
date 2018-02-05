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

#include "LRUBuf.h"
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
  BatchHelper_hook_setFlagKill = trapSignal;
  BatchHelper_setSignalstraps();
}

///////////////////////////////////////////////////////////////////////////
void setUp()
{
  Erreur_cleanAll();
}

///////////////////////////////////////////////////////////////////////////
void tearDown()
{
  Erreur_cleanAll();
}

void finish()
{
}


/************* Definition des Tests **************************************/


///////////////////////////////////////////////////////////////////////////
// que teste on : les fonctions de gestion du buffer LRU par défaut
///////////////////////////////////////////////////////////////////////////
void test_LRUBuf(void)
{
  char* line_1 = LRUBuf_get(DEFAULT_LRUBufId);
  TEST_NOT_NULL_MSG(line_1,"line_1 not null");
  strcpy(line_1,"une information");
  TEST_EQU_STR_MSG("une information",line_1,"line_1 filling");

  char* line_2 = LRUBuf_get(DEFAULT_LRUBufId);
  TEST_NOT_NULL_MSG(line_2,"line_2 not null");
  strcpy(line_2,"une autre information");
  TEST_EQU_STR_MSG("une autre information",line_2,"line_2 filling");


  TEST_EQU_INT_MSG(LRUBuf_get_nlines(DEFAULT_LRUBufId),1000,"nb de lignes dans LRUBuf static");
  TEST_EQU_INT_MSG(LRUBuf_get_lineSize(DEFAULT_LRUBufId),1000,"taille des lignes dans LRUBuf static");

  LRUBuf_clean(DEFAULT_LRUBufId);
  TEST_EQU_STR_MSG("",line_1,"line_1 empty");
  TEST_EQU_STR_MSG("",line_2,"line_2 empty");
  return;
}



///////////////////////////////////////////////////////////////////////////
// que teste on : les fonctions de gestion des buffers LRU alloués amiquement
///////////////////////////////////////////////////////////////////////////
void test_LRUBuf_dyn(void)
{
  {
    LRUBufId idLRU = LRUBuf_init(10, 20);

    char* line_1 = LRUBuf_get(idLRU);
    TEST_NOT_NULL_MSG(line_1, "line_1 not null");
    strcpy(line_1, "une information");
    TEST_EQU_STR_MSG("une information", line_1, "line_1 filling");

    char* line_2 = LRUBuf_get(idLRU);
    TEST_NOT_NULL_MSG(line_2, "line_2 not null");
    strcpy(line_2, "une autre information");
    TEST_EQU_STR_MSG("une autre information", line_2, "line_2 filling");

    TEST_EQU_INT_MSG(10, LRUBuf_get_nlines(idLRU), "nb de lignes dans LRUBuf static");
    TEST_EQU_INT_MSG(20, LRUBuf_get_lineSize(idLRU), "taille des lignes dans LRUBuf static");

    LRUBuf_clean(idLRU);
    TEST_EQU_STR_MSG("", line_1, "line_1 empty");
    TEST_EQU_STR_MSG("", line_2, "line_2 empty");
    LRUBuf_free(idLRU);
  }

  {
    LRUBufId idLRU = LRUBuf_init(10, 20);
    char* line_1 = LRUBuf_get(idLRU);
    TEST_NOT_NULL_MSG(line_1, "line_1 not null");
    strcpy(line_1, "une information");
    TEST_EQU_STR_MSG("une information", line_1, "line_1 filling");
    LRUBuf_free(idLRU);
  }
  {
    // doit générer un signal SIGUSR1
    LRUBufId idLRU = LRUBuf_init(10, 20);
  }
  LRUBuf_free(DEFAULT_LRUBufId);
  return;
}



/************* Lancement des Tests **************************************/


void test()
{
  RUN(test_LRUBuf);
  RUN(test_LRUBuf_dyn);
}


