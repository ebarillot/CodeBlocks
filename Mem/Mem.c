#include "Mem.h"



///////////////////////////////////////////////////////////////////////////////
MSP* MSP_new(void)
{
  MSP* msp = (MSP*)ISeq_new();
  return msp;
}


///////////////////////////////////////////////////////////////////////////////
Adr MSP_malloc(MSP* msp, size_t sz)
{
  msp = ISeq_app(msp, calloc(1,sz));
  return adr;
}


///////////////////////////////////////////////////////////////////////////////
void MSP_free(MSP* msp, Adr adr)
{
}


///////////////////////////////////////////////////////////////////////////////
void MSP_freeAll(MSP* msp, Adr adr)
{
}


///////////////////////////////////////////////////////////////////////////////
void MSP_destroy(MSP** msp)
{
}
