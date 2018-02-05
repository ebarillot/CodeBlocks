
#if defined _SOLARIS || defined _LINUX
#include <sys/resource.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "Cerr.h"
#include "utile.h"	/* pour IN et OUT */
#include "mallocCtl.h"

/* flags pour savoir si on affiche la allocations
   et desallocations dynamiques, sert a debugger ce module */
#define flagAffMalloc		0
/* pour afficher ou non les In et OUT dans chacune des fonctions
 * tres lourd dans les logs */
#define flagAffINOUT		0

/* taille max de la zone de stockage des adresses non encore liberees */
#define	N_adrmem	1000
/* nb d'adresses stockees qui declenche le vidage dans le fichier des
   allocations des adresses liberees */
#define	Max_adrmem	999


#define	ADRESSE_INCONNUE		-1
#define	ADRESSE_DEJA_LIBRE		-2
#define	GARBAGE_COLLECTION_IMPOSSIBLE	-3

#define	FMT_VARIABLE	"%-25.25s"
#define	FMT_ADRESSE	"%-8.8s"
#define	FMT_ADRESSEx	"%-8.8x"
#define	FMT_TAILLE	"%9.9s"
#define	FMT_TAILLEd	"%9ld"
#define	FMT_ACTUELLE	"%9.9s"
#define	FMT_ACTUELLEd	"%9ld"
#define	FMT_ALLOC	"%-25.25s"
#define	FMT_DESALLOC	"%-25.25s"
#define	FMT_STATUT	"%2.2s"


/************************************************************************/
/* Types								*/
/************************************************************************/
/* pour gerer les adresses allouees */
#define	L_memnom	100
typedef struct {
  void*	adr;			/* l'adresse allouee */
  long	lg;			/* la taille de memoire allouee */
  long	lgact;			/* la taille de memoire allouee actuellement */
  char	idvar[L_memnom+1];	/* nom (facultatif) de la variable */
  char	idalloc[L_memnom+1];	/* demandeur de l'allocation (nom fonction) */
  char	idliber[L_memnom+1];	/* demandeur de la liberation (nom fonction) */
  int	statut;			/* statut OK ou non */
} typAdrmem;

/* valeurs pour le statut d'une adresse */
#define	statutOK	0
#define	statutKO	-1
#define	libStatutOK	"OK"
#define	libStatutKO	"Pb"

/************************************************************************/
/* variable globale							*/
/************************************************************************/
static int mallocCount;	/* nb de malloc/free en dehors
			   de toute gestion d'adresse */

/* flags pour savoir si on affiche les allocations
   et desallocations dynamiques, sert a debugger ce module */
static int trackMalloc;

static typAdrmem	adrmem[N_adrmem];
static int		nbAdrmem = 0;
static int		nbTotAlloc = 0;/* nb total de demandes d'alloc */
static int		szTotDemAlloc = 0;/* total de demandes d'alloc */
static int		szTotAlloc = 0;/* taille totale allouée à l'instant t*/
static int		szMaxAlloc = 0;/* pic max d'allocation */

/************************************************************************/
/* Fonctions locales							*/
/************************************************************************/
static int getIndexAdrmem (void* mem);
static int ecritEntete (void);
static int ecritSeparateurs (void);
static int garbageCollection (void);

/************************************************************************/
/* Gestion de la memoire allouee dynamiquement en service		*/
/************************************************************************/
void stockAdressesOn (void)
{
  trackMalloc = 1;
  afficheMessage(traceMem,MEM,"Suivi allocation memoire activé");
  mallocCount = 0;
  nbTotAlloc  = 0;
  szTotAlloc  = 0;
  szMaxAlloc  = 0;
  return;
}

/************************************************************************/
/* Gestion de la memoire allouee dynamiquement en service		*/
/************************************************************************/
void stockAdressesOnReinitCount (void)
{
  trackMalloc = 1;
  afficheMessage(traceMem,MEM,"Suivi allocation memoire ré-activé");
  mallocCount = 0;
  nbTotAlloc  = 0;
  return;
}

/************************************************************************/
/* Gestion de la memoire allouee dynamiquement hors service		*/
/************************************************************************/
void stockAdressesOff (void)
{
  trackMalloc = 0;
  afficheMessage(traceMem,MEM,"Suivi allocation memoire desactivé");
  return;
}

/*****************************************************************************/
/* Affichage du solde d'appels malloc/free				     */
/*****************************************************************************/
int getMallocCount (void)
{
  return mallocCount;
}

