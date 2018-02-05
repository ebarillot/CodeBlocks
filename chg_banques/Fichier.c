
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>	/* pour va_list */
#include <string.h>
#include <signal.h>

#include <exploit.h>	/* pour CODE_FIN_ERREUR_APPLICATIVE */

#include <errno.h>
int errno;


#include "Cerr.h"
#include "utile.h"
#include "mallocCtl.h"

#include "Fichier.h"



/************************************************************************/
/* variables globales							*/
/************************************************************************/

/************************************************************************/
/* constructeur								*/
/************************************************************************/
Fichier*	newFichier(char* foncNom)
{
  char	nomFonction[] = "newFichier";
  int	ret = 0;
  Fichier* fichier = NULL;
  IN(nomFonction);
  fichier = mallocCtl(sizeof(Fichier),"fichier",(foncNom)?foncNom:nomFonction);
  OUT(nomFonction,ret);
  return fichier;
}

/************************************************************************/
/* Renseignements des données de l'objet Fichier			*/
/************************************************************************/
int	setFichier(Fichier* fichier,
		   char*	nomComplet,
		   char*	nom,
		   char*	chemin,
		   char*	mode)
{
  char	nomFonction[] = "setFichier";
  int	ret = 0;
  if (nomComplet) {
    char* ptr = NULL;
    ptr = strrchr(nomComplet, '/');
    strcpy (fichier->nomComplet, nomComplet);
    if (ptr) {
      strcpy (fichier->nom, ptr+1);
      strncpy (fichier->chemin, nomComplet, ptr-nomComplet);
      fichier->chemin[ptr-nomComplet] = 0x00;
    }
    else {
      strcpy (fichier->nom, nomComplet);
      strcpy (fichier->chemin, ".");
    }
  }
  else {
    strcpy (fichier->nomComplet, chemin);
    strcat (fichier->nomComplet, "/");
    strcat (fichier->nomComplet, nom);
  }
  strcpy (fichier->mode, mode);
  return ret;
}

