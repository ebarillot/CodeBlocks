#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "ABS.h"

// Linear space of memory adresses (MSP)
// management based on ABS structure: array based sequence of adresses

typedef ABS MSP;     // memory space: set of adresses
typedef any_t* Adr;   // Adress: pointer to any type
typedef enum Adr_status Adr_status;
enum Adr_status {ADR_UNDEF = -1};


// cr�ation d'un MSP
MSP* MSP_new(void);
// obtention d'une zone m�moire allou�e dans le MSP donn�
Adr MSP_malloc(MSP* msp, size_t sz);
// lib�ration de la zone m�moire allou�e dans le MSP donn�
void MSP_free(MSP* msp, Adr adr);
// lib�ration de toutes les zones m�moire allou�es dans le MSP donn�
void MSP_freeAll(MSP* msp, Adr adr);
// destruction du MSP ainsi que lib�ration de toutes les zones m�moire allou�es dans le MSP donn�
void MSP_destroy(MSP** msp);
// retourne l'indice associ�s � l'adresse dans le MSP
size_t MSP_get_IndFromAdr(MSP* msp, Adr adr);


#endif // MEM_H_INCLUDED
