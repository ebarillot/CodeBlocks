/************************************************************************/
/*    ooo___                                                            */
/*   o  /o  )                                                           */
/*   o   o--\   TELEMATIQUE - CENTRE INFORMATIQUE - ROCHECORBON         */
/*    ooo    \                                                          */
/************************************************************************/
/* NOM        :                                                         */
/* TYPE       :                                                         */
/* PROJET     : ORDIRECT/MKF1	                                        */
/************************************************************************/
/* OBJET : recueil de fonctions à but utilitaire			*/
/************************************************************************/
/*  ____________________________________________________________________*/
/* |        |         |            |                                   |*/
/* |  DATE  | Version |  Auteur    | Commentaires                      |*/
/* |--------|---------|------------|-----------------------------------|*/
/* |10/12/02|   01    | E.BARILLOT | Création                          |*/
/* |08/10/04|   01.1  | E.BARILLOT | Modif afficheMessage()            |*/
/* |        |         |            | Suppression des \n                |*/
/*  ------------------------------------------------------------------- */
/*                                                                      */
/************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>	/* pour struct pm */
#include <stdarg.h>	/* pour gérer les va_list */

#include <exploit.h>	/* TestEcriture(), Log(), Print() */

#include "Cerr.h"
#include "utile.h"
#include "mallocCtl.h"

#include <traceTP.h>	/* pour traceTP */


static int	niveauTrace;

/************************************************************************/
/* met le niveau de trace						*/
/************************************************************************/
int initNiveauTrace (int niveau)
{
  niveauTrace = niveau;
  if (niveauTrace < 0 ) niveauTrace = 1;
  init_traceTP (stdout, niveauTrace);
  return 0;
}

/************************************************************************/
/* retourne le niveau de trace						*/
/************************************************************************/
int getNiveauTrace ()
{
  niveauTrace = get_niveau_traceTP();
  return niveauTrace;
}

/************************************************************************/
/* Message dans stdout et Log						*/
/************************************************************************/
int afficheMessage (int niveau, char* tag, char* fmt, ...)
{
  char	nomFonction[] = "afficheMessage";
  niveauTrace = get_niveau_traceTP();
  if (niveau <= niveauTrace) {
    va_list ap;
    char avancement[L_afficheMessage+1];
    va_start (ap, fmt);
    vsprintf (avancement, fmt, ap);
    va_end (ap);
    /* 2004/06/08 E.Barillot
     * on remplace les \n dans la chaine par des blancs de façon à éviter
     * les problèmes de format du log dont le premier champ de chaque ligne
     * doit être une date */
    {
      char* ptr = avancement;
      while (*ptr && ptr <= (avancement+L_afficheMessage)) {
	if (*ptr == '\n') {
	  *ptr = ' ';
	}
	ptr++;
      }
    }
    traceTP(niveau,"%s|%s", tag, avancement);
    /* fflush(NULL); */
  }
  return 0;
}

