
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>	/* pour va_list */
#include <string.h>
#include <signal.h>
#include <time.h>

#include <exploit.h>	/* pour CODE_FIN_ERREUR_APPLICATIVE */

#include <unistd.h>
#include <errno.h>
int errno;


#include "inout.h"
#include "Erreur.h"
#include "Utile.h"
#include "Message.h"
#include "File32.h"

// format des noms de la série des fichiers
#define Format_fileNameExt    "%s_%03d.%s"
#define Format_fileNameNoExt  "%s_%03d"

// longueur du buffer pour les retours sous forme de char*
#define L_toStringFile32  10000

static void buildCurNames (File32* file32);
static void buildCurName (File32* file32);
static void buildFullCurName (File32* file32);
static char* buildTempCurName (File32* file32, int iseq);
static char* buildTempFullCurName (File32* file32, int iseq);
static void setNextSeqName (File32* file32);
static char* getFileNameToOpen (File32* file32);


/************************************************************************/
/* constructeur                                                         */
/************************************************************************/
File32* File32_new (void)
{
  int     ret = 0;
  File32* file32 = NULL;
  IN();
  if ((file32 = malloc(sizeof(File32))) == NULL) {
    Erreur_set (CLA_SYS, nomFonction, "Probleme d'allocation memoire");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  else {
    memset (file32, 0, sizeof (File32));
  }
  Message_printDebugMem(TAG_MALLOC,file32,1,sizeof(File32),sizeof(File32));
  OUT(ret);
  return file32;
}

//************************************************************************
// constructeur
//************************************************************************
File32* File32_recycleMem (File32* file32)
{
  int     ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  else {
    memset (file32, 0, sizeof (File32));
  }
  OUT(ret);
  return file32;
}

/************************************************************************/
/* détruit une instance de File32                                      */
/************************************************************************/
int File32_free (File32** file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  File32_close(*file32);
  Message_printDebug(TAG_FREE,"%s(), %s : 0x%x",nomFonction,"*file32",*file32);
  if (*file32) free((void**)*file32);
  *file32 = NULL;
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// Renseignements des données de l'objet File32
// fullName ou name+path indispensables
// hasSeq et maxVol doivent être renseignés ensembles
//************************************************************************
int File32_set (File32* file32      ,
                char*   fullName    ,
                char*   name        ,
                char*   path        ,
                char*   mode        ,
                int     hasSeq      ,
                long    maxVol      ,
                int     typeEOL     )
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (fullName) {
    char* ptr = NULL;
    ptr = strrchr(fullName, '/');
    strcpy (file32->fullName, fullName);
    if (ptr) {
      strcpy (file32->name, ptr+1);
      strncpy (file32->path, fullName, ptr-fullName);
      file32->path[ptr-fullName] = 0x00;
    }
    else {
      strcpy (file32->name, fullName);
      strcpy (file32->path, ".");
    }
  }
  else {
    if (path == NULL || name == NULL) {
      Erreur_set (CLA_APP, nomFonction, "Doivent être présents, soit le nom complet soit le chemin et le nom du file32");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    else {
      strcpy (file32->name, name);
      strcpy (file32->fullName, path);
      strcat (file32->fullName, "/");
      strcat (file32->fullName, name);
    }
  }
  file32->typeEOL = typeEOL;
  if (hasSeq && maxVol <= 0) {
    Erreur_set (CLA_APP, nomFonction, "Il faut spécifier un maxVol pour une séquence de fichiers");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  strcpy (file32->mode, mode);
  file32->exists = 0; // a priori
  file32->nbFetch = 0; // a priori
  file32->hasSeq = hasSeq;
  file32->seqName = (hasSeq) ? 1 : 0;
  file32->maxVol = maxVol;
  // decoupage du nom en prefixe et extension
  {
    char* ptr = NULL;
    char locName[L_fileName+L_extName+1];
    strcpy (locName, file32->name);
    ptr = strpbrk (locName, ".");
    if (ptr == NULL) {
      strcpy (file32->extName,"");
    }
    else {
      if (strlen ((ptr+1)) > L_extName)
      {
        Erreur_set (CLA_APP, nomFonction, "Extension de fichier trop longue : %s",ptr+1);
        ret = CODE_FIN_ERREUR_APPLICATIVE;
        goto XIT;
      }
      strcpy (file32->extName,ptr+1);
      *ptr = 0x00;
    }
    strcpy (file32->pfxName, locName);
  }
  buildCurNames (file32);
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
int File32_setMode (File32* file32, char* mode)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  strcpy (file32->mode, mode);
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// Construction des noms de fichiers courants
static void buildCurNames (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  buildCurName (file32);
  buildFullCurName (file32);
 XIT:
  OUT(ret);
  return;
}

//************************************************************************
// Construction du nom du fichier courant
static void buildCurName (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (file32->curName, 0, L_curFileName);
  strcpy (file32->curName, buildTempCurName (file32, file32->seqName));
 XIT:
  OUT(ret);
  return;
}

//************************************************************************
// Construction du nom complet du fichier courant
static void buildFullCurName (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (file32->fullCurName, 0, L_fullFileName);
  strcpy (file32->fullCurName, buildTempFullCurName (file32, file32->seqName));
 XIT:
  OUT(ret);
  return;
}

//************************************************************************
// Construction du nom du fichier courant à partir d'un numéro de sequence
static char* buildTempCurName (File32* file32, int iseq)
{
  int   ret = 0;
  // buffer pour les retours sous forme de char*
  static char strout[L_toStringFile32+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (strout, 0, L_toStringFile32);
  if (0 == file32->hasSeq) {
    strcpy (strout, file32->name);
  }
  else {
    if (file32->extName[0] == 0x00) {
      sprintf (strout, Format_fileNameNoExt, file32->pfxName, iseq);
    }
    else {
      sprintf (strout, Format_fileNameExt, file32->pfxName, iseq, file32->extName);
    }
  }
 XIT:
  OUT(ret);
  return strout;
}

//************************************************************************
// Construction du nom complet du fichier courant à partir d'un numéro de sequence
static char* buildTempFullCurName (File32* file32, int iseq)
{
  int   ret = 0;
  // buffer pour les retours sous forme de char*
  static char strout[L_toStringFile32+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (strout, 0, L_toStringFile32);
  strcpy (strout, file32->path);
  strcat (strout, "/");
  strcat (strout, buildTempCurName (file32, iseq));
 XIT:
  OUT(ret);
  return strout;
}

//************************************************************************
// Numéro suivant dans la sequence des noms de fichiers
static void setNextSeqName (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (file32->hasSeq) {
    file32->seqName += 1;
    buildCurNames(file32);
    file32->curFileVol = 0; // initialisation du volume du fichier suivant
  }
 XIT:
  OUT(ret);
  return;
}

//************************************************************************
// Nom effectif du fichier à ouvrir, s'il existe une séquence ou non
static char* getFileNameToOpen (File32* file32)
{
  int   ret = 0;
  char* fileNameToOpen = NULL;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  fileNameToOpen = (file32->hasSeq) ? (buildCurNames(file32), file32->fullCurName) : file32->fullName;
 XIT:
  OUT(ret);
  return fileNameToOpen;
}

//************************************************************************
// Ouverture du fichier
// - le fichier lui même
// - le premier fichier de la série s'il s'agit d'une série
//************************************************************************
int File32_open (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (0 == File32_isOpen (file32)) {  // si n'est pas déjà ouvert
    if (file32->fullName[0] == 0x00) {
      Erreur_set (CLA_APP, nomFonction, "Nom absent pour l'objet File32 %x", file32);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    if (file32->mode[0] == 0x00) {
      Erreur_set (CLA_APP, nomFonction, "Mode d'ouverture absent pour le file32 %s", file32->fullName);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    if ((file32->fd = fopen (getFileNameToOpen(file32), file32->mode)) == NULL) {
      Erreur_set (CLA_SYS, nomFonction, "Impossible d'ouvrir le file32 <%s> : %s", file32->fullName, strerror(errno));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
    // pour avoir les stats du fichier en mémoire
    if ((ret = File32_statCurName (file32)) != 0) goto XIT;
  }
 XIT:
  OUT(ret);
  return ret;
}

/************************************************************************/
/* indicateur pour savoir si le file32 est ouvert                      */
/************************************************************************/
int File32_isOpen (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    ret = 0;
  }
  else if (file32->fd == NULL) {
    ret = 0;
  }
  else {
    ret = 1;
  }
 XIT:
  OUT(ret);
  return ret;
}

/************************************************************************/
/* ferme le curseur                                                     */
/************************************************************************/
int File32_close (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (file32 && file32->fd) {
    fclose (file32->fd);
    file32->fd = NULL;
  }
 XIT:
  OUT(ret);
  return ret;
}


//************************************************************************
// ouverture du fichier suivant dans la série
//************************************************************************
int File32_openNextFile (File32* file32)
{
  int   ret     = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!file32->hasSeq) {
    Erreur_set (CLA_APP, nomFonction, "Ce n'est pas une séquence de fichier : %s", file32->curName);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (1 == File32_isOpen (file32)) {  // si est déjà ouvert
    // fermeture du fichier de sortie courant
    if ((ret = File32_close(file32)) != 0) goto XIT;
  }
  Message_print(LOG,"Fermeture du fichier : %s, volume = %ld / %f Mo, total = %lld / %f Mo",
                      file32->curName, file32->curFileVol, (float)file32->curFileVol/(float)1000000,
                      file32->totalVol, (float)file32->totalVol/(float)1000000);
  // no de seq suivant
  setNextSeqName(file32);
  // ouverture du suivant
  if ((ret = File32_open(file32)) != 0) goto XIT;
  Message_print(LOG,"Nouveau fichier de sortie courant : %s", file32->curName);
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// ramene une ligne du file32
//************************************************************************
int File32_fetch(File32* file32, char* out, int lout, int nl)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!file32->fd) {
    Erreur_set (CLA_APP, nomFonction, "file32 non ouvert");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!out) {
    Erreur_set (CLA_APP, nomFonction, "Buffer NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (lout<=0) {
    Erreur_set (CLA_APP, nomFonction, "Longueur nulle pour le buffer");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (fgets (out, lout, file32->fd) == NULL) {
    if (feof(file32->fd)) {
      ret = NO_MORE_DATA;
      goto XIT;
    }
    else {
      Erreur_set (CLA_APP, nomFonction, "Erreur de lecture sur le file32 %s", file32->fullName);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  // on enlève le car fin de ligne
  if (nl == O_NONL) {
    char* strNL = NULL;
    if (O_winNL == file32->typeEOL) {
      strNL = EOL_win;
    }
    else if (O_macNL == file32->typeEOL) {
      strNL = EOL_mac;
    }
    else {
      strNL = EOL_lnx;  // par defaut Linux
    }
    char* ptr = strstr (out, strNL);
    if (ptr) {
      *ptr = 0x00;
      if (O_winNL == file32->typeEOL || O_macNL == file32->typeEOL) *(ptr+1) = 0x00;
    }
  }

  file32->nbFetch += 1;
 XIT:
  // on ne ferme pas le fichier : il peut être reparcouru en repartant du début
  OUT(ret);
  return ret;
}


/************************************************************************/
/* revient à une ligne donnée du file32                                */
/************************************************************************/
int File32_fseek (File32* file32, long offset)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!file32->fd) {
    Erreur_set (CLA_APP, nomFonction, "file32 non ouvert");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = fseek(file32->fd, offset, SEEK_SET)) != 0) {
    Erreur_set (CLA_APP, nomFonction, "Erreur de positionnement le file32 %s [%d]", file32->fullName, ret);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return ret;
}

/************************************************************************/
/* donne la position courante dans le file32                           */
/************************************************************************/
int File32_ftell (File32* file32, long* offset)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (!file32->fd) {
    Erreur_set (CLA_APP, nomFonction, "file32 non ouvert");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((*offset = ftell(file32->fd)) <= -1) {
    Erreur_set (CLA_APP, nomFonction, "Erreur de positionnement le file32 %s [%d]", file32->fullName, ret);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// flush du buffer du file32
//************************************************************************
int File32_flush (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (file32 && file32->fd) {
    ret = fflush (file32->fd);
    if (ret) {
      Erreur_set (CLA_SYS, nomFonction, "ret=%d, %s", ret, strerror(errno));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
 XIT:
  OUT(ret);
  return ret;
}

/************************************************************************/
/* retourne le nb de lignes retournées par les fetchs depuis            */
/* l'ouverture du file32                                               */
/************************************************************************/
int File32_nbFetch (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->nbFetch;
}

//************************************************************************
// retourne une chaine de caractere qui decrit le contenu de l'objet
//************************************************************************
char* File32_toString (File32* file32)
{
  int   ret = 0;
  // buffer pour les retours sous forme de char*
  static char strout[L_toStringFile32+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (strout, 0, L_toStringFile32+1);
  sprintf (strout, "(chemin=%s) (nom=%s) (ouvert=%s) (nbFetch=%s)",
                file32->path,
                file32->name,
                (file32->fd) ? "O" : "N",
                file32->nbFetch);
 XIT:
  OUT(ret);
  return strout;
}

//************************************************************************
// ecrire dans un file32 ASCII
//************************************************************************
int File32_printFmt (File32* file32, char* fmt, ...)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  va_list ap;
  va_start (ap, fmt);
  vfprintf (file32->fd, fmt, ap);
  va_end (ap);
 XIT:
  OUT(ret);
  return ret;
}


//************************************************************************
// ecrire une ligne dans un file32 ASCII
//************************************************************************
int File32_printLine (File32* file32, char* line, int optionNL)
{
  int   ret     = 0;
  long  lineVol = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (NULL == line) {
    Erreur_set (CLA_APP, nomFonction, "\"line\" est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (0x00 == file32->fullName[0]) {
    Erreur_set (CLA_APP, nomFonction, "aucun fichier à ouvrir");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // 1ere ouverture / initialisation de la série de fichiers de sortie
  if (1 != File32_isOpen(file32)) {
    if ((ret = File32_open(file32)) != 0) goto XIT;
    Message_print(LOG,"========");
    Message_print(LOG,"Fichier de sortie courant : %s", File32_getCurName(file32));
  }
  lineVol = strlen(line);    // volume d'une ligne
  if ((file32->curFileVol+lineVol) > file32->maxVol) {
    // Ouverture d'autre fichier de sortie dans la série
    // si la limite de volume est atteinte
    if ((ret = File32_openNextFile (file32)) != 0) goto XIT;
  }
  {
    int vol = 0;
    vol += fprintf (file32->fd, "%s", line);
    file32->curFileVol  += vol;
    file32->totalVol    += vol;
    if (O_NL == optionNL) {
      if ((ret=File32_printEOL (file32)) != 0) goto XIT;
    }
  }
 XIT :
  OUT(ret);
  return ret;
}

//************************************************************************
// Fin de ligne
//************************************************************************
int File32_printEOL (File32* file32)
{
  int   ret = 0;
  int   vol = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (O_winNL == file32->typeEOL) {
    vol += fprintf (file32->fd, "%s", EOL_win);
  }
  else if (O_macNL == file32->typeEOL) {
    vol += fprintf (file32->fd, "%s", EOL_mac);
  }
  else {// linux pax defaut
    vol += fprintf (file32->fd, "%s", EOL_lnx);
  }
  file32->curFileVol  += vol;
  file32->totalVol    += vol;
  file32->totalWLines += 1;
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// Les stats du file32
//************************************************************************
int File32_statCurName (File32* file32)
{
  int   ret = 0;
  char  fullCurName[L_fullFileName+1];
  IN();
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
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (file32->curName[0] == 0x00) {
    Erreur_set (CLA_APP, nomFonction, "Nom du file32 non défini");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  errno = 0;
  if (!file32->hasSeq) {
    // pas de serie => fichier global unique
    strcpy (fullCurName, buildTempFullCurName (file32, 0));
  }
  else {
    // serie => le fichier courant
    strcpy (fullCurName, buildTempFullCurName (file32, file32->seqName));
  }
  ret = stat (fullCurName, &file32->stat);
  if (ret) {
    if (errno == ENOENT) {
      file32->exists = 0;
      ret = STAT_NO_FILE;
      goto XIT;
    }
    else {
      Erreur_set (CLA_SYS, nomFonction, "%s", strerror(errno));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  file32->curFileVol = (long) file32->stat.st_size;
  file32->exists = 1;
 XIT:
  OUT(ret);
  return ret;
}

/************************************************************************/
/* Existence du file32 sur le filesystem                               */
/************************************************************************/
int File32_exists (int* existe, long* size, char* dir, char* name)
{
  int   ret = 0;
  char  fullName[L_fullFileName+1];
  struct stat theStat;
  IN();
  memset (fullName,0,L_fullFileName+1);
  memset (&theStat,0,sizeof(struct stat));
  *existe = 0;
  if (dir) {
    strcpy(fullName,dir);
  }
  else {
    strcpy(fullName,".");
  }
  if (!name) {
    Erreur_set (CLA_APP, nomFonction, "Nom du file32 non défini");
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
      Erreur_set (CLA_SYS, nomFonction, "%s", strerror(errno));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  else {
    *existe = 1;
    *size = (long)theStat.st_size;
  }
 XIT:
  OUT(ret);
  return ret;
}


//************************************************************************
// supprime le fichier
int File32_rm (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  ret = File32_unlink (file32);
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// supprime le fichier
int File32_unlink (File32* file32)
{
  int   ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (1 == File32_isOpen(file32)) {
    if ((ret = File32_close(file32)) != 0) goto XIT;
  }
  if ((ret = File32_statCurName (file32)) != 0) goto XIT;
  if (1 == file32->exists) {
    if ((ret = unlink (file32->fullName)) != 0) {
      Erreur_set (CLA_SYS, nomFonction, strerror(ret));
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  else {
    Erreur_set (CLA_APP, nomFonction, "Le fichier %s n'existe pas", file32->fullName);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
int     File32_getNbLines (File32* file32, long *nbLines)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (1 == File32_isOpen(file32)) {
    if ((ret = File32_close(file32)) != 0) goto XIT;
  }
  // ouverture en lecture
  if ((ret = File32_setMode (file32, "r"))  != 0) goto XIT;
  if ((ret = File32_open    (file32     ))  != 0) goto XIT;
  if ((ret = File32_fseek   (file32, 0  ))  != 0) goto XIT;
  // lectures
  {
    char  lineIn[L_file32_default_line_size+1];  // pour recevoir ligne lue dans fichier
    *nbLines = 0;
    do
    {
      memset(lineIn, 0, L_file32_default_line_size+1);  // pour éviter les "bavures" d'une ligne à l'autre
      // Fetch de la ligne suivante
      ret = File32_fetch (file32, lineIn, L_file32_default_line_size, O_NONL);
      Message_printDebug (LOG, "lineIn[%ld]=\"%s\"", *nbLines, lineIn);
      if (ret != 0 && ret != NO_MORE_DATA) goto XIT;  // erreur bloquante
      if (ret != NO_MORE_DATA)
      {
        (*nbLines)++;  // toutes les lignes
      }
    } while (ret != NO_MORE_DATA);
  }
  // fermeture du fichier
  if ((ret = File32_close (file32)) != 0) goto XIT;
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// Retourne le nom du fichier
char* File32_getName (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  OUT(ret);
  return file32->name;
}

//************************************************************************
char* File32_getPath (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  OUT(ret);
  return file32->path;
}

//************************************************************************
char* File32_getMode (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  OUT(ret);
  return file32->mode;
}

//************************************************************************
// Retourne le descripteur du fichier de plus bas niveau
FILE* File32_getFd (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return NULL;
  }
  OUT(ret);
  return file32->fd;
}

//************************************************************************
int   File32_isStdout     (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->fd == stdout;
}

//************************************************************************
int   File32_isStdin      (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->fd == stdin;
}


//************************************************************************
File32* File32_getStdout (void)
{
  int ret = 0;
  static File32 file32Stdout;
  IN();
  memset(&file32Stdout,0,sizeof(File32));
  File32_set (&file32Stdout ,
              "stdout"      ,
              "stdout"      ,
              ""            ,
              "w"           ,
              O_SeqNo       ,
              FILE32_MAX_VOL,
              O_lnxNL       );
  file32Stdout.fd = stdout;
  OUT(ret);
  return &file32Stdout;
}

//************************************************************************
File32* File32_getStdin (void)
{
  int ret = 0;
  static File32 file32Stdin;
  IN();
  memset(&file32Stdin,0,sizeof(File32));
  file32Stdin.fd = stdin;
  strcpy(file32Stdin.name,"stdin");
  OUT(ret);
  return &file32Stdin;
}

//************************************************************************
char* File32_getCurName (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  OUT(ret);
  return file32->curName;
}

//************************************************************************
char* File32_getPfxName (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  OUT(ret);
  return file32->pfxName;
}

//************************************************************************
char* File32_getExtName (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  OUT(ret);
  return file32->extName;
}

//************************************************************************
int   File32_getSeqName (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->seqName;
}

//************************************************************************
long  File32_getCurFileVol (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->curFileVol;
}

//************************************************************************
long long File32_getTotalVol (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->totalVol;
}

//************************************************************************
long File32_getMaxVol (File32* file32)
{
  int ret = 0;
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return -1;
  }
  OUT(ret);
  return file32->maxVol;
}

//************************************************************************
// pour retourner les valeurs numériques en chaines
char*   File32_strgetHasSeq (File32* file32)
{
  int ret = 0;
  static char strout[30+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  memset (strout,0,30);
  sprintf(strout,"%s",(file32->hasSeq)?"oui":"non");
  OUT(ret);
  return strout;
}

//************************************************************************
// pour retourner les valeurs numériques en chaines
char*   File32_strgetSeqName (File32* file32)
{
  int ret = 0;
  static char strout[30+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  memset (strout,0,30);
  sprintf(strout,"%10d",file32->seqName);
  OUT(ret);
  return strout;
}

//************************************************************************
// pour retourner les valeurs numériques en chaines
char*   File32_strgetCurFileVol (File32* file32)
{
  int ret = 0;
  static char strout[30+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  memset (strout,0,30);
  sprintf(strout,"%10ld",file32->curFileVol);
  OUT(ret);
  return strout;
}

//************************************************************************
// pour retourner les valeurs numériques en chaines
char*   File32_strgetTotalVol (File32* file32)
{
  int ret = 0;
  static char strout[30+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  memset (strout,0,30);
  sprintf(strout,"%10lld",file32->totalVol);
  OUT(ret);
  return strout;
}


//************************************************************************
// pour retourner les valeurs numériques en chaines
char*   File32_strgetMaxVol (File32* file32)
{
  int ret = 0;
  static char strout[30+1];
  IN();
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    return "<vide>";
  }
  memset (strout,0,30);
  sprintf(strout,"%10ld",file32->maxVol);
  OUT(ret);
  return strout;
}


//************************************************************************
// Les stats de la série file32
char* File32_toStringStat (File32* file32)
{
  int         ret = 0;
  char        fullCurName[L_fullFileName+1];
  char        curName[L_curFileName+1];
  int         iseq = 0;
  // buffer pour les retours sous forme de char*
  static char strout[L_toStringFile32+1];
  char        tmpout[L_toStringFile32+1];
  #define L_timebuff  20
  char        timebuff[L_timebuff+1];
  struct stat locstat;
  IN();
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
  if (file32 == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Pointeur file32 NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset(strout,0,L_toStringFile32);
  if (!file32->hasSeq) {
    errno = 0;
    strcpy (fullCurName, buildTempFullCurName (file32, 0));
    ret = stat (fullCurName, &locstat);
    if (ret) {
      if (errno == ENOENT) {
        file32->exists = 0;
        Erreur_set(CLA_APP, nomFonction, "Le fichier no %d n'existe pas", iseq);
        ret = STAT_NO_FILE;
        goto XIT;
      }
      else {
        Erreur_set(CLA_SYS, nomFonction, "%s", strerror(errno));
        ret = CODE_FIN_ERREUR_APPLICATIVE;
        goto XIT;
      }
    }
    memset(tmpout,0,L_toStringFile32);
    memset(timebuff,0,L_timebuff);
    strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_atime));
    sprintf (tmpout,"File=%s,", fullCurName);             strcat (strout, tmpout);
    sprintf (tmpout," File mode=%ld,", locstat.st_mode);  strcat (strout, tmpout);
    sprintf (tmpout," inode=%ld,", locstat.st_ino);  strcat (strout, tmpout);
    sprintf (tmpout," device_id=%ld,", locstat.st_dev);  strcat (strout, tmpout);
    sprintf (tmpout," st_rdev=%ld,", locstat.st_rdev);  strcat (strout, tmpout);
    sprintf (tmpout," Number of links=%ld,", locstat.st_nlink);  strcat (strout, tmpout);
    sprintf (tmpout," User ID of the file's owner=%ld,", locstat.st_uid);  strcat (strout, tmpout);
    sprintf (tmpout," Group ID of the file's group=%ld,", locstat.st_gid);  strcat (strout, tmpout);
    sprintf (tmpout," File size in bytes=%ld,", locstat.st_size);  strcat (strout, tmpout);
    sprintf (tmpout," Preferred I/O block size=%ld,", locstat.st_blksize);  strcat (strout, tmpout);
    sprintf (tmpout," Number of 512 byte blocks allocated=%ld,", locstat.st_blocks);  strcat (strout, tmpout);
    strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_atime));
    sprintf (tmpout," last access=%s,",timebuff);  strcat (strout, tmpout);
    strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_mtime));
    sprintf (tmpout," last modification=%s,",timebuff);  strcat (strout, tmpout);
    strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_ctime));
    sprintf (tmpout," last file status change=%s",timebuff);  strcat (strout, tmpout);
    file32->exists |= 1;
  }
  else {
    for (iseq=1;iseq<=file32->seqName;iseq++)
    {
      errno = 0;
      strcpy (fullCurName, buildTempFullCurName (file32, iseq));
      ret = stat (fullCurName, &locstat);
      if (ret) {
        if (errno == ENOENT) {
          file32->exists = 0;
          Erreur_set(CLA_APP, nomFonction, "Le fichier no %d n'existe pas", iseq);
          ret = STAT_NO_FILE;
          goto XIT;
        }
        else {
          Erreur_set (CLA_SYS, nomFonction, "%s", strerror(errno));
          ret = CODE_FIN_ERREUR_APPLICATIVE;
          goto XIT;
        }
      }
      memset(tmpout,0,L_toStringFile32);
      memset(timebuff,0,L_timebuff);
      strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_atime));
      sprintf (tmpout,"File=%s,", fullCurName);             strcat (strout, tmpout);
      sprintf (tmpout," File mode=%ld,", locstat.st_mode);  strcat (strout, tmpout);
      sprintf (tmpout," inode=%ld,", locstat.st_ino);  strcat (strout, tmpout);
      sprintf (tmpout," device_id=%ld,", locstat.st_dev);  strcat (strout, tmpout);
      sprintf (tmpout," st_rdev=%ld,", locstat.st_rdev);  strcat (strout, tmpout);
      sprintf (tmpout," Number of links=%ld,", locstat.st_nlink);  strcat (strout, tmpout);
      sprintf (tmpout," User ID of the file's owner=%ld,", locstat.st_uid);  strcat (strout, tmpout);
      sprintf (tmpout," Group ID of the file's group=%ld,", locstat.st_gid);  strcat (strout, tmpout);
      sprintf (tmpout," File size in bytes=%ld,", locstat.st_size);  strcat (strout, tmpout);
      sprintf (tmpout," Preferred I/O block size=%ld,", locstat.st_blksize);  strcat (strout, tmpout);
      sprintf (tmpout," Number of 512 byte blocks allocated=%ld,", locstat.st_blocks);  strcat (strout, tmpout);
      strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_atime));
      sprintf (tmpout," last access=%s,",timebuff);  strcat (strout, tmpout);
      strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_mtime));
      sprintf (tmpout," last modification=%s,",timebuff);  strcat (strout, tmpout);
      strftime(timebuff, L_timebuff, "%Y-%m-%d %H:%M:%S", localtime(&locstat.st_ctime));
      sprintf (tmpout," last file status change=%s",timebuff);  strcat (strout, tmpout);
      file32->exists |= 1;
    }
  }
 XIT:
  OUT(ret);
  return strout;
}
