#ifndef	list_H
#define	list_H

struct List {
  void*		obj;
  struct List*	next;
};
typedef struct List List;


/* constructeur	*/
List*	newList(void);
/* ajout d'un élément dans la liste avec allocation de l'element */
List* addElementList(List* list, void* obj);
/* ajout d'un objet dans un élément de la liste */
int setObjList(List* list, void* elt);
/* retourne l'objet */
void* getObjList(List* list);
/* retourne l'élément suivant */
List* getNextList(List* list);
/* retourne le dernier élément de la liste */
List* getLastList(List* list);
/* effacement du dernier élément de la liste */
List* removeLastList(List* list, int (*freeObj)());
/* effacement du 1er élément de la liste */
List* removeFirstList(List* list, int (*freeObj)());
/* effacement de la list et des objets de la liste */
List* removeList(List* list, int (*freeObj)());

#endif
