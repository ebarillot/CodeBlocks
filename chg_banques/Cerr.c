#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>	/* pour gérer les va_list */
#include <string.h>

#include <exploit.h>	/* pour CODE_FIN_ERREUR_APPLICATIVE */

#include "Cerr.h"
#include "utile.h"
#include "mallocCtl.h"

/* structure erreur connue comme variable externe de tous les modules
 * qui interragissent avec celui ci
 * permet de traiter de façon sure (pas de pb d'allocation) une erreur
 * à la fois */
static Cerr currentError;

/* la liste commune des erreurs */
static List* lsterr;

/* une varaible locale interne pour parcourir les erreurs */
static List* itererr;


/************************************************************************/
/* constructeur								*/
/************************************************************************/
Cerr*	newError (void)
{
  char	nomFonction[] = "newError";
  Cerr* lerreur = NULL;
  if (!(lerreur = mallocCtl(sizeof(Cerr),"lerreur",nomFonction))) {
    currentError.classe = CLA_APP;
    strcpy (currentError.fonction, nomFonction);
    /* sprintf (currentError.message,"Probleme d'allocation memoire: %s",getMemerror()); */
  }
  else {
    memset (lerreur, 0, sizeof (Cerr));
  }
  return lerreur;
}

/************************************************************************/
/* Detruit un objet erreur						*/
/************************************************************************/
int freeError(Cerr* err)
{
  char nomFonction[] = "freeError";
  int ret = 0;
  freeCtl((void*)err,nomFonction);
  return ret;
}

/************************************************************************/
/* retourne le libellé associé à la classe d'erreur                     */
/************************************************************************/
char* getLibClassError (int cerr)
{
  switch (cerr) {
  case CLA_SYS : return LIB_CLA_SYS;
  case CLA_MEM : return LIB_CLA_MEM;
  case CLA_APP : return LIB_CLA_APP;
  case CLA_SS  : return LIB_CLA_SS;
  case CLA_ORA : return LIB_CLA_ORA;
  case CLA_NP  : return LIB_CLA_NP;
  case CLA_TUX : return LIB_CLA_TUX;
  case CLA_PROV : return LIB_CLA_PROV;
  case CLA_XML : return LIB_CLA_XML;
  case CLA_EDI : return LIB_CLA_EDI;
  case CLA_MDW : return LIB_CLA_MDW;
  case CLA_FTP : return LIB_CLA_FTP;
  case CLA_BUS : return LIB_CLA_BUS;
  default : return LIB_CLA_VIDE;
  }
}

/************************************************************************/
/* Retourne 1 si erreur de type métier					*/
/************************************************************************/
int   isBusinessClassError (Cerr* err)
{
  return (err->classe==CLA_BUS);
}

/************************************************************************/
/* Retourne 1 si il existe au moins une erreur de l'un des types donnés	*/
/************************************************************************/
int   existsClassError (int* classError)
{
  char nomFonction[] = "existsClassError";
  int	ret = 0;
  int*	pi = NULL;
  Cerr* err = NULL;
  if (!pi) {
    ret = 0;
    goto XIT;
  }
  err = initIterError();
  while (err) {
    pi = classError;
    while (pi && *pi) {
      if (err->classe == *pi) {
	ret = 1;
	break;
      }
      pi++;
    }
    err = getNextIterError();
  }
 XIT:
  return ret;
}

/************************************************************************/
/* Affiche erreur							*/
/************************************************************************/
char*	toStringError (Cerr* argErreur)
{
  static char messageErreur[L_fonction+L_messageErreur+1];
  if (!argErreur) {
    sprintf (messageErreur, "No error");
  }
  else {
    sprintf (messageErreur, "(classe=%s) (fonction=%s) (message=%s)",
	     getLibClassError (argErreur->classe),
	     argErreur->fonction,
	     argErreur->message);
  }
  return messageErreur;
}

/************************************************************************/
/* Remplissage et ajout à la liste commune des erreurs			*/
/* de l'erreur courante							*/
/************************************************************************/
Cerr* keepError(int   classe,
		char* fonction,
		char* fmt, ...)
{
  char avancement[L_afficheMessage+1];
  va_list ap;
  va_start (ap, fmt);
  vsprintf (avancement, fmt, ap);
  fillCurrentError(classe, fonction, avancement);
  va_end (ap);
  return addCurrentErrorToList(&lsterr);
}

/************************************************************************/
/* Retourne la dernière erreur enregistrée				*/
/************************************************************************/
Cerr* getCurrentError(void)
{
  char	nomFonction[] = "getCurrentError";
  return &currentError;
}