/************************************************************************/
/* Ouvre le fichier							*/
/************************************************************************/
int	ouvreFichier(Fichier* fichier)
{
  char	nomFonction[] = "ouvreFichier";
  int	ret = 0;
  if (fichier->nomComplet[0] == 0x00) {
    keepError(CLA_APP, nomFonction,
	      "Nom absent pour l'objet Fichier %x", fichier);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (fichier->mode[0] == 0x00) {
    keepError(CLA_APP, nomFonction,
	      "Mode d'ouverture absent pour le fichier %s",
	      fichier->nomComplet);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((fichier->fd = fopen (fichier->nomComplet, fichier->mode)) == NULL) {
    keepError(CLA_APP, nomFonction, "Impossible d'ouvrir le fichier %s",
	      fichier->nomComplet);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  return ret;
}


/************************************************************************/
/* ramene une ligne du fichier						*/
/************************************************************************/
int	fetchFichier(Fichier*	fichier, char*	out, int lout, int nl)
{
  char	nomFonction[] = "fetchFichier";
  int	ret = 0;
  if (!fichier) {
    keepError(CLA_APP, nomFonction, "fichier non alloué");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!fichier->fd) {
    keepError(CLA_APP, nomFonction, "fichier non ouvert");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!out) {
    keepError(CLA_APP, nomFonction, "Buffer NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (lout<=0) {
    keepError(CLA_APP, nomFonction, "Longueur nulle pour le buffer");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (fgets (out, lout, fichier->fd) == NULL) {
    if (feof(fichier->fd)) {
      ret = NO_MORE_DATA;
      goto XIT;
    }
    else {
      keepError(CLA_APP, nomFonction, "Erreur de lecture sur le fichier %s",
                fichier->nomComplet);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  /* on enlève le car fin de ligne */
  if (nl == O_NONL) {
    char* ptr = strchr (out, '\n');
    if (ptr) *ptr = 0x00;
  }

  fichier->nbFetch += 1;
 XIT:
  if (ret == NO_MORE_DATA || ret != 0) {
    int ret2 = fermeFichier(fichier);
    /* si erreur dans la fermeture du fichier, on ecrase l'erreur precedente */
    if (ret2) ret = ret2;
  }
  return ret;
}


/************************************************************************/
/* ferme le curseur							*/
/************************************************************************/
int	fermeFichier(Fichier*	fichier)
{
  char	nomFonction[] = "fermeFichier";
  int	ret = 0;
  if (fichier && fichier->fd) {
    fclose (fichier->fd);
    fichier->fd = NULL;
  }
 XIT:
  return ret;
}

/************************************************************************/
/* revient à une ligne donnée du fichier				*/
/************************************************************************/
int	fseekFichier(Fichier*	fichier, long offset)
{
  char	nomFonction[] = "fseekFichier";
  int	ret = 0;
  IN(nomFonction);
  if (!fichier) {
    keepError(CLA_APP, nomFonction, "fichier non alloué");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!fichier->fd) {
    keepError(CLA_APP, nomFonction, "fichier non ouvert");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = fseek(fichier->fd, offset, SEEK_SET)) != 0) {
    keepError(CLA_APP, nomFonction,
	      "Erreur de positionnement le fichier %s [%d]",
	      fichier->nomComplet,
	      ret);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(nomFonction,ret);
  return ret;
}

/************************************************************************/
/* donne la position courante dans le fichier				*/
/************************************************************************/
int	ftellFichier(Fichier*	fichier, long* offset)
{
  char	nomFonction[] = "ftellFichier";
  int	ret = 0;
  IN(nomFonction);
  if (!fichier) {
    keepError(CLA_APP, nomFonction, "fichier non alloué");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!fichier->fd) {
    keepError(CLA_APP, nomFonction, "fichier non ouvert");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((*offset = ftell(fichier->fd)) <= -1) {
    keepError(CLA_APP, nomFonction,
	      "Erreur de positionnement le fichier %s [%d]",
	      fichier->nomComplet,
	      ret);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(nomFonction,ret);
  return ret;
}

/************************************************************************/
/* ferme le curseur							*/
/************************************************************************/
int	flushFichier(Fichier*	fichier)
{
  char	nomFonction[] = "flushFichier";
  int	ret = 0;
  if (fichier && fichier->fd) {
    fflush (fichier->fd);
  }
 XIT:
  return ret;
}

/************************************************************************/
/* détruit une instance de Fichier					*/
/************************************************************************/
int	freeFichier(Fichier*	fichier)
{
  char	nomFonction[] = "freeFichier";
  int	ret = 0;
  fermeFichier(fichier);
  if (fichier && fichier->ctu) freeAstring(fichier->ctu);
  if (fichier) freeCtl((void*)fichier,nomFonction);
 XIT:
  return ret;
}

/************************************************************************/
/* détruit une instance de Fichier sauf le contenu en memoire		*/
/* et retourne le buffer sur ce contenu					*/
/************************************************************************/
Astring*	freeFichierButCtu(Fichier*	fichier)
{
  char	nomFonction[] = "freeFichierButCtu";
  Astring* ctu = NULL;

  if (fichier) ctu = fichier->ctu;
  if (fichier) fermeFichier(fichier);
  if (fichier) freeCtl((void*)fichier,nomFonction);
 XIT:
  return ctu;
}

/************************************************************************/
/* Place le contenu du fichier dans l'espace memoire dédié		*/
/************************************************************************/
int	loadCtuFichier(Fichier* fichier)
{
  char	nomFonction[] = "loadCtuFichier";
  int	ret = 0;
#define	L_ligne	10000
  char ligne_lue[L_ligne+1];

  ret = allocCtuFichier(fichier);
  if (ret) goto XIT;
  ret = ouvreFichier(fichier);
  if (ret) goto XIT;
  while (1) {
    memset(ligne_lue,0,L_ligne+1);
    if ((ret = fetchFichier (fichier, ligne_lue, L_ligne, O_NL)) != 0) break;
    setAstring(fichier->ctu,ligne_lue,O_CAT);
  }
 XIT:
  if (ret == NO_MORE_DATA) {
    ret = 0;
  }
  return ret;
#undef	L_ligne
}

/************************************************************************/
/* Attribution des espaces memoire nécessaires au stockage du contenu	*/
/* du fichier								*/
/************************************************************************/
int	allocCtuFichier(Fichier* fichier)
{
  char	nomFonction[] = "allocCtuFichier";
  int	ret = 0;
  if ((ret = statFichier(fichier)) != 0) goto XIT;
  fichier->ctu = newAstring((fichier->stat.st_size+8192), 0);
  if (!fichier->ctu) {
    keepError(CLA_APP, nomFonction,
	      "Impossible d'allouer la mémoire pour le contenu du fichier");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  return ret;
}

/************************************************************************/
/* indicateur pour savoir si le fichier est ouvert			*/
/************************************************************************/
int	estOuvertFichier(Fichier*	fichier)
{
  char	nomFonction[] = "estOuvertFichier";
  if (fichier == NULL) {
    return 0;
  }
  else if (fichier->fd == NULL) {
    return 0;
  }
  else {
    return 1;
  }
}

/************************************************************************/
/* retourne le nb de lignes retournées par les fetchs depuis		*/
/* l'ouverture du fichier						*/
/************************************************************************/
int	nbFetchFichier(Fichier*	fichier)
{
  char	nomFonction[] = "nbFetchFichier";
  return fichier->nbFetch;
}

/************************************************************************/
/* retourne une chaine de caractere qui decrit le contenu de l'objet	*/
/************************************************************************/
char*	toStringFichier(Fichier*	fichier)
{
  char	nomFonction[] = "toStringFichier";
#define	L_messageOut	1000
  static char	out[L_messageOut+1];
  memset (out, 0, L_messageOut+1);
  sprintf (out, "(chemin=%s) (nom=%s) (ouvert=%s) (nbFetch=%d)",
	   fichier->chemin,
	   fichier->nom,
	   (fichier->fd) ? "O" : "N",
	   fichier->nbFetch);
  return out;
#undef	L_messageOut
}

/************************************************************************/
/* ecrire dans un fichier ASCII						*/
/************************************************************************/
int	ecrireFichier(Fichier*	fichier, char* fmt, ...)
{
  char	nomFonction[] = "ecrireFichier";
  int	ret = 0;
  va_list ap;
  va_start (ap, fmt);
  vfprintf (fichier->fd, fmt, ap);
  va_end (ap);
  return ret;
}


/************************************************************************/
/* Les stats du fichier							*/
/************************************************************************/
int	statFichier(Fichier*	fichier)
{
  char	nomFonction[] = "statFichier";
  int	ret = 0;
#if 0
  /* signification des champs de la fonction stat */
  mode_t   st_mode;     /* File mode (see mknod(2)) */
  ino_t    st_ino;      /* Inode number */
  dev_t    st_dev;      /* ID of device containing
			 * a directory entry for this file */
  dev_t    st_rdev;     /* ID of device
			 * This entry is defined only for
			 * char special or block special files */
  nlink_t  st_nlink;    /* Number of links */
  uid_t    st_uid;      /* User ID of the file's owner */
  gid_t    st_gid;      /* Group ID of the file's group */
  off_t    st_size;     /* File size in bytes */
  time_t   st_atime;    /* Time of last access */
  time_t   st_mtime;    /* Time of last data modification */
  time_t   st_ctime;    /* Time of last file status change
			 * Times measured in seconds since
			 * 00:00:00 UTC, Jan. 1, 1970 */
  long     st_blksize;  /* Preferred I/O block size */
  blkcnt_t st_blocks;   /* Number of 512 byte blocks allocated*/
#endif
  if (fichier == NULL) {
    keepError(CLA_APP, nomFonction, "Fichier non défini");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (fichier->nomComplet[0] == 0x00) {
    keepError(CLA_APP, nomFonction, "Nom du fichier non défini");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  errno = 0;
  ret = stat(fichier->nomComplet,&fichier->stat);
  if (ret) {
    if (errno == ENOENT) {
      fichier->exists = 0;
      ret = STAT_NO_FILE;
      goto XIT;
    }
    else {
      keepError(CLA_APP, nomFonction, "%s", strerror(errno));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  fichier->exists = 1;
 XIT:
  return ret;
}

/************************************************************************/
/* Les stats du fichier							*/
/************************************************************************/
int	existeFichier(int* existe, long* size, char* dir, char* name)
{
  char	nomFonction[] = "existeFichier";
  int	ret = 0;
  char	fullName[L_FichierNomComplet+1];
  struct stat theStat;
  IN(nomFonction);
  memset(fullName,0,L_FichierNomComplet+1);
  memset(&theStat,0,sizeof(struct stat));
  *existe = 0;
  if (dir) {
    strcpy(fullName,dir);
  }
  else {
    strcpy(fullName,".");
  }
  if (!name) {
    keepError(CLA_APP, nomFonction, "Nom du fichier non défini");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  /* eventuellement ajouter un test sur la presence du / deja */
  strcat(fullName, "/");
  strcat(fullName, name);
  errno = 0;
  ret = stat(fullName, &theStat);
  if (ret) {
    *existe = 0;
    if (errno == ENOENT) {
      ret = 0;
    }
    else {
      keepError(CLA_APP, nomFonction, "%s", strerror(errno));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  else {
    *existe = 1;
    *size = (long)theStat.st_size;
  }
 XIT:
  OUT(nomFonction,ret);
  return ret;
}

/************************************************************************/
/* Retourne un pointeur sur le contenu d'un fichier			*/
/************************************************************************/
Astring*	getCtuFichier(char* nomFichier)
{
  char	nomFonction[] = "getCtuFichier";
  int	ret = 0;
  Fichier* fic = NULL;
  Astring* ctu = NULL;

  fic = newFichier(nomFonction);
  if ((ret = setFichier(fic, nomFichier, NULL, NULL, "r")) != 0) goto XIT;
  ret = loadCtuFichier(fic);
  if (ret) goto XIT;
  ctu = freeFichierButCtu(fic);
 XIT:
  if (ret) {
    freeFichier(fic);
  }
  return ctu;
#undef	L_ligne
}

/************************************************************************/
/* Retourne un pointeur sur le contenu d'un fichier			*/
/************************************************************************/
int	getFileAsSepBuffer(char** buffer,
			   long* lenout,
			   char* nomFichier,
			   char* sep)
{
  char	nomFonction[] = "getFileAsSepBuffer";
  int	ret = 0;
  long len = 0;
  Fichier* fic = NULL;
#define	L_ligne	10000
  char ligne_lue[L_ligne+1];

  IN(nomFonction);
  if (!sep) {
    keepError(CLA_APP,nomFonction,"separateur absent");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  /* objet fichier */
  fic = newFichier(nomFonction);
  if ((ret = setFichier(fic, nomFichier, NULL, NULL, "r")) != 0) goto XIT;
  /* pour avoir la taille du fichier */
  if ((ret = statFichier(fic)) != 0) {
    if (ret == STAT_NO_FILE) {
      keepError(CLA_APP,nomFonction,"Fichier %s absent",fic->nom);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
    }
    goto XIT;
  }
  /* allocation de la structure Astring qui va recevoir le contenu
   * du fichier sous la forme d'un tableau de lignes */
  fic->ctu = newAstring((fic->stat.st_size+8192), N_ptrAstring);
  if (!fic->ctu) {
    keepError(CLA_APP, nomFonction,
	      "Impossible d'allouer la mémoire pour le contenu du fichier");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = ouvreFichier(fic)) != 0) goto XIT;
  while (1) {
    memset(ligne_lue,0,L_ligne+1);
    if ((ret = fetchFichier (fic, ligne_lue, L_ligne, O_NONL)) != 0) goto XIT_WHILE;
    if ((ret = addElemAstring(fic->ctu,ligne_lue)) != 0) goto XIT_WHILE;
  }
 XIT_WHILE:
  if (ret == NO_MORE_DATA) {
    ret = 0;
  }
  else if (ret) {
    goto XIT;
  }
  len = getLenAstring(fic->ctu);
  *buffer = newString(len+(getNlinesAstring(fic->ctu)*strlen(sep)));
  if (!*buffer) {
    keepError(CLA_APP,nomFonction,"allocation impossible du buffer");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  *lenout = getStringAstring(fic->ctu,*buffer,len,sep);
  if (*lenout >= len) {
    keepError(CLA_APP,nomFonction,"buffer trop petit");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  *lenout = strlen(*buffer)+1;

 XIT:
  if (fic) freeFichier(fic);
  OUT(nomFonction,ret);
  return ret;
#undef	L_ligne
}

