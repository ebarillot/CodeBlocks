#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "MallocCtl.h"

/* taille max de la zone de stockage des adresses non encore liberees */
#define  N_ADRESSES  1000
/* nb d'adresses stockees qui declenche le vidage dans le fichier des
   allocations des adresses liberees */
#define  LIM_ADRESSES  999

/* flags pour savoir si on affiche la allocations
   et desallocations dynamiques, sert a debugger ce module */
#define flagAffMemoire    0



/************************************************************************/
/* Types de données							*/
/************************************************************************/
static int mallocCount;  /* nb de malloc/free en dehors
			   de toute gestion d'adresse */

/* pour gerer la quantite de memoire allouee */
typedef struct {
  long cum;    /* la totalite de la memoire allouee cumulee */
  long max;    /* la quantite max de memoire allouee a l'instant t */
  long all;    /* la totalite de la memoire allouee a l'instant t */
  long last;    /* la derniere allocation */
  void *adress;    /* la derniere adresse accedee */
  int mallocCount;  /* nb de malloc/free */
} typMemoire;

/* pour gerer les adresses allouees */
#define  L_NOM  200
typedef struct {
  void *ptr;      /* l'adresse de la variable pointeur */
  void *adresse;    /* l'adresse allouee */
  long lg;      /* la taille de memoire allouee */
  long lgact;      /* la taille de memoire allouee actuellement */
  char idvar[L_NOM + 1];    /* nom (facultatif) de la variable */
  char idalloc[L_NOM + 1];  /* demandeur de l'allocation (nom fonction) */
  char idliber[L_NOM + 1];  /* demandeur de la liberation (nom fonction) */
  int statut;      /* statut OK ou non */
} typAdresse;

/* valeurs pour le statut d'une adresse */
#define  statutOK  0
#define  statutKO  -1
#define  LibstatutOK  "OK"
#define  LibstatutKO  "Pb"

/************************************************************************/
/* Prototypes des fonctions internes au module				*/
/************************************************************************/
static int GarbageCollection(void);

static int EcritEntete(FILE *fd);

static int EcritSeparateurs(FILE *fd);

static int GetIndiceAdresse(void *mem);

#define  FMT_PTR    "%-8.8s"
#define  FMT_PTRx  "%-8.8x"
#define  FMT_VARIABLE  "%-25.25s"
#define  FMT_ADRESSE  "%-8.8s"
#define  FMT_ADRESSEx  "%-8.8x"
#define  FMT_TAILLE  "%9.9s"
#define  FMT_TAILLEd  "%9ld"
#define  FMT_ACTUELLE  "%9.9s"
#define  FMT_ACTUELLEd  "%9ld"
#define  FMT_ALLOC  "%-25.25s"
#define  FMT_DESALLOC  "%-25.25s"
#define  FMT_STATUT  "%2.2s"



/************************************************************************/
/* variables globales							*/
/************************************************************************/
/* pour gerer la memoire allouee */
static typMemoire LaMemoire;
static typAdresse Adresses[N_ADRESSES];
static int nbAdresses = 0;
static struct rlimit rlp;
static int flagGereAdresses = 0;


/************************************************************************/
/* Gestion de la memoire allouee dynamiquement en service		*/
/************************************************************************/
void StockAdressesOn(void) {
  flagGereAdresses = 1;
  return;
}

/************************************************************************/
/* Gestion de la memoire allouee dynamiquement hors service		*/
/************************************************************************/
void StockAdressesOff(void) {
  flagGereAdresses = 0;
  return;
}

