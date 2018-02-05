/************************************************************************/
/* OBJET : recueil de fonctions à but utilitaire			*/
/************************************************************************/
/*  ____________________________________________________________________*/
/* |        |         |            |                                   |*/
/* |  DATE  | Version |  Auteur    | Commentaires                      |*/
/* |--------|---------|------------|-----------------------------------|*/
/* |10/12/02|   01    | E.BARILLOT | Création                          |*/
/*  ------------------------------------------------------------------- */
/*                                                                      */
/************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>       // pour struct pm
#include <sys/time.h>   // gettimeofday()
#include <stdarg.h>     // pour gérer les va_list

#include <exploit.h>    // TestEcriture(), Log(), Print()

#include "Utile.h"
#include "Erreur.h"
#include "Message.h"
#include "inout.h"


//*****************************************************************************
// Rend inactif les éventuels 'kill' le temps de la MAJ
//*****************************************************************************
void Utile_masquerSignaux (void)
{
  int   ret = 0;
  IN();
  sigset_t s;
  memset (&s, 0xFF, sizeof(s));
  sigprocmask (SIG_SETMASK, &s, NULL);
  OUT(ret);
}

//*****************************************************************************
// Rend actif les éventuels 'kill' le temps de la MAJ
//*****************************************************************************
void Utile_demasquerSignaux (void)
{
  int   ret = 0;
  IN();
  sigset_t s;
  memset (&s, 0, sizeof(s));
  sigprocmask (SIG_SETMASK, &s, NULL);
  OUT(ret);
}

//************************************************************************
// Test d'ecriture dans les fichiers (LOG et sortie notamment)
//************************************************************************
int Utile_flushEcritures (void)
{
  int   ret = 0;
  char  message[L_messageErreur+1];
  IN();
  if (TestEcriture (message) != 0) {
    Erreur_set(CLA_SYS,nomFonction, "Erreur d'écriture (file system full?) %s", message);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  OUT(ret);
  return ret;
}

//************************************************************************
// flush des buffers du processus appelant
// Fait la même chose que ci-dessus, mais sans utiliser TestEcriture()
//************************************************************************
int Utile_flushAll (void)
{
  int   ret = 0;
  IN();
  ret = fflush (NULL);
  if (ret) {
    Erreur_set (CLA_SYS, nomFonction, "ret=%d, %s", ret, strerror(errno));
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  OUT(ret);
  return ret;
}


//************************************************************************
// on enlève les blancs de droite
// en sortie la chaine str est modifiée
//************************************************************************
void Utile_rtrim (char* str)
{
  size_t  len = 0;
  char* ptr = str;
  int   ret = 0;
  IN();
  if (str == NULL) return;
  len = strlen (str);
  ptr += len-1;
  while (ptr > str && *ptr == ' ') {
    *ptr = 0x00;
    ptr--;
  }
  OUT(ret);
  return;
}

//************************************************************************
// on complète à gauche par le caractère 'c'
// jusqu'à atteindre la longueur len
//************************************************************************
void Utile_lpad (char* str, int c, int len)
{
  int   lenstr  = 0;
  char* tmpstr  = NULL;
  int   ret     = 0;
  IN();
  if (str == NULL) return;
  lenstr = strlen (str);
  if (len <= lenstr) return;
  tmpstr = malloc (len*sizeof(char));
  memset (tmpstr,0,len*sizeof(char));
  Message_printDebug("MALLOC","%s(), tmpstr: 0x%x, %d",__func__,tmpstr,len*sizeof(char));
  int i;
  for (i=0;i<(len-lenstr);i++) {
    tmpstr[i] = (char)c;
  }
  i = strlen(tmpstr);
  int j;
  for (j=0;j<lenstr;j++) {
    tmpstr[i+j] = str[j];
  }
  strncpy (str, tmpstr, (size_t)len);
  str[len] = 0x00;
  Message_printDebug(TAG_FREE,"%s(), tmpstr: %x",nomFonction,tmpstr);
  if (tmpstr) free(tmpstr);
  OUT(ret);
  return;
}

//************************************************************************
// on enlève tous les blancs de la chaine str
// en sortie la chaine str est modifiée
//************************************************************************
void Utile_supspace (char* str)
{
  char* i = NULL;
  char* j = NULL;
  int   ret = 0;
  IN();
  i = &str[0];
  j = &str[0];
  while (*i) {
    *j = *i;
    if (*j != ' ') j++;
    i++;
  }
  *j = 0x00;  // pour tronquer les caracteres résiduels dans str
  OUT(ret);
  return;
}

//************************************************************************
// constructeur de chaine de caractère
//************************************************************************
char* Utile_newString(long lg)
{
  int ret = 0;
  char* chaine = NULL;
  IN();
  chaine = malloc(lg+1);
  if (chaine) memset (chaine, 0, lg+1);
  Message_printDebug("MALLOC","%s(), chaine: 0x%x, %d",__func__,chaine,lg+1);
  OUT(ret);
  return chaine;
}

//************************************************************************
// reallocateur de chaine de caractère
//************************************************************************
char* Utile_reallocString(char* str, long lg)
{
  int   ret = 0;
  IN();
  str = realloc(str, (size_t)(lg+1));
  OUT(ret);
  return str;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Isole un champ csv dans une chaine, à partir de la position *ptr1
// Renvoie le valeur dans tokenOut
int String_nextToken (  char*   tokenOut    // valeur du champ isolé, en sortie de fonction
                      , int     tokenOutMax // longueur max du receptacle tokenOut
                      , short*  ind         // indicateur de presence de valeur
                      , int*    no_champ    // numéro du champ, incrémenté à chaque appel
                      , char**  ptr1        // pointeur au début d'un champ à isoler dans une chaine
                                            // modifié en sortie, pointe sur le début du champ suivant
                      , char*   csvsep)     // séparateur csv
{
  IN();
  int   ret  = 0;
  char* ptr2 = NULL;
  int csvseplen = strlen(csvsep);
  (*no_champ)++;
  ptr2 = strstr(*ptr1,csvsep);  // on cherche le séparateur suivant
  if (ptr2 == NULL) { // pas trouvé de séparateur => ptr1 pointe sur le dernier champ
    ptr2 = *ptr1 + strlen (*ptr1);  // ptr2 pointe sur la fin de la ligne car elle ne se termine pas par un separateur
    // ATTENTION, en faisant ainsi, on ne peut pas détecter le fait qu'il n'y a plus de champ à traiter
    // Il faudrait pouvoir comparer le nb de separateurs déja rencontrés et le nombre de champs déja traités
    // or il n'y a qu'un seul compteur no_champ
  }
  if (ptr2 >= *ptr1)  // le dernier champ peut être vide
  {
    if ((ptr2-*ptr1) > (tokenOutMax)) {
      Erreur_set(CLA_APP,nomFonction,"Valeur reelle > taille max pour champ %d", *no_champ);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
             *ind = (short)((ptr2 == *ptr1)?-1:0);
    strncpy (tokenOut , *ptr1, ptr2-*ptr1);
             tokenOut [ptr2-*ptr1] = 0x00;
  }
  else {  // ne devrait logiquement jamais arriver
    Erreur_set(CLA_APP,nomFonction,"Pb parsing champ %d", *no_champ);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // tant qu'on n'est pas sur la fin de ligne
  // on passe au début du champ suivant
  *ptr1 = (*ptr2 != 0x00) ? ptr2+csvseplen : ptr2;
 XIT:
  OUT(ret);
  return (ret);
}


/////////////////////////////////////////////////////////////////////////////////////////
// Convertit un token en type long
int Token_toLong (long* field_N, char* tokenIn, short* ind)
{
  int ret = 0;
  *ind = -1;
  Utile_errno_reset();
  if (!(tokenIn == NULL || tokenIn[0] == 0x00)) {
    *field_N = (long) strtol(tokenIn, (char**)NULL, 10);
    if (Utile_errno_test()) goto XIT;
    *ind = 0;
  }
 XIT:
  return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Convertit un token en type long long
int Token_toLongLong (long long* field_N, char* tokenIn, short* ind)
{
  int ret = 0;
  *ind = -1;
  Utile_errno_reset();
  if (!(tokenIn == NULL || tokenIn[0] == 0x00)) {
    *field_N = (long long) strtoll(tokenIn, (char**)NULL, 10);
    if (Utile_errno_test()) goto XIT;
    *ind = 0;
  }
 XIT:
  return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Convertit un token en type char* (pour la beauté du geste :)
int Token_toChar (char* fieldOut, int fieldOutMax, char* tokenIn, short* ind)
{
  int ret = 0;
  *ind = -1;
  if (!(tokenIn == NULL || tokenIn[0] == 0x00)) {
    strncpy(fieldOut, tokenIn, (size_t)fieldOutMax);
    *ind = 0;
  }
 XIT:
  return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
// test de la errno produite par les fonctions de conversion entre chaines et numériques
int Utile_errno_test(void)
{
  int ret = 0;
  IN();
  switch (errno) {
    case EINVAL :
      Erreur_set(CLA_APP,nomFonction,"La base donnée n'est pas supportée");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      break;
    case ERANGE :
      Erreur_set(CLA_APP,nomFonction,"La chaîne n'était pas dans l'intervalle acceptable, la valeur a été arrondie");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      break;
    case 0 :
      break;
    default :
      Erreur_set(CLA_APP,nomFonction,"Erreur inconnue [%d]",errno);
      break;
  }
  Utile_errno_reset();
  OUT(ret);
  return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Remise à 0 de errno
// A utiliser avant d'appeler une fonction de conversion strtol() par exemple
int Utile_errno_reset(void)
{
  IN();
  int ret = 0;
  errno = 0;
  OUT(ret);
  return ret;
}


//************************************************************************
// supprime les EOL
//************************************************************************
char* Utile_noEOL (char* str)
{
  int   ret = 0;
  char* tmpstr = NULL;
  char* ptr = NULL;
  int   i = 0;
  size_t l_str = 0;
  IN();
  l_str = strlen(str);
  if ((tmpstr = malloc(l_str)) == NULL) {
    ptr = str;
    while (*ptr) {
      if (*ptr != '\n') {
        tmpstr[i] = *ptr;
        i++;
      }
      ptr++;
    }
    strcpy (str, tmpstr);
    Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"tmpstr",tmpstr);
    free((void*)tmpstr);
    tmpstr = NULL;
  }
  Message_printDebugMem(TAG_MALLOC,tmpstr,1,l_str,l_str);
  OUT(ret);
  return str;
}

//************************************************************************
// Convertit un pct d'une échelle de 0 à 100 vers une échelle de 0 à 1
// non destructeur pour str
//************************************************************************
char* Utile_convPct1 (char* str)
{
  static char tmpstr[20+1];
  double      pct = 0;
  IN();
  memset(tmpstr,0,20+1);
  if (str && *str) {
    pct = atof(str)/(double)100;
    sprintf(tmpstr,"%.3f",pct);
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// convertit une virgule en point dans un nombre
// destructeur pour la chaine en entrée
//************************************************************************
char* Utile_convCommaToPoint(char* in)
{
  int   ret = 0;
  char* ptr = NULL;
  IN();
  ptr = strchr(in,',');
  if (ptr) *ptr = '.';
  OUT(ret);
  return in;
}

//************************************************************************
// supprime le point ou la virgule dans un nombre
// non destructeur pour la chaine en entrée
//************************************************************************
char* Utile_dropComma(char* in)
{
  int   ret = 0;
  char* ptr = NULL;
  int   i;
  #define   L_tmpstr  500+1
  static char tmpstr[L_tmpstr];
  IN();
  memset(tmpstr,0,(L_tmpstr));
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
  OUT(ret);
  return tmpstr;
  #undef   L_tmpstr
}

//************************************************************************
// retourne 1 si la ligne est un commentaire
//************************************************************************
int Utile_isComment(char* in, char* tagComment)
{
  int   ret = 0;
  IN();
  if (in == NULL || tagComment == NULL) {ret = -1; goto XIT;}
  while (*in == ' ') in++;
  if ((strncmp(in,tagComment,strlen(tagComment))) == 0) {
    ret = 1;
  }
 XIT:
  OUT(ret);
  return ret;
}

//************************************************************************
// retourne la date + heure : YYYYMMDDHH24MISS
//************************************************************************
char* Utile_getDateHeure (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;
  int ret = 0;
  IN();
  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%Y%m%d%H%M%S", s_heure);
  OUT(ret);
  return dateOut;
#undef L_dateOut
}

//************************************************************************
// retourne la date tronquée : YYYYMMDD
//************************************************************************
char* Utile_truncDateHeureToYYYYMMDD (char* dateHeure)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  int ret = 0;
  IN();
  memset (dateOut, 0, L_dateOut);
  memcpy (dateOut, dateHeure, 8);
  OUT(ret);
  return dateOut;
#undef L_dateOut
}

//************************************************************************
// retourne la date : YYYYMMDD
//************************************************************************
char* Utile_getDateYYYYMMDD (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;
  int ret = 0;
  IN();
  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%Y%m%d", s_heure);
  OUT(ret);
  return dateOut;
#undef L_dateOut
}

/************************************************************************/
/* retourne la date : YYYYMMDD                                          */
/************************************************************************/
char* Utile_getAnneeMoisJour (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;
  IN();
  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%Y%m%d", s_heure);
  OUT(0);
  return dateOut;
#undef L_dateOut
}

