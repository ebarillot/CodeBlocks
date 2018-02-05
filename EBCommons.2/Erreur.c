
#include <signal.h>
#include <unistd.h>   // getpid()
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"   // pour va_list()

#include "exploit.h"
#include "Message.h"
#include "inout.h"

// pour éviter de déclarer la variable comme extern ici
#define __ERREUR_C__
#include "Erreur.h"
int  __Erreur_for_SIGUSR1_flag__ = 0;
char __Erreur_for_SIGUSR1_msg__[] = "Flag du module Erreur activé, impossible de gérer plus d'erreurs !";

// buffer collection d'erreurs: permet de garder la trace d'un certain nb d'erreurs en memoire
// 100000 pointeurs: 400ko
//        erreurs de 604 o => moins de 60Mo
#define N_erreurMax       100000                  // nb max d'erreurs stockables avant kill au processus
#define First_ErreurId    (ErreurId)(-1)          // +1 => 0
// on stocke des pointeurs, les erreurs sont allouées au fur et à mesure
// => permet de gérer plus d'erreurs au cas où, tout en ne réservant pas trop de place mémoire
// dans la plupart des cas où les programmes fonctionnent normalement
static Erreur*    _erreurColl[N_erreurMax];
static ErreurId   _lastErreurColl = First_ErreurId; // +1 => 0
static Erreur     _erreurEmpty; // singleton pour erreur vierge
// pointeur sur la dernière erreur créée
// variable publique pour le debugging uniquement, ne pas référencer dans un source
Erreur*     _erreur = &_erreurEmpty;


static int Default_handler (int ret, void* gen_arg);


//************************************************************************
//* nettoie l'espace memoire disponible pour les erreurs
//************************************************************************
int Erreur_cleanAll (void)
{
  IN();
  Erreur_freeAll();
  memset (&_erreurColl, 0, sizeof(Erreur*)*N_erreurMax);
  _lastErreurColl = First_ErreurId;
  _erreur = Erreur_getLast();
  OUT(0);
  return 0;
}

//************************************************************************
//* libere l'espace memoire disponible pour les erreurs
//************************************************************************
int Erreur_freeAll (void)
{
  IN();
  int i;
  if (_lastErreurColl != First_ErreurId) {
    for (i=0; i<_lastErreurColl ; i++) {
      if (_erreurColl[i] != NULL) free(_erreurColl[i]);
      _erreurColl[i] = NULL;
    }
  }
  _erreur = &_erreurEmpty;
  OUT(0);
  return 0;
}

//************************************************************************
//* retourne la classe d'erreur
//************************************************************************
int Erreur_getClasse (ErreurId id)
{
  IN();
  ErreurId _id = (id == Default_lastErreur) ? _lastErreurColl : id;
  if (_id < 0 || _id >= N_erreurMax) return CLA_VIDE;
  OUT(0);
  return _erreurColl[_id]->classe;
}

/************************************************************************/
/* retourne le libellé associé à la classe d'erreur                     */
/************************************************************************/
char* Erreur_getLibClasse (int classe)
{
  IN();
  switch (classe) {
  case CLA_APP : return LIB_CLA_APP;
  case CLA_SS  : return LIB_CLA_SS;
  case CLA_ORA : return LIB_CLA_ORA;
  case CLA_NP  : return LIB_CLA_NP;
  case CLA_TUX : return LIB_CLA_TUX;
  case CLA_INF : return LIB_CLA_INF;
  case CLA_XML : return LIB_CLA_XML;
  case CLA_EDI : return LIB_CLA_EDI;
  case CLA_SYS : return LIB_CLA_SYS;
  default : return LIB_CLA_VIDE;
  }
  OUT(0);
}

//************************************************************************
//* retourne la fonction en erreur
//************************************************************************
char* Erreur_getFonction (ErreurId id)
{
  IN();
  ErreurId _id = (id == Default_lastErreur) ?
      (_lastErreurColl == First_ErreurId) ? 0 : _lastErreurColl : id;
  if (_id < 0 || _id >= N_erreurMax) _id = _lastErreurColl;
  Erreur* err = (_erreurColl[_id] != NULL)? _erreurColl[_id] : &_erreurEmpty;
  OUT(0);
  return err->fonction;
}

