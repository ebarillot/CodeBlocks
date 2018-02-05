#ifndef Erreur_H
#define Erreur_H

#include <stdlib.h> // pour size_t

/************************************************************************/
/* Constantes								*/
/************************************************************************/

#define Default_lastErreur    (size_t)(-1)  // pour indiquer qu'on cherche la dernière

// classes d'erreur
#define CLA_APP 900
#define CLA_SS  901
#define CLA_ORA 902
#define CLA_NP  903
#define CLA_TUX 904
#define CLA_INF 905
#define CLA_XML 906
#define CLA_EDI 907
#define CLA_SYS 908
#define CLA_VIDE 0

#define LIB_CLA_APP     "applicative"
#define LIB_CLA_SS      "Search Server"
#define LIB_CLA_ORA     "Oracle"
#define LIB_CLA_NP      "NewPort"
#define LIB_CLA_TUX     "Tuxedo"
#define LIB_CLA_INF     "Informa"
#define LIB_CLA_XML     "Parsing XML"
#define LIB_CLA_EDI     "encodage/decodage EDI"
#define LIB_CLA_SYS     "Systeme"
#define LIB_CLA_VIDE    "Aucune erreur"

//************************************************************************
// Types
//************************************************************************

typedef size_t ErreurId;


#define L_fonction        100
#define L_messageErreur   500	// dangereux sans recompilation des progs et libs
//#define L_messageErreur   2500
typedef struct _Erreur {
  int   classe;
  char  fonction[L_fonction+1];
  char  message[L_messageErreur+1];
} Erreur;


#ifndef __ERREUR_C__
// la variable n'est déclarée externe que en dehors de Erreur.c
// elle permet à un programme qui est interrompu par le module Erreur (kill SIGUSR1) de savoir
// que c'est bien ce module qui a un pb de place pour stocker les erreurs.
extern int  __Erreur_for_SIGUSR1_flag__ ;
extern char __Erreur_for_SIGUSR1_msg__[] ;
#endif

//************************************************************************
// fonctions
//************************************************************************
int       Erreur_cleanAll (void);
int       Erreur_freeAll (void);
int       Erreur_getClasse (ErreurId id);
char*     Erreur_getLibClasse (int classe);
char*     Erreur_getFonction (ErreurId id);
char*     Erreur_getMessage (ErreurId id);
// retourne le nb d'erreurs en stock
size_t    Erreur_getNbAll (void);
// retourne l'adresse mémoire d'une erreur à partir de son id
Erreur*   Erreur_get (ErreurId id);
// retourne un pointeur vers la dernière erreur
Erreur*   Erreur_getLast (void);
// retourne l'id de la dernière erreur
ErreurId  Erreur_getLastId (void);
// Message à partir de l'erreur courante
char*     Erreur_format (void);
// Message à partir d'une erreur passée en paramètre
char*     Erreur_format2 (Erreur* erreur);
// Message à partir d'un id d'erreur passé en paramètre
char*     Erreur_format3 (ErreurId id);
// positionne une erreur dans le buffer
size_t    Erreur_set (int classe, char* fonction,  char* fmt, ...);

// capture et gestion d'erreur, bloque les appels recursifs de traitement d'erreur
int catchR (int (*handler)(int, void*), int ret, void* gen_arg);


// Ecriture des messages sur la sortie standard en cas d'erreur critique
void Erreur_criticalPrint (void);

#endif