/************************************************************************/
/* Test d'ecriture dans les fichiers (LOG et sortie notamment)		*/
/************************************************************************/
int flushEcritures (void)
{
  char	nomFonction[] = "flushEcritures";
  int	ret = 0;
  char	message[L_messageErreur+1];
  if ((ret = TestEcriture (message)) != 0) {
    keepError(CLA_APP, nomFonction,
	      "Erreur d'écriture ret = %d (file system full?) %s", ret, message);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  return ret;
}

/************************************************************************/
/* on enlève les blancs de droite					*/
/************************************************************************/
char* myRtrim (char* str)
{
  long len = strlen (str);
  char* ptr = NULL;
  ptr = &str[len-1];
  while (*ptr == ' ') {
    *ptr = 0x00;
    ptr--;
  }
  return str;
}

/************************************************************************/
/* on enlève les blancs de gauche					*/
/************************************************************************/
char* myLtrim (char* str)
{
  long len = 0;
  char* ptr = NULL;
  ptr = str;
  while (*ptr == ' ') {
    ptr++;
  }
  len = strlen (ptr);
  strncpy(str,ptr,len);
  str[len] = 0x00;
  return str;
}

/*****************************************************************************/
/* Rend inactif les éventuels 'kill' le temps de la MAJ                      */
/*****************************************************************************/
void masquerSignaux (void)
{
  sigset_t s;
  memset (&s, 0xFF, sizeof(s));
  sigprocmask (SIG_SETMASK, &s, NULL);
}

/*****************************************************************************/
/* Rend actif les éventuels 'kill' le temps de la MAJ                        */
/*****************************************************************************/
void demasquerSignaux (void)
{
  sigset_t s;
  memset (&s, 0, sizeof(s));
  sigprocmask (SIG_SETMASK, &s, NULL);
}

/************************************************************************/
/* retourne la date : YYYYMMDD                                          */
/************************************************************************/
char* getAnneeMoisJour (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;

  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%Y%m%d", s_heure);
  return dateOut;
#undef L_dateOut
}

/************************************************************************/
/* retourne l'heure en chaine de caracteres				*/
/************************************************************************/
char* getHeure (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;

  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%H%M%S", s_heure);
  return dateOut;
}

/************************************************************************/
/* constructeur de chaine de caractère					*/
/************************************************************************/
char*	newString(long lg)
{
  char	nomFonction[] = "newString";
  char* chaine = NULL;
  chaine = mallocCtl(lg+1,"chaine",nomFonction);
  return chaine;
}

/************************************************************************/
/* reallocateur de chaine de caractère					*/
/************************************************************************/
char*	reallocString(char* str, long lg)
{
  char	nomFonction[] = "reallocString";
  str = reallocCtl(str,lg+1,"str",nomFonction);
  return str;
}

/************************************************************************/
/* supprime les EOL							*/
/************************************************************************/
char*        noEOL (char* str)
{
  char	nomFonction[] = "noEOL";
  char* tmpstr = NULL;
  char *ptr = NULL;
  int	i = 0;
  long	l_str = 0;
  l_str = strlen(str);
  if (tmpstr = mallocCtl(l_str,"tmpstr",nomFonction)) {
    ptr = str;
    while (*ptr != 0x00) {
      if (*ptr != '\n') {
        tmpstr[i] = *ptr;
        i++;
      }
      ptr++;
    }
    strcpy (str, tmpstr);
    freeCtl((void*)tmpstr,nomFonction);
    tmpstr = NULL;
  }
  return str;
}

/************************************************************************/
/* Convertit un pct d'une échelle de 0 à 100 vers une échelle de 0 à 1	*/
/* non destructeur pour str						*/
/************************************************************************/
char*        convPct1 (char* str)
{
  char	nomFonction[] = "convPct1";
  static char  tmpstr[20+1];
  float pct = 0;
  memset(tmpstr,0,20+1);
  if (str && *str) {
    pct = atof(str)/(float)100;
    sprintf(tmpstr,"%.3f",pct);
  }
  return tmpstr;
}

/************************************************************************/
/* Convertit une date sur 14 car en date sur 8 car			*/
/* YYYYMMDDHHMISS vers YYYYMMDD						*/
/* non destructeur pour str						*/
/************************************************************************/
char*        convDate14To8 (char* str)
{
  char	nomFonction[] = "convDate14To8";
  static char	tmpstr[30+1];
  memset(tmpstr,0,(30+1));
  if (str) {
    strcpy(tmpstr,str);
    if (strlen(tmpstr)==14) {
      tmpstr[8] = 0x00;
    }
  }
  return tmpstr;
}

/************************************************************************/
/* Convertit une date de type DD/MM/YYYY vers YYYYMMDD			*/
/* non destructeur pour str						*/
/************************************************************************/
char*        convDateSlashToYYYYMMDD (char* str)
{
  char	nomFonction[] = "convDateSlashToYYYYMMDD";
  char	tmpstr[30+1];
  static char	tmpstr2[30+1];
  memset(tmpstr,0,(30+1));
  memset(tmpstr2,0,(30+1));
  if (str) {
    strcpy(tmpstr,str);
    if (strlen(tmpstr)==10) {
      tmpstr[2] = 0x00;
      tmpstr[5] = 0x00;
      sprintf(tmpstr2,"%s%s%s",&tmpstr[6],&tmpstr[3],&tmpstr[0]);
    }
  }
  return tmpstr2;
}

/************************************************************************/
/* Convertit une date de type YYYYMMDD vers DD/MM/YYYY 			*/
/* non destructeur pour str						*/
/************************************************************************/
char*        convDateYYYYMMDDToSlash (char* str)
{
  char	nomFonction[] = "convDateYYYYMMDDToSlash";
  static char	tmpstr[30+1];
  memset(tmpstr,0,(30+1));
  if (str) {
    if (strlen(str)==8) {
      strncpy(tmpstr,&str[6],2);
      strcat(tmpstr,"/");
      strncpy(tmpstr,&str[4],2);
      strcat(tmpstr,"/");
      strncpy(tmpstr,&str[0],4);
    }
  }
  return tmpstr;
}

/************************************************************************/
/* Convertit une date de type YYYY vers YYYYMMDD			*/
/* non destructeur pour str						*/
/************************************************************************/
char*        convDateYYYYToYYYYMMDD (char* str)
{
  char	nomFonction[] = "convDateYYYYToYYYYMMDD";
  static char	tmpstr[30+1];
  memset(tmpstr,0,(30+1));
  if (str) {
    if (strlen(str)==4) {
      sprintf(tmpstr,"%s0101",str);
    }
  }
  return tmpstr;
}

/************************************************************************/
/* convertit une virgule en point dans un nombre			*/
/* destructeur pour la chaine en entrée					*/
/************************************************************************/
char* convCommaToPoint(char* in)
{
  char	nomFonction[] = "convCommaToPoint";
  char*	ptr = NULL;
  ptr = strchr(in,',');
  if (ptr) *ptr = '.';
  return in;
}

/************************************************************************/
/* supprime le point ou la virgule point dans un nombre			*/
/* non destructeur pour la chaine en entrée				*/
/************************************************************************/
char* dropComma(char* in)
{
  char	nomFonction[] = "dropComma";
  char*	ptr = NULL;
  int   i;
  static char	tmpstr[30+1];
  memset(tmpstr,0,(30+1));
  if (strlen(in) > 30) {
    return in;
  }
  ptr = in;
  i = 0;
  while (*ptr) {
    if (*ptr != '.' && *ptr != ',') {
      *(tmpstr+i) = *ptr;
      i++;
    }
    ptr++;
  }
  return tmpstr;
}

/************************************************************************/
/* Convertit un taux de tva écrit en décimal en un nombre entier	*/
/* Par ex 19,6 en 1960							*/
/* non destructeur pour la chaine en entrée				*/
/************************************************************************/
char* convTvaToInt(char* in)
{
  char	nomFonction[] = "convTvaToInt";
  static char	tmpstr[30+1];
  float	tva = 0;
  memset(tmpstr,0,(30+1));
  if (strlen(in) > 30) {
    ;
  }
  else if (strlen(in) == 0) {
    ;
  }
  else {
    tva = atof(convCommaToPoint(in))*100.0;
    sprintf(tmpstr,"%-.0f",tva);
  }
  return tmpstr;
}

/************************************************************************/
/* Convertit un taux de tva écrit comme un nombre entier en un nombre	*/
/* décimal avec une virgule						*/
/* Par ex 1960 en 19,60							*/
/* non destructeur pour la chaine en entrée				*/
/************************************************************************/
char* convTvaToComma(char* in)
{
  char	nomFonction[] = "convTvaToComma";
  static char	tmpstr[30+1];
  float	tva = 0;
  memset(tmpstr,0,(30+1));
  if (strlen(in) > 30) {
    ;
  }
  else if (strlen(in)==0) {
    ;
  }
  else {
    tva = atof(in)/100.0;
    sprintf(tmpstr,"%-.2f",tva);
    {	/* substitution du point par une virgule */
      char* ptr = NULL;
      ptr = strchr(tmpstr,'.');
      if (ptr) *ptr = ',';
    }
  }
  return tmpstr;
}

/************************************************************************/
/* Convertit un taux de tva écrit comme un nombre entier en un nombre	*/
/* décimal compris entre 0 et 1 avec un point				*/
/* Par ex 1960 en 0.196							*/
/* non destructeur pour la chaine en entrée				*/
/************************************************************************/
char* convTvaToPct1(char* in)
{
  char	nomFonction[] = "convTvaToPct1";
  static char	tmpstr[30+1];
  float	tva = 0;
  memset(tmpstr,0,(30+1));
  if (strlen(in) > 30) {
    return in;
  }
  tva = atof(in)/10000.0;
  sprintf(tmpstr,"%-.4f",tva);
  return tmpstr;
}