//************************************************************************
// retourne l'heure en chaine de caracteres
//************************************************************************
char* Utile_getHeure (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;
  int ret = 0;
  IN();
  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%H%M%S", s_heure);
  OUT(ret);
  return dateOut;
}

//************************************************************************
// retourne l'heure en chaine de caracteres jusqu'à à la seconde
//************************************************************************
char* Utile_getDateSeconde (void)
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
  strftime (dateOut, 50, "%Y/%m/%d %H:%M:%S", s_heure);
  return dateOut;
}

//************************************************************************
// retourne le nb de mois depuis une date donnée
// 2014-09-16 Fonctionne aussi sur les dates au format YYYYsMMsDD
//************************************************************************
int Utile_getMonthsFromNow (char* dateYYYYMMDD)
{
  char  annee2[4+1];
  char  mois2[2+1];
  char  jour2[2+1];
  char  annee1[4+1];
  char  mois1[2+1];
  char  jour1[2+1];
  int   nbMonths = 0;
  char* date2 = NULL;
  int   ret = 0;
  IN();
  if (dateYYYYMMDD == NULL) return 0;

  if (strchr(dateYYYYMMDD,'/') == NULL)
  { // format YYYYMMDD
    strncpy(annee1, dateYYYYMMDD  , 4);  annee1[4] = 0x00;
    strncpy(mois1,  dateYYYYMMDD+4, 2);  mois1 [2] = 0x00;
    strncpy(jour1,  dateYYYYMMDD+6, 2);  jour1 [2] = 0x00;
  }
  else if (   dateYYYYMMDD[4] == '/'
           && dateYYYYMMDD[7] == '/' )
  { // format YYYY/MM/DD
    strncpy(annee1, dateYYYYMMDD  , 4);  annee1[4] = 0x00;
    strncpy(mois1,  dateYYYYMMDD+5, 2);  mois1 [2] = 0x00;
    strncpy(jour1,  dateYYYYMMDD+8, 2);  jour1 [2] = 0x00;
  }
  else {
    return -1;  // format inconnu
  }
  date2 = Utile_getDateYYYYMMDD();
  strncpy(annee2, date2  , 4);  annee2[4] = 0x00;
  strncpy(mois2,  date2+4, 2);  mois2 [2] = 0x00;
  strncpy(jour2,  date2+6, 2);  jour2 [2] = 0x00;
  #if 0
  printf("date2=%s\n", date2);
  printf("date1=%s\n", dateYYYYMMDD);
  printf("annee2=%s mois2=%s jour2=%s\n", annee2, mois2, jour2);
  printf("annee1=%s mois1=%s jour1=%s\n", annee1, mois1, jour1);
  printf("test=%d\n", ((atoi(jour2) - atoi(jour1))>0)?1:-1);
  #endif
  // la formule magique !
  //nbMonths = ((atoi(annee2) - atoi(annee1))*12) + atoi(mois2) - atoi(mois1) ;
  // jusqu'au 08/06/2015 arrondi par excès : tout mois entamé compte pour +1
  // nbMonths = ((atoi(annee2) - atoi(annee1))*12) + atoi(mois2) - atoi(mois1) + (((atoi(jour2) - atoi(jour1))>0)?1:0);
  // à partir du 08/06/2015 arrondi par défaut : un mois entamé ne compte pas pour +1; pour compter pour 1, un mois doit être complet
  nbMonths = ((atoi(annee2) - atoi(annee1))*12) + atoi(mois2) - atoi(mois1) + (((atoi(jour2) - atoi(jour1))>=0)?0:-1);
  OUT(ret);
  return nbMonths;
}

