#ifndef Cerr_H
#define Cerr_H

#include "List.h"
#include "exploit.h"

/************************************************************************/
/* Constantes								*/
/************************************************************************/
#define NO_MORE_DATA	-1

/* classes d'erreur */
#define CLA_SYS 900
#define CLA_MEM 901
#define CLA_APP 902
#define CLA_SS  903
#define CLA_ORA 904
#define CLA_NP  905
#define CLA_TUX 906
#define CLA_PROV 907
#define CLA_XML 908
#define CLA_EDI 909
#define CLA_MDW 910
#define CLA_FTP 911
#define CLA_BUS 990
#define CLA_VIDE 999

#define LIB_CLA_SYS     "System"
#define LIB_CLA_MEM     "Memory"
#define LIB_CLA_APP     "Application"
#define LIB_CLA_SS      "Search Server"
#define LIB_CLA_ORA     "Oracle"
#define LIB_CLA_NP      "NewPort"
#define LIB_CLA_TUX     "Tuxedo"
#define LIB_CLA_PROV    "Provider"
#define LIB_CLA_XML     "XML Parsing"
#define LIB_CLA_EDI     "EDI Encoding/decoding"
#define LIB_CLA_MDW     "Middleware"
#define LIB_CLA_FTP     "Ftp"
#define LIB_CLA_BUS     "Business"
#define LIB_CLA_VIDE    "Unknow error class"

/************************************************************************/
/* Types								*/
/************************************************************************/

#define	L_fonction	500
#define	L_messageErreur	5000
typedef struct {
  int	classe;
  char	fonction[L_fonction+1];
  char	message[L_messageErreur+1];
} Cerr;


/************************************************************************/
/* fonctions								*/
/************************************************************************/
Cerr* newErreur (void);
int   freeError(Cerr* err);
char* getLibClassError (int classeErreur);
int   isBusinessClassError (Cerr* err);
int   existsClassError (int* classError);
char* toStringError (Cerr* argErreur);
Cerr* keepError(int   classe,
		char* fonction,
		char* fmt, ...);
/* remplisage de l'erreur courante */
Cerr* getCurrentError(void);
Cerr* initIterError(void);
Cerr* getNextIterError(void);
Cerr* fillCurrentError(int   classe,
		       char* fonction,
		       char* fmt, ...);
int   existsCurrentError(void);
void  clearCurrentError(void);

/* fonctions pour la liste commune des erreurs */
Cerr* getLastError(void);
int   existsErrors(void);
void  printErrors(void);
void  removeErrors(void);

/* fonctions pour une liste d'erreurs */
Cerr* addCurrentErrorToList(List** list);
Cerr* addObjErrorToList(List** list, Cerr* err);
void printErrorsInList(List* list);
void removeErrorList(List* list);

#endif