//************************************************************************
//* retourne le message stocké dans l'erreur (sans formatage de l'erreur)
//************************************************************************
char* Erreur_getMessage (ErreurId id)
{
  IN();
  ErreurId _id = (id == Default_lastErreur) ?
      (_lastErreurColl == First_ErreurId) ? 0 : _lastErreurColl : id;
  if (_id < 0 || _id >= N_erreurMax) _id = _lastErreurColl;
  Erreur* err = (_erreurColl[_id] != NULL)? _erreurColl[_id] : &_erreurEmpty;
  OUT(0);
  return err->message;
}

//************************************************************************
//* retourne le nb d'erreurs en stock
//************************************************************************
size_t Erreur_getNbAll (void)
{
  IN();
  OUT(0);
  return (_lastErreurColl == First_ErreurId) ? 0 : _lastErreurColl+1;
}

//************************************************************************
//* retourne l'adresse mémoire d'une erreur à partir de son id
//************************************************************************
Erreur* Erreur_get (ErreurId id)
{
  IN();
  ErreurId _id = (id == Default_lastErreur) ?
      (_lastErreurColl == First_ErreurId) ? 0 : _lastErreurColl : id;
  if (_id < 0 || _id >= N_erreurMax) _id = _lastErreurColl;
  Erreur* err = (_erreurColl[_id] != NULL)? _erreurColl[_id] : &_erreurEmpty;
  OUT(0);
  return err;
}

//////////////////////////////////////////////////////////////////////////
// retourne un pointeur vers la dernière erreur
Erreur* Erreur_getLast (void)
{
  IN();
  OUT(0);
  return Erreur_get(Erreur_getLastId());
}

//////////////////////////////////////////////////////////////////////////
// retourne l'id de la dernière erreur
ErreurId Erreur_getLastId (void)
{
  IN();
  ErreurId _id = (_lastErreurColl == First_ErreurId) ? 0 : _lastErreurColl;
  OUT(0);
  return _id;
}

//************************************************************************
// Message à partir de l'erreur courante
//************************************************************************
char* Erreur_format (void)
{
  IN();
  OUT(0);
  return Erreur_format3(Erreur_getLastId());
}

//************************************************************************
// Message à partir d'une erreur passée en paramètre
//************************************************************************
char* Erreur_format2 (Erreur* erreur)
{
  IN();
  Erreur* err = (erreur != NULL)? erreur : &_erreurEmpty;

  static char messageErreur[L_fonction+L_messageErreur+10+1];
  sprintf (messageErreur, "(%s():%s:%s)",
    err->fonction,
    Erreur_getLibClasse(err->classe),
    err->message);
  OUT(0);
  return messageErreur;
}

//************************************************************************
// Message à partir d'un id d'erreur passé en paramètre
// Cette fonction est moins sensible aux effets de bords que la précédente
// du fait de l'utilisation d'un buffer interne de type LRU cyclique
//************************************************************************
char* Erreur_format3 (ErreurId id)
{
  IN();
#define _N_messageErreur    10
#define _L_messageErreur    L_fonction+L_messageErreur+10+1
  static char messageErreur[_N_messageErreur][_L_messageErreur];
  static int _curMessageErreur = -1;
  ErreurId _id = (id == Default_lastErreur) ?
      (_lastErreurColl == First_ErreurId) ? 0 : _lastErreurColl : id;
  { // astuce pour utiliser un buffer de plusieurs lignes
    // et changer de ligne à chaque appel
    // ca permet par exemple d'afficher plusieurs lignes d'erreurs
    // sans effet de bord (écrasement d'un message par un autre avant affichage
    // si on fait printf("%s, %s",Erreur_format3(0),Erreur_format3(1)))
    _curMessageErreur = (_curMessageErreur+1) % _N_messageErreur; // ça tourne ici
    memset(messageErreur[_curMessageErreur], 0, _L_messageErreur);
    if (_id < 0 || _id >= N_erreurMax) {
      sprintf(messageErreur[_curMessageErreur],"%s(), Unknown error id=[%d]",nomFonction,id);
    }
    else {
      Erreur* err = (_erreurColl[_id] != NULL)? _erreurColl[_id] : &_erreurEmpty;
      sprintf(
          messageErreur[_curMessageErreur]
          , "(%s():%s:%s)"
          , err->fonction
          , Erreur_getLibClasse(err->classe)
          , err->message
      );
    }
  }
  OUT(0);
  return messageErreur[_curMessageErreur];
#undef _N_messageErreur
#undef _L_messageErreur
}