//************************************************************************
// retourne le nb de mois depuis une date donnée
// 2014-09-16 Fonctionne aussi sur les dates au format YYYYsMMsDD
//************************************************************************
int Utile_getMonthsBetween (char* dateYYYYMMDD_1, char* dateYYYYMMDD_2)
{
  char  annee2[4+1];
  char  mois2[2+1];
  char  jour2[2+1];
  char  annee1[4+1];
  char  mois1[2+1];
  char  jour1[2+1];
  int   nbMonths = 0;
  int   ret = 0;
  IN();
  if (dateYYYYMMDD_1 == NULL) return 0;
  if (dateYYYYMMDD_2 == NULL) return 0;
  if (strchr(dateYYYYMMDD_1,'/') == NULL)
  { // format YYYYMMDD
    strncpy(annee1, dateYYYYMMDD_1  , 4); annee1[4] = 0x00;
    strncpy(mois1,  dateYYYYMMDD_1+4, 2); mois1 [2] = 0x00;
    strncpy(jour1,  dateYYYYMMDD_1+6, 2); jour1 [2] = 0x00;
  }
  else if (   dateYYYYMMDD_1[4] == '/'
           && dateYYYYMMDD_1[7] == '/' )
  { // format YYYY/MM/DD
    strncpy(annee1, dateYYYYMMDD_1  , 4); annee1[4] = 0x00;
    strncpy(mois1,  dateYYYYMMDD_1+5, 2); mois1 [2] = 0x00;
    strncpy(jour1,  dateYYYYMMDD_1+8, 2); jour1 [2] = 0x00;
  }
  else {
    return -1;  // format inconnu
  }
  if (strchr(dateYYYYMMDD_2,'/') == NULL)
  { // format YYYYMMDD
    strncpy(annee2, dateYYYYMMDD_2  , 4); annee2[4] = 0x00;
    strncpy(mois2,  dateYYYYMMDD_2+4, 2); mois2 [2] = 0x00;
    strncpy(jour2,  dateYYYYMMDD_2+6, 2); jour2 [2] = 0x00;
  }
  else if (   dateYYYYMMDD_2[4] == '/'
           && dateYYYYMMDD_2[7] == '/' )
  { // format YYYY/MM/DD
    strncpy(annee2, dateYYYYMMDD_2  , 4); annee2[4] = 0x00;
    strncpy(mois2,  dateYYYYMMDD_2+5, 2); mois2 [2] = 0x00;
    strncpy(jour2,  dateYYYYMMDD_2+8, 2); jour2 [2] = 0x00;
  }
  else {
    return -1;  // format inconnu
  }
  // la formule magique !
  nbMonths = ((atoi(annee2) - atoi(annee1))*12) + atoi(mois2) - atoi(mois1);
  nbMonths = ((atoi(annee2) - atoi(annee1))*12) + atoi(mois2) - atoi(mois1) + (((atoi(jour2) - atoi(jour1))>0)?1:0);
  OUT(ret);
  return nbMonths;
}