/*****************************************************************************/
/* Gestion de la memoire allouee dynamiquement: ALLOCATION                   */
/*****************************************************************************/
int MallocCtl(void **mem,      /* out */
              long sz,      /* in */
              char *varNom,      /* in */
              char *foncNom,    /* in */
              char *foncMessageErreur)  /* out */
{
  void *alloue = NULL;
  int codeERR = 0;
  char *foncLocale = "MallocCtl";

  if (mem == NULL) {
    codeERR = POINTEUR_NULL;
    if (foncMessageErreur) {
      if (varNom)
        sprintf(foncMessageErreur,
                "%s() Adresse du pointeur à NULL [%s]",
                foncLocale,
                varNom);
      else
        sprintf(foncMessageErreur, "%s() Adresse du pointeur à NULL",
                foncLocale);
    }
    goto XIT;
  }
  if (sz <= 0) {
    codeERR = TAILLE_INCORRECTE;
    if (foncMessageErreur)
      sprintf(foncMessageErreur,
              "%s() taille à allouer incorrecte = %d",
              foncLocale,
              sz);
    goto XIT;
  }
  /* probleme si le pointeur a allouer n'est pas null, il pointe peut
     etre encore sur une zone non desallouee */
  if (*mem != NULL) {
    codeERR = POINTEUR_NON_NULL;
    if (foncMessageErreur) {
      if (varNom)
        sprintf(foncMessageErreur,
                "%s() Pointeur non desalloue [%s]",
                foncLocale,
                varNom);
      else
        sprintf(foncMessageErreur,
                "%s() Pointeur non desalloue",
                foncLocale);
    }
    goto XIT;
  }

  if (flagGereAdresses) {
    if (nbAdresses >= LIM_ADRESSES) {
      codeERR = GarbageCollection();
      if (codeERR) {
        if (foncMessageErreur)
          strcpy(foncMessageErreur,
                 "GarbageCollection() Garbage collection impossible, zone "
                     "de stockage des adresses pleine");
        goto XIT;
      }
    }
    if (foncNom) {
      strcpy(Adresses[nbAdresses].idalloc, foncNom);
      strcpy(Adresses[nbAdresses].idliber, "");
    } else {
      strcpy(Adresses[nbAdresses].idalloc, "<inconnue>");
      strcpy(Adresses[nbAdresses].idliber, "");
    }
    if (varNom) strcpy(Adresses[nbAdresses].idvar, varNom);
    else strcpy(Adresses[nbAdresses].idvar, "<nom inconnu>");
    Adresses[nbAdresses].ptr = mem;
    Adresses[nbAdresses].lg = sz;
    Adresses[nbAdresses].lgact = sz;
  }
  alloue = malloc(sz);
  if (alloue != NULL) {
    *mem = alloue;
    memset(alloue, 0, sz);
    mallocCount += 1;
    if (flagGereAdresses) {
      LaMemoire.mallocCount += 1;
      LaMemoire.cum += sz;
      LaMemoire.all += sz;
      LaMemoire.max = (LaMemoire.all > LaMemoire.max) ?
                      LaMemoire.all : LaMemoire.max;
      LaMemoire.last = sz;
      LaMemoire.adress = alloue;
      Adresses[nbAdresses].adresse = alloue;
      Adresses[nbAdresses].statut = 0;
    }
    codeERR = 0;
  } else {
    *mem = NULL;
    if (foncMessageErreur)
      sprintf(foncMessageErreur,
              "%s() Erreur allocation memoire",
              foncLocale);
    codeERR = MALLOC_ALLOCATION_IMPOSSIBLE;
    if (flagGereAdresses) {
      LaMemoire.last = 0;
      Adresses[nbAdresses].adresse = NULL;
      Adresses[nbAdresses].lg = 0;
      Adresses[nbAdresses].lgact = 0;
      Adresses[nbAdresses].statut = -1;
    }
  }
  if (flagGereAdresses) {
#if flagAffMemoire
    AffMemoire (stdout);
#endif
    nbAdresses += 1;
  }
  XIT:
  return (codeERR);
}

