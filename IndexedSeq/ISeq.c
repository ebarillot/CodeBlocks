

#include <stdio.h>
#include <string.h>


#include "ISeq.h"




///////////////////////////////////////////////////////////////////////////////
// TODO pas besoin de reallouer l'enveloppe du conteneur, juste le conteneur d'elements
static ISeq* _resize(ISeq* iseq, size_t sz)
{
  // nouveau conteneur
  ISeq* iseq_new;
  iseq_new = (ISeq*)calloc(1,sizeof(ISeq));
  if (iseq_new == NULL) fprintf(stdout,"ISeq_resize calloc 1 failed");
  iseq_new->sz  = sz;
  iseq_new->ind = calloc(sz,sizeof(any_t));
  if (iseq_new->ind == NULL) fprintf(stdout,"ISeq_resize calloc 2 failed");
  // recopie des elem de l'ancien conteneur vers le nouveau s'il existe
  if (iseq != NULL) {
    int i;
    for (i=0;i<iseq->sz;i++) {
      iseq_new->ind = iseq->ind;
    }
    iseq_new->used = iseq->sz;
    // free de l'ancien conteneur
    ISeq_free(&iseq);
  }
  return iseq_new;
}


///////////////////////////////////////////////////////////////////////////////
ISeq* ISeq_new(void)
{
  ISeq* iseq = _resize(NULL,ISeq_INIT_SZ);
  return iseq;
}


///////////////////////////////////////////////////////////////////////////////
void ISeq_free(ISeq** iseq)
{
  if ((*iseq) != NULL && (*iseq)->ind != NULL) free ((*iseq)->ind);
  if ((*iseq) != NULL) free (*iseq);
  *iseq = NULL;
}


///////////////////////////////////////////////////////////////////////////////
void ISeq_deepfree(ISeq** iseq)
{
  int i;
  for (i=0;i<(*iseq)->sz;i++) {
    if ((*iseq)->ind != NULL) free ((*iseq)->ind);
  }
  ISeq_free(iseq);
}


///////////////////////////////////////////////////////////////////////////////
ISeq* ISeq_app(ISeq* iseq, any_t elem)
{
  ISeq* iseq_new;
  if (iseq->used == iseq->sz) iseq_new = _resize(iseq,iseq->sz*2);
  else iseq_new = iseq;
  iseq_new->ind[iseq->used] = elem;
  iseq_new->used++;
  return iseq_new;
}


///////////////////////////////////////////////////////////////////////////////
ISeq* ISeq_put(ISeq* iseq, size_t ind, any_t elem)
{
  ISeq* iseq_new = iseq;
  while (iseq->sz <= ind) iseq_new = _resize(iseq_new,iseq_new->sz*2);
  iseq_new->ind[ind] = elem;
  iseq_new->used++;
  return iseq_new;
}


///////////////////////////////////////////////////////////////////////////////
ISeq* ISeq_rm(ISeq* iseq, size_t ind)
{
  ISeq* iseq_new;
  if ((iseq->used-1) <= iseq->sz/4) iseq_new = _resize(iseq,iseq->sz/2);
  else iseq_new = iseq;
  if (iseq_new->ind[ind]) free(iseq_new->ind[ind]);
  iseq_new->ind[ind] = NULL;
  iseq_new->used--;
  return iseq_new;
}


///////////////////////////////////////////////////////////////////////////////
ISeq* ISeq_deepcopy(ISeq* iseq)
{
  ISeq* iseq_new = _resize(NULL,iseq->sz);
  int i;
  for (i=0;i<iseq->sz;i++) {
    if (iseq->ind != NULL) {
        size_t szelem = sizeof(*iseq->ind[i]);
        iseq_new->ind[i] = calloc(1,szelem);
        memcpy(iseq_new->ind[i], iseq->ind[i],szelem);
    }
  }
  return iseq_new;
}

