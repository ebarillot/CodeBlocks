/*
///////////////////////////////////////////////////////////////////////////
// 2017/01 E.Barillot
//
///////////////////////////////////////////////////////////////////////////
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "test_unit.h"

#include "ABS.h"
#include "Erreur.h"
#include "Message.h"

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

///////////////////////////////////////////////////////////////////////////
// fonction pour afficher les valeurs not ABS_None
void _ABS_printf(ABS* abs)
{
  size_t i;
  char* elem;
  for (i=0; i<ABS_len(abs); i++) {
    elem = (char*)ABS_get_elem(abs,i);
    if (elem != ABS_None) printf("[%u] %s\n",i,elem);
  }
}


/************* Definition des Tests **************************************/


///////////////////////////////////////////////////////////////////////////
// que teste on : les fonctions de gestion d'une ABS
///////////////////////////////////////////////////////////////////////////
void test_ABS_new(void)
{
  ABS* abs = ABS_new();
  TEST_NOT_NULL_MSG(abs,"abs not null");
  size_t ind = 0;
  TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem at %d is None",ind);
  ind = (ABS_INIT_SZ-1);
  TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem at %d is None", ind);
  ind = (ABS_INIT_SZ);
  TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem at %d is None", ind);

  TEST_EQU_INT_MSG(0,ABS_len(abs),"len");
  TEST_EQU_INT_MSG(0,ABS_nb_not_None(abs),"not None");

  return;
}


///////////////////////////////////////////////////////////////////////////
// que teste on : l'extension automatique d'une ABS et sa réduction automatique
///////////////////////////////////////////////////////////////////////////
void test_ABS_extend(void)
{
  ABS* abs = ABS_new();
  TEST_NOT_NULL_MSG(abs,"abs not null");

  int iext;
  printf("--> extensions successives\n");
  for (iext=0;iext<5;iext++) {
    size_t sz_prec = ABS_sz(abs);
    size_t ind     = sz_prec+20;
    printf("ind créé = %u\n",ind);
    abs = ABS_set(abs,ind,"une valeur");
    TEST_EQU_INT_MSG(ind+1,ABS_len(abs),"len");
    TEST_EQU_INT_MSG(iext+1,ABS_nb_not_None(abs),"not None");
    TEST_EQU_INT_MSG(sz_prec*2,ABS_sz(abs),"sz");
    printf("sz = %u\n",ABS_sz(abs));
  }
  _ABS_printf(abs);

  printf("--> suppression sans reduction\n");
  {
    size_t sz_prec = ABS_sz(abs);
    size_t ind = ABS_INIT_SZ+20;
    abs = ABS_rm(abs,ind);
    TEST_EQU_INT_MSG(sz_prec,ABS_sz(abs),"sz");
    TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem supprimé %u",ind);
    _ABS_printf(abs);
  }

  printf("--> suppression avec réduction\n");
  {
    size_t sz_prec = ABS_sz(abs);
    size_t ind = ABS_INIT_SZ*(2<<3)+20;   // 1024*2^3 +20
    printf("ind=%u\n",ind);
    abs = ABS_rm(abs,ind);
    TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem supprimé %u",ind);
    //
    ind = ABS_INIT_SZ*(2<<2)+20;
    printf("ind=%u\n",ind);
    abs = ABS_rm(abs,ind);
    TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem supprimé %u",ind);
    //
    ind = ABS_INIT_SZ*(2<<1)+20;
    printf("ind=%u\n",ind);
    abs = ABS_rm(abs,ind);
    TEST_EQU_STR_MSG(ABS_None,ABS_get_elem(abs,ind),"elem supprimé %u",ind);
    //
    TEST_EQU_INT_MSG(ABS_INIT_SZ*(2<<4)/4,ABS_sz(abs),"sz");
    _ABS_printf(abs);
  }
  return;
}


///////////////////////////////////////////////////////////////////////////
// que teste on : l'extension par append
///////////////////////////////////////////////////////////////////////////
void test_ABS_append(void)
{
  ABS* abs = ABS_new();
  TEST_NOT_NULL_MSG(abs,"abs not null");
  // ajout d'une valeur au bout de l'ABS
  size_t ind = ABS_INIT_SZ-1;
  printf("--> ind créé = %u\n",ind);
  abs = ABS_set(abs,ind,"une valeur");
  TEST_EQU_INT_MSG(ind+1,ABS_len(abs),"len");
  TEST_EQU_INT_MSG(1,ABS_nb_not_None(abs),"not None");
  TEST_EQU_INT_MSG(ABS_INIT_SZ,abs->sz,"sz");

  // append d'une valeur
  // => doit déclencher un _resize
  printf("--> append\n");
  abs = ABS_app(abs,"une valeur");
  TEST_EQU_INT_MSG(ABS_INIT_SZ+1,ABS_len(abs),"len");
  TEST_EQU_INT_MSG(2,ABS_nb_not_None(abs),"not None");
  TEST_EQU_INT_MSG(ABS_INIT_SZ*2,abs->sz,"sz");

  _ABS_printf(abs);
  return;
}


/************* Lancement des Tests **************************************/


void test()
{
  RUN(test_ABS_new);
  RUN(test_ABS_extend);
  RUN(test_ABS_append);
}