//************************************************************************
// retourne le nb de jours depuis une date donnée
//************************************************************************
int Utile_getDaysBetween (char* dateYYYYMMDD_1, char* dateYYYYMMDD_2)
{
  char  annee2[4+1];
  char  mois2[2+1];
  char  jour2[2+1];
  char  annee1[4+1];
  char  mois1[2+1];
  char  jour1[2+1];
  int   nbDays = 0;
  if (dateYYYYMMDD_1 == NULL) return 0;
  if (dateYYYYMMDD_2 == NULL) return 0;
  strncpy(annee1, dateYYYYMMDD_1  , 4); annee1[4] = 0x00;
  strncpy(mois1,  dateYYYYMMDD_1+4, 2); mois1 [2] = 0x00;
  strncpy(jour1,  dateYYYYMMDD_1+6, 2); jour1 [2] = 0x00;
  strncpy(annee2, dateYYYYMMDD_2  , 4); annee2[4] = 0x00;
  strncpy(mois2,  dateYYYYMMDD_2+4, 2); mois2 [2] = 0x00;
  strncpy(jour2,  dateYYYYMMDD_2+6, 2); jour2 [2] = 0x00;
  // la formule simple (pas d'années bissextiles, tous les mois à 30 jours)
  nbDays = ((atoi(annee2) - atoi(annee1))*12*365) + (atoi(mois2) - atoi(mois1))*30 + (atoi(jour2) - atoi(jour1));
  return nbDays;
}

