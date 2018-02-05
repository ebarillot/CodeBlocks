
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>	/* pour va_list */
#include <string.h>
#include <signal.h>

#ifdef _USE_ORA
#include <ortutil.h>
#include <ortproc.h>
#endif

#include <exploit.h>	/* pour CODE_FIN_ERREUR_APPLICATIVE */

#include "Cerr.h"
#include "utile.h"
#include "mallocCtl.h"

#include "Astring.h"

#if 0
#define	flagInout
#else
#undef	flagInout
#endif

/************************************************************************/
/* variables globales							*/
/************************************************************************/

/************************************************************************/
/************************************************************************/
/*									*/
/* Apchar								*/
/*									*/
/************************************************************************/
/************************************************************************/

/************************************************************************/
/* constructeur								*/
/************************************************************************/
Apchar*	newApchar(int n)
{
  char	nomFonction[] = "newApchar";
  int	ret = 0;
  Apchar* apchar = NULL;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (!(apchar = mallocCtl(sizeof(Apchar),"apchar",nomFonction))) {
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else {
    if (!(apchar->a = mallocCtl(sizeof(pchar)*n,"apchar->a",nomFonction))) {
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    else {
      apchar->nalloc = n;
    }
  }
 XIT:
  if (ret) {
    if (apchar) freeCtl((void*)apchar,nomFonction);
  }
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return apchar;
}

/************************************************************************/
/* constructeur								*/
/************************************************************************/
Apchar*	reallocApchar(Apchar* ap, int n)
{
  char	nomFonction[] = "reallocApchar";
  int ret = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  ap->a = reallocCtl(ap->a,sizeof(pchar)*n,"ap->a",nomFonction);
  if (!(ap->a)) {
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    OUT(nomFonction,ret);
    return NULL;
  }
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ap;
}

/************************************************************************/
/* détruit une instance de Apchar					*/
/************************************************************************/
int	freeApchar(Apchar* ap)
{
  char	nomFonction[] = "freeApchar";
  int	ret = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (ap && ap->a) freeCtl((void*)(ap->a),nomFonction);
  if (ap) freeCtl((void*)ap,nomFonction);
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* nettoie le tableau de pointeurs					*/
/************************************************************************/
int	clearApchar(Apchar* ap)
{
  char	nomFonction[] = "clearApchar";
  int	ret = 0;
  int	i = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  ap->nused = 0;
  for (i=0;i<ap->nalloc;i++) {
    ap->a[i].s = NULL;
  }
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Affectation d'un élément du tableau					*/
/************************************************************************/
int	setApchar(Apchar* ap, int i, char* s)
{
  char	nomFonction[] = "setApchar";
  int	ret = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (i>=ap->nalloc) {
    keepError(CLA_APP, nomFonction, "Tableau trop petit");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  ap->a[i].s = s;
 XIT:
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Affectation d'un élément du tableau					*/
/************************************************************************/
int	getNallocApchar(Apchar* ap)
{
  return (ap->nalloc);
}

/************************************************************************/
/* Affectation d'un élément du tableau					*/
/************************************************************************/
int	getNusedApchar(Apchar* ap)
{
  return (ap->nused);
}

/************************************************************************/
/************************************************************************/
/*									*/
/* Astring								*/
/*									*/
/************************************************************************/
/************************************************************************/


/************************************************************************/
/* constructeur								*/
/* en entrée:								*/
/*   - le volume supposé du contenu					*/
/*   - le nb supposé de pointeurs sur les parties du contenu		*/
/************************************************************************/
Astring*	newAstring(long vol, int np)
{
  char	nomFonction[] = "newAstring";
  int	ret = 0;
  Astring* as = NULL;
  char*    s  = NULL;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (!(as = mallocCtl(sizeof(Astring), "as",nomFonction))) {
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else {
    if (!(s = mallocCtl(vol, "s",nomFonction))) {
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    as->s = s;
    as->lalloc = vol;
    /* on n'alloue le tableau de pointeurs que si demandé */
    if (np > 0) {
      if (!(as->ap = newApchar(np))) {
	goto XIT;
      }
    }
  }
 XIT:
  if (ret) {
    if (s) freeCtl((void*)s,nomFonction);
    if (as) freeCtl((void*)as,nomFonction);
  }
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return as;
}

/************************************************************************/
/* détruit une instance de Astring					*/
/************************************************************************/
int	freeAstring(Astring* as)
{
  char	nomFonction[] = "freeAstring";
  int	ret = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (as && as->s) freeCtl((void*)(as->s),nomFonction);
  if (as && as->ap) freeApchar(as->ap);
  if (as) freeCtl((void*)as,nomFonction);
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Recopie le buffer "contenu"						*/
/* mais ne fait pas le decoupage					*/
/************************************************************************/
int	setAstring(Astring* as, char* buffin, int mode)
{
  char	nomFonction[] = "setAstring";
  int	ret = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) {
    keepError(CLA_APP,nomFonction,"Objet non alloué");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!as->s) {
    keepError(CLA_APP,nomFonction,"Contenu non alloué");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (mode == O_COPY) {
    if (as->lalloc <= (strlen(buffin)+1)) {
      keepError(CLA_APP,nomFonction,"Pas assez de place dans la string");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    strcpy(as->s, buffin);
  }
  else {
    if (as->lalloc <= (as->lused+1+strlen(buffin)+1)) {
      keepError(CLA_APP,nomFonction,"Pas assez de place dans la string");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    strcat(as->s, buffin);
  }
  as->lused = strlen(as->s);
 XIT:
  if (ret) freeAstring(as);
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Place les pointeurs sur les séparateurs				*/
/************************************************************************/
int	sepAstring(Astring* as, char sep)
{
  char	nomFonction[] = "sepAstring";
  int	ret = 0;
  char*	ptr = NULL;
  int	nptr = N_ptrAstring;

#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) {
    keepError(CLA_APP,nomFonction,"Struct Astring non initialisée");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!as->s) {
    keepError(CLA_APP,nomFonction,"Aucun contenu");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (as->ap) {
    clearApchar(as->ap);
  }
  else {
    as->ap = newApchar(nptr);
  }
  ptr = as->s;
  as->ap->a[as->ap->nused].s = ptr;	/* la 1ere ligne */
  as->ap->nused++;
  while (*ptr) {
    if (*ptr == sep) {
      if (as->ap->nused >= as->ap->nalloc) {
	nptr += N_ptrAstring;
	if (!(as->ap = reallocApchar(as->ap, nptr))) goto XIT;
      }
      as->ap->a[as->ap->nused].s = ptr+1;	/* les lignes suivantes */
      *ptr = 0x00;	/* on coupe les lignes */
      as->ap->nused++;
    }
    ptr++;
  }
 XIT:
  if (ret) {
    freeAstring(as);
  }
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Place les pointeurs sur les séparateurs				*/
/************************************************************************/
long	getLenAstring(Astring* as)
{
  char	nomFonction[] = "getLenAstring";
#ifdef flagInout
  IN(nomFonction);
#endif
  if (as && as->s) {
#ifdef flagInout
    OUT(nomFonction,as->lused);
#endif
    return as->lused;
  }
  else {
#ifdef flagInout
    OUT(nomFonction,(-1));
#endif
    return -1;
  }
}

/************************************************************************/
/* Place les pointeurs sur les séparateurs				*/
/************************************************************************/
long	getNlinesAstring(Astring* as)
{
  char	nomFonction[] = "getNlinesAstring";
#ifdef flagInout
  IN(nomFonction);
#endif
  if (as && as->ap) {
#ifdef flagInout
    OUT(nomFonction,as->ap->nused);
#endif
    return as->ap->nused;
  }
  else {
#ifdef flagInout
    OUT(nomFonction,(-1));
#endif
    return -1;
  }
}

/************************************************************************/
/* Concatène les lignes dans un buffer					*/
/* si le séparateur est NULL, pas de séparateur, les champs sont	*/
/* mis bout à bout.							*/
/************************************************************************/
long	getStringAstring(Astring* as, char* buffout, long lmax, char* sep)
{
  char	nomFonction[] = "getStringAstring";
#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) return -1;
  if (!as->ap) {
    strcpy (buffout, as->s);
  }
  else  {
    int i = 0;
    while (as->ap->a[i].s) {
      if ((strlen(as->ap->a[i].s) + strlen(buffout)) > lmax) break;
      if (i&&sep) strcat (buffout, sep);
      strcat (buffout, as->ap->a[i].s);
      i++;
    }
  }
#ifdef flagInout
  OUT(nomFonction,0);
#endif
  return strlen(buffout);
}

/************************************************************************/
/* Affiche l'objet							*/
/************************************************************************/
char*	toStringAstring(Astring* as, char* sep)
{
  char	nomFonction[] = "toStringAstring";
#define	L_messageOut	10000
  static char	out[L_messageOut+1];
#ifdef flagInout
  IN(nomFonction);
#endif
  memset(out, 0, L_messageOut+1);
  sprintf (out, "Astring (lalloc=%ld) (lused=%ld) (nalloc=%d) (nused=%d) (s=",
	   as->lalloc, as->lused, as->ap->nalloc, as->ap->nused);
  getStringAstring(as, out, (L_messageOut-100), sep);
  strcat(out, ")");
#ifdef flagInout
  OUT(nomFonction,0);
#endif
  return out;
#undef	L_messageOut
}

/************************************************************************/
/* Affiche le Dossier sur la sortie standard				*/
/************************************************************************/
int	afficheAstring(int level, Astring* as)
{
  char	nomFonction[] = "afficheAstring";
  int	ret = 0;
#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) {
    afficheMessage(level,ERR,"Impossible d'afficher le tableau");
  }
  else if (!as->ap) {
    afficheMessage(level,ERR,"Impossible d'afficher le tableau");
  }
  else  {
    int i=0;
    afficheMessage(level,LOG,"DEBUT affichage tableau:");
    afficheMessage(level,LOG,"Nb de champs %d", as->ap->nused);
    afficheMessage(level,LOG,"Nb de pointeurs alloués %d", as->ap->nalloc);
    afficheMessage(level,LOG,"Longueur totale allouée %ld", as->lalloc);
    afficheMessage(level,LOG,"Longueur totale occupée %ld", as->lused);
    while (as->ap->a[i].s) {
      afficheMessage(level,LOG,"%3.3d - \'%s\'",i,as->ap->a[i].s);
      i++;
    }
    afficheMessage(level,LOG,"FIN affichage tableau");
  }
 XIT:
#ifdef flagInout
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Enlève les espaces à droite de chaque champ				*/
/************************************************************************/
int	rtrimAstring(Astring* as)
{
  char	nomFonction[] = "rtrimAstring";
#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) {
#ifdef flagInout
    OUT(nomFonction,-1);
#endif
    return -1;
  }
  {
    int i = 0;
    while (as->ap->a[i].s) {
      myRtrim(as->ap->a[i].s);
      i++;
    }
  }
#ifdef flagInout
  OUT(nomFonction,0);
#endif
  return 0;
}

/************************************************************************/
/* Ajoute un élément au tableau.					*/
/* Suppose que la Astring a été allouée de même que le tableau de	*/
/* pointeurs.								*/
/* elem est une chaine avec terminateur					*/
/************************************************************************/
int	addElemAstring(Astring* as, char* elem)
{
  char	nomFonction[] = "addElemAstring";
  int	ret = 0;

#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) {
    keepError(CLA_APP,nomFonction,"Struct Astring non initialisée");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else {
    if (!as->s) {
      keepError(CLA_APP,nomFonction,"Aucun contenu");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    if (as->s && !as->ap) {
      keepError(CLA_APP,nomFonction,"Contenu sans pointeurs");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }

  /* ajout de la chaine de l'élément */
  if (as->lalloc <= (as->lused+1+strlen(elem))) {
    as->lalloc += L_volAstring;
    if (!(as->s = reallocString(as->s, as->lalloc))) goto XIT;
  }
  if (as->lused<=0) {	/* le 1er elem */
    as->lused = 0;
    strcpy(as->s, elem);	/* copie du contenu */
    as->ap->a[0].s = as->s;	/* pointeur sur l'elem */
    as->ap->nused = 1;		/* nb de pointeurs */
    as->lused += 1+strlen(elem);/* place utilisée (terminateur compris) */
  }
  else {	/* les elem suivants */
    strcpy((as->s+as->lused),elem);
    if (as->ap->nused >= as->ap->nalloc) {
      as->ap->nalloc += N_ptrAstring;	/* on ajoute un paquet de pointeurs */
      if (!(as->ap = reallocApchar(as->ap, as->ap->nalloc))) goto XIT;
    }
    as->ap->a[as->ap->nused].s = as->s+as->lused;/* les lignes suivantes */
    as->ap->nused++;
    as->lused += 1+strlen(elem);
  }
 XIT:
  if (ret) {
    freeAstring(as);
  }
#ifdef flagInout
  printErrors();
  OUT(nomFonction,ret);
#endif
  return ret;
}

/************************************************************************/
/* Retourne le contenu d'un élement du tableau.				*/
/* commence à 0								*/
/************************************************************************/
char*	getElemAstring(Astring* as, int ielem)
{
  char	nomFonction[] = "getElemAstring";
  int	ret = 0;
  char*	ptr = NULL;

#ifdef flagInout
  IN(nomFonction);
#endif
  if (!as) {
    keepError(CLA_APP,nomFonction,"Struct Astring non initialisée");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else {
    if (!as->s) {
      keepError(CLA_APP,nomFonction,"Aucun contenu");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    if (as->s && !as->ap) {
      keepError(CLA_APP,nomFonction,"Contenu sans pointeurs");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  if (ielem >= as->ap->nused) {
    ptr = NULL;
    goto XIT;
  }
  ptr = as->ap->a[ielem].s;

 XIT:
#ifdef flagInout
  OUT(nomFonction,0);
#endif
  return ptr;
}

