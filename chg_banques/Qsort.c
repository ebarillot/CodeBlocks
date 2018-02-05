#include <stdlib.h>
#include <string.h>

#include "Qsort.h"
#include "utile.h"	/* pour IN et OUT */
#include "temps.h"	/* pour Chrono */

/* #define useChrono */
#undef useChrono

/************************************************************************/
/* tri des donnees							*/
/************************************************************************/
int     Qsort (void*		data,
	       unsigned int	lgel,
	       unsigned int	nbel,
	       int		reverse,
	       int*		outIndex,
	       int 		(compare (void*, void*)))
{
  char		nomFonction[] = "Qsort";
  int		ret = 0;
  char**	localData = NULL;
  char*		ptr = NULL;
  unsigned int	i;
  unsigned int	ld = 0;
#ifdef useChrono
  Chrono*	chrono = NULL;
#endif

  IN(nomFonction);
#ifdef useChrono
  chrono = newChrono();
#endif
  /* si aucune donnée */
  if (!data || !lgel || !nbel || !compare) {
    ret = -1;
    goto XIT;
  }
  /* il y a un index (outIndex) à sortir */
  if (outIndex) {
    /* taille d'une ligne dans le tableau localData:
       une ligne de donnees en entree + un entier + 0x00 */
    ld = (lgel+sizeof (int)+1);
    /* allocation memoire */
    if ((localData = (char**) calloc (nbel, sizeof (char*))) == NULL) {
      ret = -1;
      goto XIT;
    }
    if ((ptr = (char*) calloc (nbel, ld)) == NULL) {
      ret = -1;
      goto XIT;
    }
    for (i=0; i<nbel; i++) {
      localData[i] = ptr+(i*ld);
    }
    /* preparation des donnees pour le tri */
    for (i=0; i<nbel; i++) {
      memcpy (localData[i], ((char*)data+(i*lgel)), lgel);
      /* on recopie l'indice ensuite */
      memcpy ((localData[i]+lgel), &i, sizeof (int));
      *(localData[i]+ld-1) = 0x00;
    }
#ifdef useChrono
    startChrono(chrono);
#endif
    /* tri des données */
    qsort (localData[0], nbel, ld, (int(*)(const void*, const void*))compare);
#ifdef useChrono
    afficheMessage(1,LOG,"Duree qsort() : %ld",getChronoMicroSec(chrono));
#endif
    /* on recupere les indices ordonnes selon les data */
    if (!reverse) {
      for (i=0; i<nbel; i++) {
        memcpy (&outIndex[i], (localData[i]+lgel), sizeof (int));
      }
    }
    else {
      unsigned int	j;
      for ((j=nbel-1, i=0); (j>=0, i<nbel); (j--, i++)) {
        memcpy (&outIndex[j], (localData[i]+lgel), sizeof (int));
      }
    }
  }
  /* il n'y a pas d'index (outIndex) à sortir */
  else {
    afficheMessage(2,LOG,"Sans outIndex");
    /* taille d'une ligne dans le tableau localData:
       une ligne de donnees en entree + 0x00 */
    ld = (lgel+1);
    /* allocation memoire */
    if ((localData = (char**) calloc (nbel, sizeof (char*))) == NULL) {
      ret = -1;
      goto XIT;
    }
    if ((ptr = (char*) calloc (nbel, ld)) == NULL) {
      ret = -1;
      goto XIT;
    }
    for (i=0; i<nbel; i++) {
      localData[i] = ptr+(i*ld);
    }
    /* preparation des donnees pour le tri */
    for (i=0; i<nbel; i++) {
      memcpy (localData[i], ((char*)data+(i*lgel)), lgel);
      *(localData[i]+ld-1) = 0x00;
    }
#ifdef useChrono
    startChrono(chrono);
#endif
    /* tri des données */
    qsort (localData[0], nbel, ld, (int(*)(const void*, const void*))compare);
#ifdef useChrono
    afficheMessage(1,LOG,"Duree qsort() : %.3f milli-sec",(float)topChrono(chrono)/(float)1000);
#endif
  }
  /* on recopie les donnees ordonnees dans le buffer de sortie */
  if (!reverse) {	/* dans l'ordre */
    for (i=0; i<nbel; i++) {
      memcpy (((char*)data+(i*lgel)), localData[i], lgel);
    }
  }
  else {		/* dans l'ordre inverse */
    for (i=0; i<nbel; i++) {
      memcpy (((char*)data+(i*lgel)), localData[outIndex[i]], lgel);
    }
  }
 XIT:
  /* desallocation */
  if (ptr) free (ptr);
  if (localData) free (localData);
#ifdef useChrono
  if (chrono) free(chrono);
#endif
  OUT(nomFonction,ret);
  return (ret);
}

/************************************************************************/
/* equivalent de la fonction uniq d'unix sur un tableau en memoire	*/
/* dedoublonnage, les données sont supposées ordonnées en entrant	*/
/************************************************************************/
int     Uniq (void*		data,		/* in out */
	      unsigned int	lgel,		/* in */
	      unsigned int*	nbel,		/* in out */
	      int*		outIndex)	/* out */
{
  char		nomFonction[] = "Uniq";
  char**	localData = NULL;
  char*		localDataPrec = NULL;
  char*		ptr = NULL;
  unsigned int	i;
  unsigned int	iq;
  unsigned int	ld = 0;
  int		ret = 0;

  IN(nomFonction);
  /* si aucune donnée */
  if (!data || !lgel || !nbel || !*nbel) return (-1);
  /* taille d'une ligne dans le tableau localData:
     une ligne de donnees en entree + 0x00 */
  ld = (lgel+1);
  /* allocation memoire */
  if ((localDataPrec = (char*) calloc (lgel, sizeof (char))) == NULL) {
    ret = -1;
    goto XIT;
  }
  if ((localData = (char**) calloc (*nbel, sizeof (char*))) == NULL) {
    ret = -1;
    goto XIT;
  }
  if ((ptr = (char*) calloc (*nbel, ld)) == NULL) {
    ret = -1;
    goto XIT;
  }
  for (i=0; i<*nbel; i++) {
    localData[i] = ptr+(i*ld);
  }
  /* on recopie les données dans localData[] */
  for (i=0; i<*nbel; i++) {
    memcpy (localData[i], ((char*)data+(i*lgel)), lgel);
    *(localData[i]+ld-1) = 0x00;
  }
  /* on dedoublonne */
  iq = 0;	/* indice dans tableau dedoublonné */
  localDataPrec[0] = 0x00;
  for (i=0; i<*nbel; i++) {
    if (strcmp (localData[i], localDataPrec) != 0) {
      if (!outIndex) memcpy (((char*)data+(iq*lgel)), localData[i], lgel);
      iq++;
      strcpy (localDataPrec, localData[i]);
    }
    else {
      /* c'est un doublon */
      if (outIndex) outIndex[i] = 1;
    }
  }
  /* le nb d'éléments dédoublonnés */
  *nbel = iq;
 XIT:
  /* desallocation */
  if (ptr) free (ptr);
  if (localData) free (localData);
  if (localDataPrec) free (localDataPrec);
  OUT(nomFonction,ret);
  return (ret);
}