/*****************************************************************************/
/* Gestion de la memoire allouee dynamiquement: REALLOCATION                 */
/* sans remise à 0 de la zone allouee					     */
/*****************************************************************************/
int ReallocCtl(void **mem,      /* in/out */
               long sz,      /* in */
               char *varNom,      /* in */
               char *foncNom,    /* in */
               char *foncMessageErreur)  /* out */
{
  void *alloue = NULL;
  int codeERR = 0;
  char *foncLocale = "MallocCtl";
  int iAdresse;

  if (mem == NULL) {
    codeERR = POINTEUR_NULL;
    if (foncMessageErreur) {
      if (varNom)
        sprintf(foncMessageErreur,
                "%s() Adresse du pointeur à NULL [%s]",
                foncLocale,
                varNom);
      else
        sprintf(foncMessageErreur,
                "%s() Adresse du pointeur à NULL",
                foncLocale);
    }
    goto XIT;
  }
  if (sz <= 0) {
    codeERR = TAILLE_INCORRECTE;
    if (foncMessageErreur)
      sprintf(foncMessageErreur,
              "%s() taille à allouer incorrecte = %d",
              foncLocale,
              sz);
    goto XIT;
  }
  /* probleme si le pointeur a allouer est null, il ne pointe pas
     sur une zone non desallouee */
  if (*mem == NULL) {
    codeERR = POINTEUR_NULL;
    if (foncMessageErreur) {
      if (varNom)
        sprintf(foncMessageErreur,
                "%s() Pointeur non alloue [%s]",
                foncLocale,
                varNom);
      else
        sprintf(foncMessageErreur,
                "%s() Pointeur non alloue",
                foncLocale);
    }
    goto XIT;
  }

  if (flagGereAdresses) {
    if (nbAdresses >= LIM_ADRESSES) {
      codeERR = GarbageCollection();
      if (codeERR) {
        if (foncMessageErreur)
          strcpy(foncMessageErreur,
                 "GarbageCollection() Garbage collection impossible, zone "
                     "de stockage des adresses pleine");
        goto XIT;
      }
    }
    /* recherche de l'adresse à réallouer */
    iAdresse = GetIndiceAdresse(*mem);
    if (iAdresse == ADRESSE_INCONNUE) {
      if (foncMessageErreur)
        sprintf(foncMessageErreur,
                "%s() Adresse inconnue",
                foncLocale);
      codeERR = ADRESSE_INCONNUE;
      goto XIT;
    }
    /* mise à jour pour la nouvelle adresse */
    if (foncNom) {
      strcpy(Adresses[nbAdresses].idalloc, foncNom);
      strcpy(Adresses[nbAdresses].idliber, "");
    } else {
      strcpy(Adresses[nbAdresses].idalloc, "<inconnue>");
      strcpy(Adresses[nbAdresses].idliber, "");
    }
    if (varNom) strcpy(Adresses[nbAdresses].idvar, varNom);
    else strcpy(Adresses[nbAdresses].idvar, "<nom inconnu>");
    Adresses[nbAdresses].ptr = mem;
    Adresses[nbAdresses].lg = sz;
    Adresses[nbAdresses].lgact = sz;
  }
  alloue = realloc(*mem, sz);
  if (alloue != NULL) {
    *mem = alloue;
    /*    memset (alloue, 0, sz); */
    if (flagGereAdresses) {
      /* mise à jour des tables d'adresse pour la nouvelle adresse allouée */
      LaMemoire.cum += sz;
      LaMemoire.all += sz;
      LaMemoire.max = (LaMemoire.all > LaMemoire.max) ?
                      LaMemoire.all : LaMemoire.max;
      LaMemoire.last = sz;
      LaMemoire.adress = alloue;
      Adresses[nbAdresses].adresse = alloue;
      Adresses[nbAdresses].statut = 0;
      /* l'adresse à réallouer est mise à jour dans la table des adresses */
      if (foncNom)
        strcpy(Adresses[iAdresse].idliber, foncNom);
      else
        strcpy(Adresses[iAdresse].idliber, "<inconnue>");
      LaMemoire.adress = *mem;
      LaMemoire.all -= Adresses[iAdresse].lg;
      LaMemoire.last = -Adresses[iAdresse].lg;
      Adresses[iAdresse].lgact = 0;
      Adresses[iAdresse].statut = 0;
    }
    codeERR = 0;
  } else {
    *mem = NULL;
    if (foncMessageErreur)
      sprintf(foncMessageErreur,
              "%s() Erreur réallocation memoire",
              foncLocale);
    codeERR = MALLOC_ALLOCATION_IMPOSSIBLE;
    if (flagGereAdresses) {
      LaMemoire.last = 0;
      /* la nouvelle adresse */
      Adresses[nbAdresses].adresse = NULL;
      Adresses[nbAdresses].lg = 0;
      Adresses[nbAdresses].lgact = 0;
      Adresses[nbAdresses].statut = -1;
      /* l'adresse précédente */
      Adresses[iAdresse].adresse = NULL;
      Adresses[iAdresse].lg = 0;
      Adresses[iAdresse].lgact = 0;
      Adresses[iAdresse].statut = -1;
    }
  }
  if (flagGereAdresses) {
#if flagAffMemoire
    AffMemoire (stdout);
#endif
    nbAdresses += 1;
  }
  XIT:
  return (codeERR);
}