/*****************************************************************************/
/* Gestion de la memoire allouee dynamiquement: ALLOCATION                   */
/*****************************************************************************/
void* mallocCtl (long	sz,			/* in */
		 char*	varNom,			/* in */
		 char*	foncNom)		/* in */
{
  char*	nomFonction = "mallocCtl";
  void*	mem = NULL;

#if flagAffINOUT
  IN(nomFonction);
#endif
  mem = malloc(sz);
  if (mem) memset(mem,0,sz);
  if (trackMalloc) {
    if (foncNom) {
      strcpy (adrmem[nbAdrmem].idalloc, foncNom);
      strcpy (adrmem[nbAdrmem].idliber, "");
    }
    else {
      strcpy (adrmem[nbAdrmem].idalloc, "<inconnue>");
      strcpy (adrmem[nbAdrmem].idliber, "");
    }
    if (varNom) strcpy (adrmem[nbAdrmem].idvar, varNom);
    else strcpy (adrmem[nbAdrmem].idvar, "<nom inconnu>");
    adrmem[nbAdrmem].adr   = mem;
    adrmem[nbAdrmem].lg    = sz;
    adrmem[nbAdrmem].lgact = sz;
    if (mem) {
      adrmem[nbAdrmem].statut = statutOK;
    }
    else {
      adrmem[nbAdrmem].statut = statutKO;
    }
    nbAdrmem +=1;
    nbTotAlloc +=1;
    szTotDemAlloc += sz;
    szTotAlloc += sz;
    szMaxAlloc = (szTotAlloc>szMaxAlloc) ? szTotAlloc : szMaxAlloc;
  }
#if flagAffMalloc
  afficheMessage(traceMem,MEM,"MEM ALLOC %x (%s - %s)",mem,varNom,foncNom);
#endif
  if (mem) mallocCount += 1;
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return (mem);
}

/*****************************************************************************/
/* Gestion de la memoire allouee dynamiquement: REALLOCATION                 */
/* sans remise à 0 de la zone allouee					     */
/*****************************************************************************/
void* reallocCtl (void*	mem,			/* in / out */
		  long	sz,			/* in */
		  char*	varNom,			/* in */
		  char*	foncNom)		/* in */
{
  char*	nomFonction = "reallocCtl";
  void*	old = mem;
  int	iAdr = 0;
  int	oldsz = 0;

#if flagAffINOUT
  IN(nomFonction);
#endif
  /* printf("old=%x  mem=%x\n",old,mem); */
  /* les données sont inchangées */
  if (!(mem = realloc (mem, sz))) {
    fillCurrentError(CLA_MEM,nomFonction,"allocation impossible");
    afficheMessage(traceMem,MEM,"allocation impossible");
    goto XIT;
  }
  /* printf("old=%x  mem=%x\n",old,mem); */
  /* si l'adresse a changé */
  if (old != mem && trackMalloc) {
    iAdr = getIndexAdrmem(old);
    if (iAdr == ADRESSE_INCONNUE) {
      fillCurrentError(CLA_MEM,nomFonction,
		       "adresse inconnue %x reallouee par %s",
		       old,foncNom);
      afficheMessage(traceMem,MEM,"%s adresse inconnue %x reallouee par %s",
		     nomFonction,old,foncNom);
    }
    else {
      /* on met à jour la table pour l'ancienne adresse */
      if (foncNom) {
	strcpy (adrmem[iAdr].idliber, foncNom);
      }
      else {
	strcpy (adrmem[iAdr].idliber, "<inconnue>");
      }
      oldsz = adrmem[iAdr].lgact;
      adrmem[iAdr].lgact = 0;
      adrmem[iAdr].statut = 0;
      /* on cree une nouvelle entree dans la table pour la nouvelle adresse */
      if (foncNom) {
	strcpy (adrmem[nbAdrmem].idalloc, foncNom);
	strcpy (adrmem[nbAdrmem].idliber, "");
      }
      else {
	strcpy (adrmem[nbAdrmem].idalloc, "<inconnue>");
	strcpy (adrmem[nbAdrmem].idliber, "");
      }
      if (varNom) strcpy (adrmem[nbAdrmem].idvar, varNom);
      else strcpy (adrmem[nbAdrmem].idvar, "<nom inconnu>");
      adrmem[nbAdrmem].adr   = mem;
      adrmem[nbAdrmem].lg    = sz;
      adrmem[nbAdrmem].lgact = sz;
      if (mem) {
	adrmem[nbAdrmem].statut = statutOK;
      }
      else {
	adrmem[nbAdrmem].statut = statutKO;
      }
      nbAdrmem +=1;
      nbTotAlloc +=1;
      szTotDemAlloc += (sz-oldsz);
      szTotAlloc += (sz-oldsz);
      szMaxAlloc = (szTotAlloc>szMaxAlloc) ? szTotAlloc : szMaxAlloc;
    }
  }
#if flagAffMalloc
  afficheMessage(traceMem,MEM,"MEM REALLOC %x (%s - %s)",mem,varNom,foncNom);
#endif
 XIT:
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return mem;
}

