#ifndef File32_H
#define File32_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Erreur.h"
#include "Message.h"

#define STAT_NO_FILE  -1

#define NO_MORE_DATA  -1

// présence ou non d'un fin de ligne
#define O_NL    1
#define O_NONL  0

// type de fin de ligne : windows ou linux
#define O_lnxNL   0
#define O_winNL   1
#define O_macNL   2

#define O_lnxNL_TXT   "lin"
#define O_winNL_TXT   "win"
#define O_macNL_TXT   "mac"

#define EOL_lnx   "\n"
#define EOL_win   "\r\n"
#define EOL_mac   "\r"

#define EOL_lnxStr   "\\n"
#define EOL_winStr   "\\r\\n"
#define EOL_macStr   "\\r"

// présence ou non d'une série de fichiers
#define O_SeqNo   0
#define O_SeqWith 1

// volume max (en octets) du fichier BAD
#define FILE32_MAX_VOL      2000*1000000

// longueur max d'une ligne de texte
#define L_file32_default_line_size    10000

#define L_filePath          1000
#define L_fileName          500
#define L_extName           100
#define L_fullFileName      L_filePath+L_fileName
#define L_curFileName       L_fileName+L_extName
#define L_fileMode          10

typedef struct {
  char          fullName[L_fullFileName+1];
  char          name[L_fileName+1];
  char          path[L_filePath+1];
  char          mode[L_fileMode+1];
  int           exists; // positionné après un appel à stat()
  int           typeEOL; // type de fin de ligne
  int           nbFetch;  // nb de lignes lues
  int           totalWLines; // nb de lignes ecrites
  int           hasSeq;     // est une série de fichiers
  int           seqName;              // No sequence pour la serie de File32s
  long          maxVol;               // volume max d'un File32 de sortie
  // champs utiles pour une série de fichiers
  char          fullCurName[L_fullFileName+1];  // nom complet fichier courant (avec chemin)
  char          curName[L_curFileName+1];// nom File32 courant dans la série, sans le chemin, avec extension
  char          pfxName[L_fileName+1];// préfixe du nom de File32
  char          extName[L_extName+1]; // extension
  long          curFileVol;           // vol courant du File32
  long long     totalVol;             // vol total de la serie de File32s, long long pour dépasser 2^32
  // structures de plus bas niveau
  FILE*         fd;
  struct stat   stat;
} File32;


File32* File32_new        (void);
File32* File32_recycleMem (File32* file32);
int     File32_free       (File32**  file32);
int     File32_set        (File32* file32       ,
                            char*   fullName    ,
                            char*   name        ,
                            char*   path        ,
                            char*   mode        ,
                            int     hasSeq      ,
                            long    maxVol      ,
                            int     typeEOL     );
int     File32_setMode    (File32* file32, char* mode);
int     File32_open       (File32* file32);
int     File32_openNextFile (File32* file32);
int     File32_isOpen     (File32* file32);
int     File32_close      (File32* file32);
int     File32_fetch      (File32* file32, char* out, int lout, int nl);
int     File32_fseek      (File32* file32, long  offset);
int     File32_ftell      (File32* file32, long* offset);
int     File32_nbFetch    (File32* file32);
char*   File32_toString   (File32* file32);
int     File32_printFmt   (File32* file32, char* fmt, ...);
int     File32_printLine  (File32* file32, char* line, int optionNL);
int     File32_printEOL   (File32* file32);
int     File32_statCurName (File32* file32);
int     File32_flush      (File32* file32);
int     File32_exists     (int* exist, long* size, char* dir, char* name);
int     File32_rm         (File32* file32);
int     File32_unlink     (File32* file32);


// retourne le nb de lignes du fichier (tient compte du caractere de fin de ligne)
int     File32_getNbLines (File32* file32, long *nbLines);

// retourne le nom du fichier, chemin ...
char*   File32_getName    (File32* file32);
char*   File32_getPath    (File32* file32);
char*   File32_getMode    (File32* file32);
// retourne le descripteur de niveau FILE, donc brise l'encapsulation File32 ...
FILE*   File32_getFd      (File32* file32);

// retourne une enveloppe (wrapping) de stdout dans un File32
File32* File32_getStdout  (void);
File32* File32_getStdin   (void);

// pour vérifier si le File32 est une enveloppe de stdin ou stdout
int     File32_isStdout   (File32* file32);
int     File32_isStdin    (File32* file32);

// pour avoir des infos la série de fichiers (nombre, volume)
char*   File32_getCurName     (File32* file32);
char*   File32_getPfxName     (File32* file32);
char*   File32_getExtName     (File32* file32);
int     File32_getSeqName     (File32* file32);
long    File32_getCurFileVol  (File32* file32);
long long File32_getTotalVol  (File32* file32);
long    File32_getMaxVol      (File32* file32);


// pour retourner les valeurs numériques en chaines
char*   File32_strgetHasSeq     (File32* file32);
char*   File32_strgetSeqName    (File32* file32);
char*   File32_strgetCurFileVol (File32* file32);
char*   File32_strgetTotalVol   (File32* file32);
char*   File32_strgetMaxVol     (File32* file32);
char*   File32_toStringStat     (File32* file32);

#endif