/*****************************************************************************/
/* Gestion de la memoire allouee dynamiquement: DESALLOCATION                */
/*****************************************************************************/
int FreeCtl(void **mem,      /* in/out */
            char *foncNom,    /* in */
            char *foncMessageErreur)  /* out */
{
  int codeERR = 0;
  int iAdresse;
  char *foncLocale = "FreeCtl";

  if (*mem == NULL) return (0);
  if (flagGereAdresses) {
    iAdresse = GetIndiceAdresse(*mem);
    if (iAdresse == ADRESSE_INCONNUE) {
      if (foncMessageErreur)
        sprintf(foncMessageErreur,
                "%s() Adresse inconnue",
                foncLocale);
      codeERR = ADRESSE_INCONNUE;
    } else if (iAdresse == ADRESSE_DEJA_LIBRE) {
      if (foncMessageErreur)
        sprintf(foncMessageErreur,
                "%s() Adresse déjà libre",
                foncLocale);
      codeERR = ADRESSE_DEJA_LIBRE;
    }
  }
  if (!codeERR) {
    free(*mem);
    mallocCount -= 1;
    if (flagGereAdresses) {
      if (foncNom)
        strcpy(Adresses[iAdresse].idliber, foncNom);
      else
        strcpy(Adresses[iAdresse].idliber, "<inconnue>");
      LaMemoire.adress = *mem;
      LaMemoire.mallocCount -= 1;
      LaMemoire.all -= Adresses[iAdresse].lg;
      LaMemoire.last = -Adresses[iAdresse].lg;
      Adresses[iAdresse].lgact = 0;
      Adresses[iAdresse].statut = 0;
    }
    *mem = NULL;
  }
  if (flagGereAdresses) {
#if flagAffMemoire
    AffMemoire (stdout);
#endif
  }
  return (codeERR);
}

/*****************************************************************************/
/* Affichage du solde d'appels malloc/free				     */
/*****************************************************************************/
int GetMemoryCount(void) {
  return mallocCount;
}

/*****************************************************************************/
/* Affichage des compteurs de memoire allouee				     */
/*****************************************************************************/
int AffMemoire(FILE *fd) {
  fprintf(fd, "ALLOC  Derniere operation memoire:\n");
  if (LaMemoire.last > 0)
    fprintf(fd, "ALLOC    Allocation memoire adresse: %08x\n",
            LaMemoire.adress);
  else
    fprintf(fd, "ALLOC    Desallocation memoire adresse: %08x\n",
            LaMemoire.adress);
  fprintf(fd, "ALLOC    Memoire allouee en dernier:  %10d\n", LaMemoire.last);
  fprintf(fd, "ALLOC  Statistiques memoire:\n");
  fprintf(fd, "ALLOC    Memoire allouee cumulee:     %10d\n", LaMemoire.cum);
  fprintf(fd, "ALLOC    Memoire max allouee:         %10d\n", LaMemoire.max);
  fprintf(fd, "ALLOC  Solde memoire:\n");
  fprintf(fd, "ALLOC    Memoire encore allouee:      %10d\n", LaMemoire.all);
  return (0);
}