/*****************************************************************************/
/* Gestion de la memoire allouee dynamiquement: DESALLOCATION                */
/* ATTENTION: ne libère l'adresse que si elle est connue de la table des     */
/* adresses allouées                                                         */
/*****************************************************************************/
int freeCtl (void*	mem,			/* in/out */
	     char*	foncNom)		/* in */
{
  char*	nomFonction = "FreeCtl";
  int	ret = 0;
  int	iAdr = 0;

#if flagAffINOUT
  IN(nomFonction);
#endif
#if flagAffMalloc
  afficheMessage(traceMem,MEM,"MEM FREE %x (%s)",mem,foncNom);
#endif
  if (trackMalloc) {
    iAdr = getIndexAdrmem(mem);
    if (iAdr == ADRESSE_INCONNUE) {
      fillCurrentError(CLA_MEM,nomFonction,
		       "adresse inconnue %x libérée par %s",
		       mem,foncNom);
      afficheMessage(traceMem,MEM,"%s adresse inconnue %x libérée par %s",
		     nomFonction,mem,foncNom);
      goto XIT;
    }
    if (mem) free (mem);
    szTotAlloc -= adrmem[iAdr].lgact;
    if (foncNom) {
      strcpy (adrmem[iAdr].idliber, foncNom);
    }
    else {
      strcpy (adrmem[iAdr].idliber, "<inconnue>");
    }
    adrmem[iAdr].lgact = 0;
    adrmem[iAdr].statut = 0;
  }
  else {
    /* même si pas de tracking, on déslloue */
    if (mem) free (mem);
  }
 XIT:
  /* on ne decremente que les zones reellement desallouees */
  if (mem) mallocCount -= 1;
  if (mallocCount < 0) mallocCount = 0;
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return (ret);
}

/************************************************************************/
/* Retourne l'indice d'une variable dans le tableau des adresses	*/
/* L'adresse en question doit correspondre a une zone memoire		*/
/* effectivement allouee						*/
/************************************************************************/
static int getIndexAdrmem (void* mem)
{
  char*	nomFonction = "getIndexAdrmem";
  int i;
  int indice = ADRESSE_INCONNUE;
  int lgact = -1;
#if flagAffINOUT
  IN(nomFonction);
#endif
  for (i=0; i<nbAdrmem; i++) {
    if (mem == adrmem[i].adr && adrmem[i].statut == statutOK) {
      /* si une adresse apparait plusieurs fois dans le tableau,
	 on conserve la ligne qui est encore allouée si elle existe */
      if (adrmem[i].lgact > lgact) {
	indice = i;
	lgact = adrmem[i].lgact;
      }
    }
  }
  if (indice == ADRESSE_INCONNUE || lgact == -1) {
    indice = ADRESSE_INCONNUE;
  }
  else if (lgact == 0) {
    indice = ADRESSE_DEJA_LIBRE;
  }
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return indice;
}

