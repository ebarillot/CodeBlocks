/*
* A unit test and example of how to use the simple C hashmap
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Hmap.h"

#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*32)
//#define KEY_COUNT (1024*1024)
typedef struct data_struct_s
{
  char key_string[KEY_MAX_LENGTH];
  int number;
} data_struct_t;

int main (char* argv, int argc)
{
  int index;
  int error;
  map_t mymap;
  char key_string[KEY_MAX_LENGTH];
  data_struct_t* value;
  mymap = Hmap_new();
  /* First, populate the hash map with ascending values */
  for (index=0; index<KEY_COUNT; index+=1)
  {
    /* Store the key string along side the numerical value so we can free it later */
    value = malloc(sizeof(data_struct_t));
    snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    value->number = index;
    error = Hmap_put(mymap, value->key_string, value);
    assert(error==MAP_OK);
  }
  /* Now, check all of the expected values are there */
  for (index=0; index<KEY_COUNT; index+=1)
  {
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    error = Hmap_get(mymap, key_string, (void**)(&value));
    /* Make sure the value was both found and the correct number */
    assert(error==MAP_OK);
    assert(value->number==index);
  }
  /* Make sure that a value that wasn't in the map can't be found */
  snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);
  error = Hmap_get(mymap, key_string, (void**)(&value));
  /* Make sure the value was not found */
  assert(error==MAP_MISSING);
  printf("-- %s -------\n",__func__);
  printf("table_size: %d\n", Hmap_total_length(mymap));
  printf("size: %d\n", Hmap_length(mymap));
  /* Free all of the values we allocated and remove them from the map */
  for (index=0; index<KEY_COUNT; index+=1)
  {
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
    error = Hmap_get(mymap, key_string, (void**)(&value));
    assert(error==MAP_OK);
    error = Hmap_remove(mymap, key_string);
    assert(error==MAP_OK);
    free(value);
  }
  /* Now, destroy the map */
  Hmap_free(mymap);
  exit(0);
}