/************************************************************************/
/* Affichage des adresses allouee					*/
/* avec ou sans suppression des adresses inutilisées ensuite		*/
/************************************************************************/
int AffAdresses(FILE *fd, int gc, int entete) {
  int i;
  static int init = 0;
  int ret = 0;
  if (flagGereAdresses) {
    if (fd != NULL) {
      if (init == 0) {
        init = 1;
        EcritEntete(fd);
      } else if (entete) {
        EcritEntete(fd);
      }
      /* separateurs ecrits a chaque passage */
      EcritSeparateurs(fd);
      for (i = 0; i < nbAdresses; i++) {
        fprintf(fd, FMT_PTRx, Adresses[i].ptr);
        fprintf(fd, "|");
        fprintf(fd, FMT_VARIABLE, Adresses[i].idvar);
        fprintf(fd, "|");
        fprintf(fd, FMT_ADRESSEx, Adresses[i].adresse);
        fprintf(fd, "|");
        fprintf(fd, FMT_TAILLEd, Adresses[i].lg);
        fprintf(fd, "|");
        fprintf(fd, FMT_ACTUELLEd, Adresses[i].lgact);
        fprintf(fd, "|");
        fprintf(fd, FMT_ALLOC, Adresses[i].idalloc);
        fprintf(fd, "|");
        fprintf(fd, FMT_DESALLOC, Adresses[i].idliber);
        fprintf(fd, "|");
        fprintf(fd, FMT_STATUT,
                (Adresses[i].statut == statutOK) ? LibstatutOK : LibstatutKO);
        fprintf(fd, "\n");
      }
      if (gc == GC_OK) {
        if (ret = GarbageCollection()) return (ret);
      }
    }
  }
  return (0);
}

/************************************************************************/
/* Ecriture de l'entete							*/
/************************************************************************/
static int EcritEntete(FILE *fd) {
  fprintf(fd, FMT_PTR, "Ptr");
  fprintf(fd, "|");
  fprintf(fd, FMT_VARIABLE, "Variable");
  fprintf(fd, "|");
  fprintf(fd, FMT_ADRESSE, "Adresse");
  fprintf(fd, "|");
  fprintf(fd, FMT_TAILLE, "Taille");
  fprintf(fd, "|");
  fprintf(fd, FMT_ACTUELLE, "Actuelle");
  fprintf(fd, "|");
  fprintf(fd, FMT_ALLOC, "Allocation par");
  fprintf(fd, "|");
  fprintf(fd, FMT_DESALLOC, "Liberation par");
  fprintf(fd, "|");
  fprintf(fd, FMT_STATUT, "St");
  fprintf(fd, "\n");
  return (0);
}

/************************************************************************/
/* Ecriture de l'entete							*/
/************************************************************************/
static int EcritSeparateurs(FILE *fd) {
  fprintf(fd, FMT_PTR, "---");
  fprintf(fd, "|");
  fprintf(fd, FMT_VARIABLE, "--------");
  fprintf(fd, "|");
  fprintf(fd, FMT_ADRESSE, "-------");
  fprintf(fd, "|");
  fprintf(fd, FMT_TAILLE, "------");
  fprintf(fd, "|");
  fprintf(fd, FMT_ACTUELLE, "--------");
  fprintf(fd, "|");
  fprintf(fd, FMT_ALLOC, "--------------");
  fprintf(fd, "|");
  fprintf(fd, FMT_DESALLOC, "--------------");
  fprintf(fd, "|");
  fprintf(fd, FMT_STATUT, "--");
  fprintf(fd, "\n");
  return (0);
}