//************************************************************************
// positionne l'erreur
//************************************************************************
ErreurId Erreur_set (int classe, char* fonction,  char* fmt, ...)
{
  IN();
  if (_lastErreurColl != First_ErreurId && _lastErreurColl >= N_erreurMax-1) {
    printf ("Erreur: NO SPACE LEFT too many errors [%d] !\n", N_erreurMax);
    __Erreur_for_SIGUSR1_flag__ = 1;
    kill(getpid(),SIGUSR1);   // ATTENTION erreur critique ici, on tue le processus
  }
  else {
    _lastErreurColl += 1;
    _erreurColl[_lastErreurColl] = (Erreur*) calloc(1,sizeof(Erreur));
    if (_erreurColl[_lastErreurColl]==NULL)
    {
      printf ("Erreur, %s(): CALLOC failed",nomFonction);
      __Erreur_for_SIGUSR1_flag__ = 1;
      kill(getpid(),SIGUSR1);   // ATTENTION erreur critique ici, on tue le processus
    }
    _erreurColl[_lastErreurColl]->classe = classe;
    if (fonction) {
      strcpy(_erreurColl[_lastErreurColl]->fonction, fonction);
    }
    else {
      strcpy(_erreurColl[_lastErreurColl]->fonction, "fonction inconnue");
    }
    if (fmt) {
      va_list ap;
      char msgtmp[L_messageErreur + 1];
      va_start (ap, fmt);
      vsnprintf(msgtmp, L_messageErreur, fmt, ap);
      va_end (ap);
      strcpy(_erreurColl[_lastErreurColl]->message, msgtmp);
    }
    else {
      strcpy(_erreurColl[_lastErreurColl]->message, "message inconnu");
    }
    _erreur = Erreur_getLast();
  }
  OUT(0);
  return _lastErreurColl;
}

//************************************************************************
// traitement "générique" d'un retour en erreur
// On évite la liste variable d'arguments car il y a trop de cas où
// l'analyse des args est imprévisible.
// On préfère un argument générique. A charge de chaque handler de savoir
// ce qu'il recoit.
//************************************************************************
int catchR (int (*handler)(int, void*), int ret, void* gen_arg)
{
  IN();
  int         retCatchR  = 0;
  static int  intoCatchR = 0;
  // rien à faire
  if (ret == 0) goto XIT;
  if (intoCatchR == 0)
  {
    // on vient d'entrer dans le catch
    intoCatchR = 1;
  }
  else
  {
    // on est dans un appel récursif du catch => risque d'emballement, on arrête tout !
    // un appel récursif peut arriver si le catch est utilisé dans une des fonctions appelées par
    // le gestionnaire d'erreur lui même.
    printf("ERREUR: %s() est entrée dans un appel récursif dangereux !",nomFonction);
    retCatchR = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  int   (*the_handler)(int, void*) = NULL;
  if (handler == NULL) {
    the_handler =  Default_handler;
  }
  else {
    the_handler = handler;
  }
  retCatchR = the_handler(ret, gen_arg);
 XIT:
  OUT(ret);
  intoCatchR = 0;   // après gestion d'erreur, terminée sans erreur, on sort du catch
  return retCatchR;
}

///////////////////////////////////////////////////////////////////////////////
static int Default_handler (int ret, void* gen_arg)
{
  IN();
  printf ("Erreur [%d]: %s", ret, Erreur_format());
  OUT(ret);
  return (ret);
}

//************************************************************************
// Ecriture des messages sur la sortie standard en cas d'erreur critique
//************************************************************************
void Erreur_criticalPrint (void)
{
  IN();
  ErreurId _id = _lastErreurColl+1;
  int i;
  printf("\nErreur stack:\n");
  for (i=0;i < N_erreurMax; i++) {
    // ça tourne ici: de la plus récente à la plus lointaine en commençant par la dernière erreur
    _id = ((_id-1)<N_erreurMax) ? _id-1 : N_erreurMax - _id -1;
    if (strcmp(_erreurColl[_id]->fonction,"") != 0 || strcmp(_erreurColl[_id]->message,"") != 0) { // que les erreurs non vides
      printf(
          "(%s():%s:%s)\n"
          , _erreurColl[_id]->fonction
          , Erreur_getLibClasse(_erreurColl[_id]->classe)
          , _erreurColl[_id]->message
      );
    }
  }
  OUT(0);
}

