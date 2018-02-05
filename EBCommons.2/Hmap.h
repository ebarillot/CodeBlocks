/*
 * Generic hashmap manipulation functions
 */
#ifndef __HMAP_H__
#define __HMAP_H__

#define KEY_MAX_LENGTH (256)    // longueur max d'une clé


// valeurs de l'indicateur de présence de l'information : fichier et / ou bdd
typedef enum HmapRet HmapRet;
enum HmapRet
{
    HMAP_MISSING   = -3  /* No such element */
  , HMAP_FULL      = -2 	/* Hashmap is full */
  , HMAP_OMEM      = -1 	/* Out of Memory */
  , HMAP_OK        = 0 	/* OK */
};
// les libellés correspondants
static char* HmapRet_lib(HmapRet cod)
{
  switch (cod) {
    case HMAP_MISSING : return "HMAP_MISSING";
    case HMAP_FULL    : return "HMAP_FULL";
    case HMAP_OMEM    : return "HMAP_OMEM";
    case HMAP_OK      : return "HMAP_OK";
    default           : return "HMAP_UNDEF";
  }
}


/**
 * any_t is a pointer.  This allows you to put arbitrary structures in
 * the hashmap.
 */
typedef void *any_t;

/**
 * PFany1 is a pointer to a function that can take one any_t argument
 * and return an integer. Returns status code.
 */
typedef int (*PFany1)(any_t);

/**
 * PFany2 is a pointer to a function that can take two any_t arguments
 * and return an integer. Returns status code..
 */
typedef int (*PFany2)(any_t, any_t);

/**
 * map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented.  They see and manipulate only map_t's.
 */
typedef any_t map_t;

/**
 * Return an empty hashmap. Returns NULL if empty.
*/
extern map_t Hmap_new(void);

/**
 * Iteratively call f with argument (item, data) for
 * each element data in the hashmap. The function must
 * return a map status code. If it returns anything other
 * than MAP_OK the traversal is terminated. f must
 * not reenter any hashmap functions, or deadlock may arise.
 */
extern int Hmap_iterate1(map_t in, PFany1 f);
extern int Hmap_iterate2(map_t in, PFany2 f, any_t item);

/**
 * Add an element to the hashmap. Return MAP_OK or MAP_OMEM.
 */
extern int Hmap_put (map_t in, char* keystring, any_t value);

/**
 * Get an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int Hmap_get(map_t in, char* keystring, any_t *arg);

/**
 * Remove an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int Hmap_remove(map_t in, char* keystring);

/**
 * Get any element. Return MAP_OK or MAP_MISSING.
 * remove - should the element be removed from the hashmap
 */
extern int Hmap_get_one(map_t in, any_t *arg, int remove);

/**
 * Free the hashmap
 */
extern void Hmap_free(map_t in);

/**
 * Get the current size of a hashmap
 */
extern int Hmap_length(map_t in);
extern int Hmap_total_length(map_t in);

#endif