/************************************************************************/
/* GARBAGE COLLECTION dans le tableau des adresses			*/
/* On enleve toutes les entrees qui n'ont plus de memoire allouee	*/
/************************************************************************/
static int GarbageCollection(void) {
  int i;
  typAdresse tmpAdresses[N_ADRESSES];
  int tmpNbAdresses = 0;

  if (!flagGereAdresses) return (0);
  memset(tmpAdresses, 0, sizeof tmpAdresses);
  for (i = 0; i < nbAdresses; i++) {
    if (Adresses[i].lgact > 0) {
      tmpAdresses[tmpNbAdresses] = Adresses[i];
      tmpNbAdresses += 1;
    }
  }
  for (i = 0; i < tmpNbAdresses; i++) {
    Adresses[i] = tmpAdresses[i];
  }
  nbAdresses = tmpNbAdresses;
  /* on ne peut pas liberer plus de memoire, donc on ne peux plus gerer
     les adresses */
  if (nbAdresses >= LIM_ADRESSES) {
    flagGereAdresses = 0;
    return (GARBAGE_COLLECTION_IMPOSSIBLE);
  }
  return (0);
}

/************************************************************************/
/* Retourne l'indice d'une variable dans le tableau des adresses	*/
/* L'adresse en question doit correspondre a une zone memoire		*/
/* effectivement allouee						*/
/************************************************************************/
static int GetIndiceAdresse(void *mem) {
  int i;
  int rc = 0;
  int indice = ADRESSE_INCONNUE;
  int lgact = -1;
  for (i = 0; i < nbAdresses; i++) {
    if (mem == Adresses[i].adresse && Adresses[i].statut == 0) {
      /* si une adresse apparait plusieurs fois dans le tableau,
	 on conserve la ligne qui est encore allouée si elle existe */
      if (Adresses[i].lgact > lgact) {
        indice = i;
        lgact = Adresses[i].lgact;
      }
    }
  }
  if (indice == ADRESSE_INCONNUE || lgact == -1) {
    return (indice);
  } else if (lgact == 0) {
    return (ADRESSE_DEJA_LIBRE);
  } else {
    return (indice);
  }
}

/*****************************************************************************/
/* Observation des ressources				                     */
/*****************************************************************************/
int GetResource(void) {
  memset(&rlp, 0, sizeof rlp);
  errno = 0;
  if (getrlimit(RLIMIT_CPU, &rlp)) perror(NULL);
  printf("CPU:   %10d  %10d %2.2f\n",
         rlp.rlim_cur, rlp.rlim_max, (float) rlp.rlim_cur / (float) rlp.rlim_max);
  memset(&rlp, 0, sizeof rlp);
  errno = 0;
  if (getrlimit(RLIMIT_DATA, &rlp)) perror(NULL);
  printf("DATA:   %10d  %10d %2.2f\n",
         rlp.rlim_cur, rlp.rlim_max, (float) rlp.rlim_cur / (float) rlp.rlim_max);
  memset(&rlp, 0, sizeof rlp);
  errno = 0;
  if (getrlimit(RLIMIT_STACK, &rlp)) perror(NULL);
  printf("STACK:  %10d  %10d %2.2f\n",
         rlp.rlim_cur, rlp.rlim_max, (float) rlp.rlim_cur / (float) rlp.rlim_max);
  memset(&rlp, 0, sizeof rlp);
  errno = 0;
  if (getrlimit(RLIMIT_AS, &rlp)) perror(NULL);
  printf("VMEM:   %10d  %10d %2.2f\n",
         rlp.rlim_cur, rlp.rlim_max, (float) rlp.rlim_cur / (float) rlp.rlim_max);
  errno = 0;
  if (getrlimit(RLIMIT_CORE, &rlp)) perror(NULL);
  printf("VMEM:   %10d  %10d %2.2f\n",
         rlp.rlim_cur, rlp.rlim_max, (float) rlp.rlim_cur / (float) rlp.rlim_max);
  return (0);
}

