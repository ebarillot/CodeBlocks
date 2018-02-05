#include "Mem.h"



///////////////////////////////////////////////////////////////////////////////
// cr�ation d'un MSP
MSP* MSP_new(void)
{
  MSP* msp = (MSP*)ABS_new();
  return msp;
}


///////////////////////////////////////////////////////////////////////////////
// obtention d'une zone m�moire allou�e dans le MSP donn�
Adr MSP_malloc(MSP* msp, size_t sz)
{
  Adr adr = calloc(1,sz);
  msp = ABS_app(msp, adr);
  return adr;
}


///////////////////////////////////////////////////////////////////////////////
// lib�ration de la zone m�moire allou�e dans le MSP donn�
void MSP_free(MSP* msp, Adr adr)
{
  if (adr != NULL) {
    size_t ind = MSP_get_IndFromAdr(msp, adr);
    ABS_rm(msp, ind);
    free (adr);
  }
  return;
}


///////////////////////////////////////////////////////////////////////////////
// lib�ration de toutes les zones m�moire allou�es dans le MSP donn�
void MSP_freeAll(MSP* msp, Adr adr)
{

}


///////////////////////////////////////////////////////////////////////////////
// destruction du MSP ainsi que lib�ration de toutes les zones m�moire allou�es dans le MSP donn�
void MSP_destroy(MSP** msp)
{
}

///////////////////////////////////////////////////////////////////////////////
// retourne l'indice associ�s � l'adresse dans le MSP
size_t MSP_get_IndFromAdr(MSP* msp, Adr adr)
{
  // TODO: impl�menter map, heap au dessus de ABS pour faciliter la recherche dans un MSP  ?
  size_t ind = ADR_UNDEF;
  size_t i;
  Adr adr_get;
  for (i=0; i<msp->used; i++) {
    adr_get = ABS_get_elem((ABS*) msp, i);
    if (adr_get == adr) {
      ind = i;
      break;
    }
  }
  return ind;
}
