/**
* Prog de test pour Pmap
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Pmap.h"

#include "Erreur.h"
#include "inout.h"
#include "Message.h"


#define KEY_MAX_LENGTH    L_Pmap_name
#define KEY_PREFIX        ("somekey")
#define KEY_COUNT         (1024*32)
//#define KEY_COUNT (1024*1024)

typedef struct data_struct_s
{
  char key_string[KEY_MAX_LENGTH];
  int number;
} data_struct_t;

/////////////////////////////////////////////////////////////////////
int main (char* argv, int argc)
{
  int ret = 0;
  int index;
  Pmap* pmap;
  Pmaptype pmaptype;
  data_struct_t* value;

  ret = Pmap_new (&pmap, -1);

  char key_string[KEY_MAX_LENGTH];

  // First, populate the hash map with ascending values
  printf("[%s %s] -- debut creation\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  for (index=0; index<KEY_COUNT; index+=1)
  {
    /* Store the key string along side the numerical value so we can free it later */
    value = malloc(sizeof(data_struct_t));
    snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    value->number = index;
    Pmapval pv;
    Pmapval_to_voidp(pv,value);
    //pv.p = (void*)value;
    ret = Pmap_put(pmap, value->key_string, Pmap_voidp, pv, NULL, free);
    //ret = Pmap_put(pmap, value->key_string, Prop_voidp, (Propval)(void*)value, NULL);
    assert(ret==0);
  }
  printf("[%s %s] -- fin creation\n", Utile_getAnneeMoisJour(), Utile_getHeure());

  // Now, check all of the expected values are there
  printf("[%s %s] -- debut lecture\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  for (index=0; index<KEY_COUNT; index+=1)
  {
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    Pmapval pv;
    ret = Pmap_getValByName(pmap, key_string, &pmaptype, &pv, NULL);
    Pmapval_from_voidp (pv,value);
    //ret = Pmap_getValByName(pmap, key_string, &proptype, (Propval*)(void**)value, NULL);
    // Make sure the value was both found and the correct number
    assert(ret==0);
    assert(value->number==index);
  }
  printf("[%s %s] -- fin lecture\n", Utile_getAnneeMoisJour(), Utile_getHeure());

  // Make sure that a value that wasn't in the map can't be found
  printf("[%s %s] -- debut test valeur absente\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);
  {
    Pmapval pv;
    ret = Pmap_getValByName (pmap, key_string, &pmaptype, &pv, NULL);
    Pmapval_from_voidp (pv,value);
    Message_print(LOG,"%s",Erreur_format());
  }
  printf("[%s %s] -- fin test valeur absente\n", Utile_getAnneeMoisJour(), Utile_getHeure());

  // test remplacement 1
  printf("[%s %s] -- debut test remplacement 1\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  {
    value = malloc(sizeof(data_struct_t));
    index = 0;
    snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    value->number = index;
    Pmapval pv;
    Pmapval_to_voidp(pv,value);
    ret = Pmap_put(pmap, value->key_string, Pmap_voidp, pv, NULL, free);
    assert(ret==0);
  }
  printf("[%s %s] -- fin test remplacement 1\n", Utile_getAnneeMoisJour(), Utile_getHeure());

  // test remplacement par un aitre type
  printf("[%s %s] -- debut test remplacement 2\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  {
    index = 1;
    unsigned long long value = (unsigned long long)index;
    char key_string[KEY_MAX_LENGTH];
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    Pmapval pv;
    Pmapval_to_ulonglong(pv,value);
    ret = Pmap_put(pmap, key_string, Pmap_ulonglong, pv, NULL, free);
    assert(ret==0);
  }
  printf("[%s %s] -- fin test remplacement 2\n", Utile_getAnneeMoisJour(), Utile_getHeure());

  // compteurs
  printf("-- %s -------\n",__func__);
  printf("table_size: %d\n", Pmap_getNbAlloc(pmap));
  printf("size: %d\n", Pmap_getNbUsed(pmap));

  #if 1
  // Free all of the values we allocated and remove them from the map
  printf("[%s %s] -- debut test remove\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  for (index=0; index<KEY_COUNT; index+=1)
  {
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    Pmapval pv;
    ret = Pmap_getValByName (pmap, key_string, &pmaptype, &pv, NULL);
    Pmapval_from_voidp (pv,value);
    assert(ret==0);
    ret = Pmap_remove (pmap, key_string);
    assert(ret==0);
    // free(value); // pas besoin car desalloué par Pmap_remove()
                    // car lors du Pmap_put() on a spécifié une fonction
                    // de desallocation
  }
  printf("[%s %s] -- fin test remove\n", Utile_getAnneeMoisJour(), Utile_getHeure());
  #endif
  // Now, destroy the Pmap
  Pmap_free (&pmap);
  exit(0);
}

