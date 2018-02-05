
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <exploit.h>	/* pour CODE_FIN_ERREUR_APPLICATIVE */

#include "List.h"
#include "Cerr.h" /* pour fillCurrentError() */
#include "mallocCtl.h"


/************************************************************************/
/* fonctions privées							*/
/************************************************************************/
static List*	allocList(void);

/************************************************************************/
/* variables globales							*/
/************************************************************************/

/************************************************************************/
/* constructeur								*/
/************************************************************************/
List*	newList(void)
{
  char	nomFonction[] = "newList";
  /* ne fait rien les elements sont alloués par addElementList
   * pour usage ultérieur */
  return NULL;
}

/************************************************************************/
/* constructeur								*/
/************************************************************************/
static List*	allocList(void)
{
  char	nomFonction[] = "allocList";
  List* liste = NULL;
  liste = mallocCtl(sizeof(List),"liste",nomFonction);
  return liste;
}

/************************************************************************/
/* ajout d'un élément dans la liste avec allocation de l'element	*/
/************************************************************************/
List* addElementList(List* liste, void* obj)
{
  char	nomFonction[] = "addElementList";
  List* last = NULL;
  List* newElt = NULL;
  newElt = allocList();
  if (!liste) {
    liste = newElt;
  }
  else {
    last = getLastList(liste);
    last->next = newElt;
  }
  newElt->obj = obj;
 XIT:
  return liste;
}

/************************************************************************/
/* ajout d'un objet dans un élément de la liste				*/
/************************************************************************/
int setObjList(List* liste, void* elt)
{
  int	ret = 0;
  char	nomFonction[] = "setObjList";
  if (liste == NULL) {
    fillCurrentError(CLA_APP, nomFonction, "List null");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (elt == NULL) {
    fillCurrentError(CLA_APP, nomFonction, "Element null");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  liste->obj = elt;
 XIT:
  return ret;
}

/************************************************************************/
/* retourne l'objet							*/
/************************************************************************/
void* getObjList(List* liste)
{
  char	nomFonction[] = "getObjList";
  if (liste == NULL) {
    fillCurrentError(CLA_APP, nomFonction, "List null");
    return NULL;
  }
  return liste->obj;
}

/************************************************************************/
/* retourne l'élément suivant						*/
/************************************************************************/
List* getNextList(List* liste)
{
  char	nomFonction[] = "getNextList";
  if (liste == NULL) {
    fillCurrentError(CLA_APP, nomFonction, "List null");
    return NULL;
  }
  return liste->next;
}

/************************************************************************/
/* retour du dernier élément de la liste				*/
/************************************************************************/
List* getLastList(List* liste)
{
  char	nomFonction[] = "getLastList";
  List* last = liste;
  if (liste == NULL) return NULL;
  while (last->next) {
    last = last->next;
  }
  return last;
}

/************************************************************************/
/* effacement du dernier élément de la liste				*/
/************************************************************************/
List* removeLastList(List* liste, int (*freeObj)())
{
  char	nomFonction[] = "removeLastList";
  List* last = liste;
  List* prev = NULL;
  if (liste == NULL) return NULL;
  while (last->next) {
    prev = last;
    last = last->next;
  }
  if (last) {
    if (liste == last) liste = NULL;
    freeObj(last->obj);
    freeCtl((void*)last,nomFonction);
    last = NULL;
    if (prev->next) prev->next = NULL;
  }
  return liste;
}

/************************************************************************/
/* effacement du 1er élément de la liste				*/
/************************************************************************/
List* removeFirstList(List* liste, int (*freeObj)())
{
  char	nomFonction[] = "removeFirstList";
  List* first = liste;
  List* next = NULL;
  if (first) {
    freeObj(first->obj);
    next = first->next;
    freeCtl((void*)first,nomFonction);
    first = next;
  }
  return first;
}

/************************************************************************/
/* effacement de la liste						*/
/************************************************************************/
List* removeList(List* liste, int (*freeObj)())
{
  char	nomFonction[] = "removeList";
  while (liste) {
    liste = removeFirstList(liste,freeObj);
  }
  return liste;
}