//************************************************************************
// retourne 0 si les dates sont égales,
// 1 si la 1ere est postérieure à la 2eme
// -1 si la 1ere est antérieure à la 2eme
//************************************************************************
int Utile_cmpDates (char* dateYYYYMMDD_1, char* dateYYYYMMDD_2)
{
  char  annee2[4+1];
  char  mois2[2+1];
  char  jour2[2+1];
  char  annee1[4+1];
  char  mois1[2+1];
  char  jour1[2+1];
  int   test = 0;
  if (dateYYYYMMDD_1 == NULL) return 0;
  if (dateYYYYMMDD_2 == NULL) return 0;
  strncpy(annee1, dateYYYYMMDD_1  , 4); annee1[4] = 0x00;
  strncpy(mois1,  dateYYYYMMDD_1+4, 2); mois1 [2] = 0x00;
  strncpy(jour1,  dateYYYYMMDD_1+6, 2); jour1 [2] = 0x00;
  strncpy(annee2, dateYYYYMMDD_2  , 4); annee2[4] = 0x00;
  strncpy(mois2,  dateYYYYMMDD_2+4, 2); mois2 [2] = 0x00;
  strncpy(jour2,  dateYYYYMMDD_2+6, 2); jour2 [2] = 0x00;
  test = strncmp(annee1, annee2, 4);
  if (test > 0) {
    return 1;
  }
  else if (test < 0) {
    return -1;
  }
  else {
    test = strncmp(mois1, mois2, 2);
    if (test > 0) {
      return 1;
    }
    else if (test < 0) {
      return -1;
    }
    else {
      test = strncmp(jour1, jour2, 2);
      if (test > 0) {
        return 1;
      }
      else if (test < 0) {
        return -1;
      }
      else {
        return 0;
      }
    }
  }
}