/************************************************************************/
/* initialise l'iterateur pour le parcours des erreurs			*/
/************************************************************************/
Cerr* initIterError(void)
{
  char	nomFonction[] = "initIterError";
  Cerr* err = NULL;
  itererr = lsterr;
  if (itererr) {
    err = (Cerr*) getObjList(itererr);
  }
  return err;
}

/************************************************************************/
/* initialise l'iterateur pour le parcours des erreurs			*/
/************************************************************************/
Cerr* getNextIterError(void)
{
  char	nomFonction[] = "getNextIterError";
  Cerr* err = NULL;
  itererr = getNextList(itererr);
  if (itererr) {
    err = (Cerr*) getObjList(itererr);
  }
  return err;
}

/************************************************************************/
/* Remplissage de l'erreur courante					*/
/************************************************************************/
Cerr* fillCurrentError(int   classe,
		       char* fonction,
		       char* fmt, ...)
{
  char	nomFonction[] = "fillCurrentError";
  currentError.classe = classe;
  strcpy (currentError.fonction, fonction);
  {
    va_list ap;
    va_start (ap, fmt);
    vsprintf (currentError.message, fmt, ap);
    va_end (ap);
  }
 XIT:
  return &currentError;
}

/************************************************************************/
/* retourne 1 si l'erreur courante existe, 0 sinon			*/
/************************************************************************/
int existsCurrentError(void)
{
  if (currentError.classe) {
    return 1;
  }
  else {
    return 0;
  }
}

/************************************************************************/
/* efface la derniere erreur						*/
/************************************************************************/
void clearCurrentError(void)
{
  memset (&currentError, 0, sizeof currentError);
}

/************************************************************************/
/* retourne la dernière erreur de la liste				*/
/************************************************************************/
Cerr* getLastError(void)
{
  List* last = NULL;
  Cerr* err = NULL;
  /* retourne le dernier élément de la liste */
  last = getLastList(lsterr);
  if (last) {
    err = (Cerr*)getObjList(last);
    /* si pas d'élément dans la liste, on retourne l'erreur courante
     * pour ne pas retourner un pointeur NULL */
  }
  if (!err) err = &currentError;
  return err;
}

/************************************************************************/
/* retourne 1 si il existe au moins une erreur, 0 sinon			*/
/************************************************************************/
int existsErrors(void)
{
  if (lsterr) {
    return 1;
  }
  else {
    return 0;
  }
}

/************************************************************************/
/* Affichage de la liste des erreurs					*/
/************************************************************************/
void printErrors(void)
{
  if (existsErrors()) {
    printErrorsInList(lsterr);
  }
  if (existsCurrentError()) {
    afficheMessage(0,WARN,"Current error : %s", toStringError(&currentError));
  }
}

/************************************************************************/
/* Detruit la liste des erreurs						*/
/* et l'erreur courante							*/
/************************************************************************/
void removeErrors(void)
{
  if (existsErrors()) {
    removeErrorList(lsterr);
    lsterr = NULL;
  }
  clearCurrentError();
}

/************************************************************************/
/* Ajout de l'erreur courante à la liste				*/
/************************************************************************/
Cerr* addCurrentErrorToList(List** list)
{
  char	nomFonction[] = "addCurrentErrorToList";
  Cerr* lerreur = NULL;
  if (!(lerreur = newError())) goto XIT;
  *lerreur = currentError;
  /* pour pouvoir intercepter des erreurs occasionnées
     par les opérations suivantes et ne pas confondre l'erreur qu'on veut
     stocker avec une erreur qui peut se produire en la stockant */
  clearCurrentError();
  lerreur = addObjErrorToList(list, lerreur);
 XIT:
  return lerreur;
}

/************************************************************************/
/* Ajout d'une erreur à la liste					*/
/************************************************************************/
Cerr* addObjErrorToList(List** list, Cerr* err)
{
  char	nomFonction[] = "addObjErrorToList";
  *list = addElementList(*list, (void*)err);
  if (existsCurrentError()) {
    err = NULL;
    printErrors();
    afficheMessage(0,ERR,"Impossible de gérer une liste des erreurs, ARRET");
    exit (-1);
  }
 XIT:
  return err;
}

/************************************************************************/
/* Affichage de la liste des erreurs					*/
/************************************************************************/
void printErrorsInList(List* list)
{
  int ierr = 0;
  Cerr* err = NULL;
  for (;list;) {
    ierr++;
    err = (Cerr*) getObjList(list);
    afficheMessage(0,WARN,"Error %d %s", ierr, toStringError(err));
    list = getNextList(list);
  }
}

/************************************************************************/
/* Detruit la liste des erreurs						*/
/************************************************************************/
void removeErrorList(List* list)
{
  removeList(list, freeError);
  list = NULL;
  return;
}