/************************************************************************/
/* Affichage des adresses allouee					*/
/* avec ou sans suppression des adresses inutilisées ensuite		*/
/************************************************************************/
int affAdrmem(int gc, int entete)
{
  char	nomFonction[] = "affAdrmem";
  int		ret = 0;
  int i;
  static int	init = 0;
  char	straff[1000+1];
  char	tmpstr[1000+1];
#if flagAffINOUT
  IN(nomFonction);
#endif
  if (trackMalloc) {
    afficheMessage(traceMem,MEM,"Nb d'allocations demandées = %d",
		   nbTotAlloc);
    afficheMessage(traceMem,MEM,"Taille totale demandée = %d", szTotDemAlloc);
    afficheMessage(traceMem,MEM,"Taille allouée à l'instant = %d",szTotAlloc);
    afficheMessage(traceMem,MEM,"Pic de mémoire allouée = %d",szMaxAlloc);
    afficheMessage(traceMem,MEM,"-----");
    if (init == 0) {
      init = 1;
      ecritEntete ();
    }
    else if (entete == enteteOn) {
      ecritEntete ();
    }
    /* separateurs ecrits a chaque passage */
    ecritSeparateurs ();
    for (i=0; i<nbAdrmem;i++) {
      memset(tmpstr,0,1001);
      memset(straff,0,1001);
      sprintf(tmpstr, FMT_VARIABLE, adrmem[i].idvar);
      strcat(straff,tmpstr); strcat(straff,"|");
      sprintf(tmpstr, FMT_ADRESSEx, adrmem[i].adr);
      strcat(straff,tmpstr); strcat(straff,"|");
      sprintf(tmpstr, FMT_TAILLEd, adrmem[i].lg);
      strcat(straff,tmpstr); strcat(straff,"|");
      sprintf(tmpstr, FMT_ACTUELLEd, adrmem[i].lgact);
      strcat(straff,tmpstr); strcat(straff,"|");
      sprintf(tmpstr, FMT_ALLOC, adrmem[i].idalloc);
      strcat(straff,tmpstr); strcat(straff,"|");
      sprintf(tmpstr, FMT_DESALLOC, adrmem[i].idliber);
      strcat(straff,tmpstr); strcat(straff,"|");
      sprintf(tmpstr, FMT_STATUT,
	       (adrmem[i].statut == statutOK) ? libStatutOK : libStatutKO);
      strcat(straff,tmpstr); strcat(straff,"|");
      afficheMessage(traceMem,MEM,"%s\n",straff);
    }
    if (gc == GC_OK) {
      afficheMessage(traceMem,MEM,"Garbage collection effectuée");
      if ((ret = garbageCollection ()) != 0) return (ret);
    }
  }
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return (0);
}

/************************************************************************/
/* Ecriture de l'entete							*/
/************************************************************************/
static int ecritEntete(void)
{
  char	nomFonction[] = "ecritEntete";
  char	straff[1000+1];
  char	tmpstr[1000+1];
#if flagAffINOUT
  IN(nomFonction);
#endif
  memset(tmpstr,0,1001);
  memset(straff,0,1001);
  sprintf(tmpstr, FMT_VARIABLE, "Variable");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_ADRESSE, "Adresse");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_TAILLE, "Taille");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_ACTUELLE, "Actuelle");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_ALLOC, "Allocation par");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_DESALLOC, "Liberation par");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_STATUT, "St");
  strcat(straff,tmpstr); strcat(straff,"|");
  afficheMessage(traceMem,MEM,"%s",straff);
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return (0);
}

/************************************************************************/
/* Ecriture de l'entete							*/
/************************************************************************/
static int ecritSeparateurs(void)
{
  char	nomFonction[] = "ecritSeparateurs";
  char	straff[1000+1];
  char	tmpstr[1000+1];
#if flagAffINOUT
  IN(nomFonction);
#endif
  memset(tmpstr,0,1001);
  memset(straff,0,1001);
  sprintf(tmpstr, FMT_VARIABLE, "--------");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_ADRESSE, "-------");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_TAILLE, "------");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_ACTUELLE, "--------");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_ALLOC, "--------------");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_DESALLOC, "--------------");
  strcat(straff,tmpstr); strcat(straff,"|");
  sprintf(tmpstr, FMT_STATUT, "--");
  strcat(straff,tmpstr); strcat(straff,"|");
  afficheMessage(traceMem,MEM,"%s\n",straff);
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return (0);
}

/************************************************************************/
/* GARBAGE COLLECTION dans le tableau des adresses			*/
/* On enleve toutes les entrees qui n'ont plus de memoire allouee	*/
/************************************************************************/
static int garbageCollection(void)
{
  char	nomFonction[] = "garbageCollection";
  int	ret = 0;
  int i;
  typAdrmem	tmpAdrmem[N_adrmem];
  int		tmpNbAdrmem = 0;

#if flagAffINOUT
  IN(nomFonction);
#endif
  if (!trackMalloc) goto XIT;
  memset(tmpAdrmem, 0, sizeof tmpAdrmem);
  for (i=0; i<nbAdrmem; i++) {
    if (adrmem[i].lgact > 0) {
      tmpAdrmem[tmpNbAdrmem] = adrmem[i];
      tmpNbAdrmem += 1;
    }
  }
  for (i=0; i<tmpNbAdrmem; i++) {
    adrmem[i] = tmpAdrmem[i];
  }
  nbAdrmem = tmpNbAdrmem;
  /* on ne peut pas liberer plus de memoire, donc on ne peux plus gerer
     les adresses */
  if (nbAdrmem >= Max_adrmem) {
    trackMalloc = 0;
    ret = GARBAGE_COLLECTION_IMPOSSIBLE;
    goto XIT;
  }
 XIT:
#if flagAffINOUT
  OUT(nomFonction,ret);
#endif
  return (ret);
}