//************************************************************************
// retourne  0 si la date d'effet a le bon format,
// retourne -1 sinon
// Formats possibles:
//    YYYY/MM/DD
//    YYYYMMDD
//************************************************************************
int Utile_hasDateFormat (char* date, char* dateFormat)
{
  int   test = 0;
  if (date!=NULL && dateFormat!=NULL && strlen(date)==strlen(dateFormat))
  {
    if (strlen(dateFormat) == 10 && dateFormat[4]=='/' && dateFormat[7]=='/') // format attendu YYYY/MM/DD
    {
      if (date[4]=='/' && date[7]=='/')
      {
        return 0;
      }
      {
        return -1;
      }
    }
    else if (strlen(dateFormat) == 8)// format attendu YYYYMMDD
    {
      if (strchr(date, '/') == NULL)
      {
        return 0;
      }
      {
        return -1;
      }
    }
  }
  return -1;
}

//************************************************************************
// Convertit une date sur 14 car en date sur 8 car
// YYYYMMDDHHMISS vers YYYYMMDD
// non destructeur pour str
//************************************************************************
char* Utile_convDate14To8 (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    strcpy(tmpstr,str);
    if (strlen(tmpstr)==14) {
      tmpstr[8] = 0x00;
    }
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type YYYY/MM/DD vers YYYYMMDD
// non destructeur pour str
//************************************************************************
char* Utile_convDateSlashToYYYYMMDD (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    if (strlen(str)==10) {
      tmpstr[0]  = str[0];
      tmpstr[1]  = str[1];
      tmpstr[2]  = str[2];
      tmpstr[3]  = str[3];
      tmpstr[4]  = str[5];
      tmpstr[5]  = str[6];
      tmpstr[6]  = str[8];
      tmpstr[7]  = str[9];
    }
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type YYYYMMDD ou YYYYMMDDHH24MISS vers YYYY/MM/DD
// non destructeur pour str
//************************************************************************
char* Utile_convDateYYYYMMDDToSlash (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    if (strlen(str)==8 || strlen(str)==14)
    {
      tmpstr[0]  = str[0];
      tmpstr[1]  = str[1];
      tmpstr[2]  = str[2];
      tmpstr[3]  = str[3];
      tmpstr[4]  = '/';
      tmpstr[5]  = str[4];
      tmpstr[6]  = str[5];
      tmpstr[7]  = '/';
      tmpstr[8]  = str[6];
      tmpstr[9]  = str[7];
    }
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type DD/MM/YY ou YYYYMMDDHH24MISS vers YYYY/MM/DD
// non destructeur pour str
//************************************************************************
char*       Utile_convDateDDsMMsYYToYYYYsMMsDD (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    if (strlen(str)==8 || strlen(str)==14)
    {
      tmpstr[0]  = '2';
      tmpstr[1]  = '0';
      tmpstr[2]  = str[6];
      tmpstr[3]  = str[7];
      tmpstr[4]  = '/';
      tmpstr[5]  = str[3];
      tmpstr[6]  = str[4];
      tmpstr[7]  = '/';
      tmpstr[8]  = str[0];
      tmpstr[9]  = str[1];
    }
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type YYYY vers YYYYMMDD
// non destructeur pour str
//************************************************************************
char* Utile_convDateYYYYToYYYYMMDD (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    if (strlen(str)==4) {
      sprintf(tmpstr,"%s0101",str);
    }
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type YYYYMMDD vers DD/MM/YYYY
// non destructeur pour str
//************************************************************************
char* Utile_convDateYYYYMMDDToDDsMMsYYYY (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    tmpstr[0]  = str[6];
    tmpstr[1]  = str[7];
    tmpstr[2]  = '/';
    tmpstr[3]  = str[4];
    tmpstr[4]  = str[5];
    tmpstr[5]  = '/';
    tmpstr[6]  = str[0];
    tmpstr[7]  = str[1];
    tmpstr[8]  = str[2];
    tmpstr[9]  = str[3];
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type YYYY/MM/DD vers DD/MM/YYYY
// non destructeur pour str
//************************************************************************
char* Utile_convDateYYYYsMMsDDToDDsMMsYYYY (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    tmpstr[0]  = str[8];
    tmpstr[1]  = str[9];
    tmpstr[2]  = '/';
    tmpstr[3]  = str[5];
    tmpstr[4]  = str[6];
    tmpstr[5]  = '/';
    tmpstr[6]  = str[0];
    tmpstr[7]  = str[1];
    tmpstr[8]  = str[2];
    tmpstr[9]  = str[3];
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Convertit une date de type DD/MM/YYYY vers YYYY/MM/DD
// non destructeur pour str
//************************************************************************
char* Utile_convDateDDsMMsYYYYToYYYYsMMsDD (char* str)
{
  static char tmpstr[30+1];
  int         ret = 0;
  IN();
  memset(tmpstr,0,(30+1));
  if (str) {
    tmpstr[0]  = str[6];
    tmpstr[1]  = str[7];
    tmpstr[2]  = str[8];
    tmpstr[3]  = str[9];
    tmpstr[4]  = '/';
    tmpstr[5]  = str[3];
    tmpstr[6]  = str[4];
    tmpstr[7]  = '/';
    tmpstr[8]  = str[0];
    tmpstr[9]  = str[1];
  }
  OUT(ret);
  return tmpstr;
}

//************************************************************************
// Retourne le plus grand entier representable dans un int
//************************************************************************
int Utile_getIntMAX (void)
{
  int   len = 8*sizeof(int);
  int   ret = 1;
  int   i;
  IN();
  // printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT(ret);
  return ret;
}

//************************************************************************
// Retourne le plus petit entier negatif representable dans un int
//************************************************************************
int Utile_getIntMIN (void)
{
  int   len = 8*sizeof(int);
  int   ret = 1;
  int   i;
  IN();
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  // ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus grand entier representable dans un long
//************************************************************************
long Utile_getLongMAX (void)
{
  int   len = 8*sizeof(long);
  long  ret = 1;
  int   i;
  IN();
  // printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus petit entier negatif representable dans un long
//************************************************************************
long Utile_getLongMIN (void)
{
  int   len = 8*sizeof(long);
  long  ret = 1;
  int   i;
  IN();
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  // ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus grand entier representable dans un long
//************************************************************************
long long Utile_getLongLongMAX (void)
{
  int       len = 8*sizeof(long long);
  long long ret = 1;
  int   i;
  IN();
  //printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    //printf("%s = %lld\n", nomFonction, ret);
  }
  ret = ret - 1;
  //printf("%s = %lld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus petit entier negatif representable dans un long
//************************************************************************
long long Utile_getLongLongMIN (void)
{
  int       len = 8*sizeof(long long);
  long long ret = 1;
  int   i;
  IN();
  //printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    //printf("%s = %lld\n", nomFonction, ret);
  }
  //ret = ret - 1;
  //printf("%s = %lld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

/////////////////////////////////////////////////////////////////////////
// les mêmes fonctions mais avec les variantes unsigned
/////////////////////////////////////////////////////////////////////////

//************************************************************************
// Retourne le plus grand entier representable dans un int
//************************************************************************
unsigned int Utile_getUIntMAX (void)
{
  int           len = 8*sizeof(unsigned int);
  unsigned int  ret = 1;
  int   i;
  IN();
  // printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus petit entier negatif representable dans un int
//************************************************************************
unsigned int Utile_getUIntMIN (void)
{
  int           len = 8*sizeof(unsigned int);
  unsigned int  ret = 1;
  int   i;
  IN();
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  // ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus grand entier representable dans un long
//************************************************************************
unsigned long Utile_getULongMAX (void)
{
  int           len = 8*sizeof(unsigned long);
  unsigned long ret = 1;
  int   i;
  IN();
  // printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus petit entier negatif representable dans un long
//************************************************************************
unsigned long Utile_getULongMIN (void)
{
  int           len = 8*sizeof(unsigned long);
  unsigned long ret = 1;
  int   i;
  IN();
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    // printf("%s = %ld\n", nomFonction, ret);
  }
  // ret = ret - 1;
  // printf("%s = %ld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus grand entier representable dans un long
//************************************************************************
unsigned long long Utile_getULongLongMAX (void)
{
  int                 len = 8*sizeof(unsigned long long);
  unsigned long long  ret = 1;
  int   i;
  IN();
  //printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    //printf("%s = %lld\n", nomFonction, ret);
  }
  ret = ret - 1;
  //printf("%s = %lld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//************************************************************************
// Retourne le plus petit entier negatif representable dans un long
//************************************************************************
unsigned long long Utile_getULongLongMIN (void)
{
  int                len = 8*sizeof(unsigned long long);
  unsigned long long ret = 1;
  int   i;
  IN();
  //printf("%s = %d\n", nomFonction, len);
  for (i=0; i<(len-1); i++) {
    ret *= 2;
    //printf("%s = %lld\n", nomFonction, ret);
  }
  //ret = ret - 1;
  //printf("%s = %lld\n", nomFonction, ret);
  OUT((int)ret);
  return ret;
}

//////////////////////////////////////////////////////////////////
// Dédoublonnage d'une collection (fonction generique)
//  void*   coll      collection: vecteur de structures le plus souvent
//  size_t  szElem    taille d'un elemt
//  size_t  nbIn      nb elemt en entrée (evec doublons)
//  size_t* nbOut     nb elemt en sortie (uniques)
//  size_t* isDoub    vecteur d'indicateurs: à 1 si l'élément de même indice de la collection est un doublon
//  FunCmp* cmpsort   la fonction de comparaison de 2 elemt de la collection pour le tri
//  FunCmp* similarity  la fonction de similarité de 2 elemt de la collection pour la recherche de doublons
//  La fonction de tri et celle de recherche de similarité peuvent être différentes.
//  même si elles sont très proches.
//
int Utile_dedoub(
    void*   coll
  , size_t  szElem
  , size_t  nbIn
  , size_t* nbOut
  , size_t* isDoub
  , FunCmp* cmpsort
  , FunCmp* similarity
)
{
  IN();
  int ret = 0;
  // tri de tous les éléments
  // tri par similarité
  // ensuite on dédoublonne à partir de cette similarité
  {
    qsort (coll
        , (size_t)nbIn
        , (size_t)(szElem)
        , cmpsort);
  }
  // dedoublonnage par similarité
  {
    if (isDoub) memset(isDoub,0,nbIn);
    size_t i1 = 0;
    size_t i2;
    for (i2=1;i2<nbIn;i2++)
    {
      if (similarity ((coll+i1*szElem), (coll+i2*szElem)) == 0)
      {
        if (isDoub) isDoub[i2] = 1;
        //Erreur_set(CLA_APP,nomFonction, "DOUBLON, [%d ->%d] et [%d]", nomFonction, i2-1,i1,i2);
        Message_printDebug(LOG,"%s() DOUBLON, [%d ->%d] et [%d]", nomFonction, i2-1,i1,i2);
      }
      else {
        i1 +=1;
        if (i2 > i1) memcpy((coll+i1*szElem), (coll+i2*szElem), szElem);
        // le test est là pour éviter de recopier sur place (overlaping pour valgrind)
      }
    }
    *nbOut = i1+1;
    Message_printDebug (ETAPE, "%s() après dedoublonnage, clés: %d", nomFonction, *nbOut);
  }
  OUT(ret);
  return ret;
}


