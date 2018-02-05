#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "ISeq.h"

// Linear space of memory adresses
// management based on ISeq structure: indexed sequence of adresses

typedef ISeq MSP;   // memory space: set of adresses
typedef any_t* Adr;   // Adress: pointer to any type


MSP* MSP_new(void);
Adr MSP_malloc(MSP* msp, size_t sz);
void MSP_free(MSP* msp, Adr adr);
void MSP_freeAll(MSP* msp, Adr adr);
void MSP_destroy(MSP** msp);


#endif // MEM_H_INCLUDED
