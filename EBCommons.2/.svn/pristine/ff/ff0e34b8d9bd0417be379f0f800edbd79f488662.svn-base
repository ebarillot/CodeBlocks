/**********************************************************
 * Librairie C EXPLOIT  mise en exploitation d'un prog. C *
 **********************************************************/
/* Modif du 24/03/2005 : TestEcriture renforce : Si errno>0 alors Erreur
   Retour arriere car les appli utilise l'api apres un traitement
   mais pas apres une ecriture de fichier : donc errno lies aux traitement
   pose des problemes de test plus renforce
   Modif du 18/05/2006 : Si strtok : rajout string.h
   Modif du 06/01/2015 : LitReprise : suppression des variables inutiles  vals et vald
                         Mise en place de plusieurs fonctions pour jouer
                         avec le fd du fichier Log et le nom du programme
*/
#include <stdio.h>
#include <stdarg.h>
#include <time.h> /* #include <sys/time.h> */
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "exploit.h"
#include <string.h> 	/* Pour strtok : surtout en 64 bits */
#include <unistd.h>

#define SEP "/"
#define NULLIF(a) (a==NULL)?"":a

static char date_lib_exploit[19+1];
static char date_lib_exploit_milli[23+1];
static char date_debut_lib_exploit_milli[23+1];

static char nom_programme[200];
static FILE *ptr_fichier_log;
static int fichier_log_ouvert = 0;
static FILE *ptr_fichier_trace_all;
static int fichier_trace_all_ouvert = 0;


//////////////////////////////////////////////////////////////////////////////////
// 2014-07-02 E.Barillot
// récupère le nom du programme pour le transmettre à une bibliothèque ou un module
// qui a besoin de logger dans le même fichier
char* Log_getNomProgramme(void)
{
  return nom_programme;
}

//////////////////////////////////////////////////////////////////////////////////

// 2014-07-01 E.Barillot
// récupère le fd du fichier Log pour le transmettre à une bibliothèque ou un module
// qui a besoin de logger dans le même fichier
FILE* Log_getFd(void)
{
  return ptr_fichier_log;
}


/////////////////////////////////////////////////////////////////////////////////////////
// 2015-01-06 E.Barillot
// positionne le nom du programme pour le Log
void Log_setNomProgramme (char* nomProgramme)
{
  printf("Log_setNomProgramme() nomProgramme = %s\n", nomProgramme);
  if (nomProgramme == NULL || nomProgramme[0] == 0x00) {
    strcpy (nom_programme, "nom programme inconnu");
  }
  else {
    strcpy (nom_programme, nomProgramme);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// 2015-01-06 E.Barillot
// positionne le fd du fichier Log pour en utiliser un déjà ouvert
void Log_setFd (FILE *ptr)
{
  ptr_fichier_log = ptr;
  fichier_log_ouvert = 1;
}



/*************************************************************
 * Ecriture de fin et debut de programme, message avec heure *
 *************************************************************/

/* Suppression du chemin dans un nom de programme */
void SupChemin(char *in, char *out)
{
  char *s;
  char *nom;
char copy_in[500];

/*  MODIF YE 05/04/2012
s_initial = malloc(500 * sizeof(char));
nom_initial = malloc(500 * sizeof(nom));
memset(s,'\0',sizeof(s));
memset(nom,'\0',sizeof(nom));
*/
  strcpy(copy_in,in);
  s = (char *)strtok(copy_in,SEP);
  nom = s;

  while ((s = (char *)strtok(NULL,SEP)) != NULL) {
    nom = s;
  }
  strcpy(out,nom);
}

/* Retourne le pointeur sur la variable globale date_lib_exploit contenant
   la date system + les millisecondes */
char *DateSystemMilli(void)
{
struct tm *tp;
struct timeval tval;

  gettimeofday(&tval, NULL);
  tp = localtime(&(tval.tv_sec));
  sprintf(date_lib_exploit_milli,"%02d/%02d/%04d %02d:%02d:%02d,%03d",
    tp->tm_mday,tp->tm_mon+1,1900+tp->tm_year,tp->tm_hour,tp->tm_min,
    tp->tm_sec,tval.tv_usec/1000);
  return date_lib_exploit_milli;
}

/* Ecriture du debut du programme */
void DebutProg_exploit(FILE *f,char *nomprog,char *datecomp,char *timecomp)
{
char nom[50];
struct tm *tp;
long now;

  /* Initialisation de la date de debut */
  sprintf(date_debut_lib_exploit_milli,"%s",DateSystemMilli());

  now = time((long *)0);
  tp = localtime(&now);
  SupChemin(nomprog,nom);
  fprintf(f,"%02d/%02d/%04d %02d:%02d:%02d DEBUT PROGRAMME %s COMPILE LE %s %s\n",
          tp->tm_mday,tp->tm_mon+1,1900+tp->tm_year,tp->tm_hour,tp->tm_min,
          tp->tm_sec,nom,datecomp,timecomp);
  fflush(f);
}

/* Ecriture de la fin du programme */
void FinProg(FILE *f,char *nomprog)
{
char nom[50];
struct tm *tp;
long now;

  /* Supprime car juge inutile : A Faire dans le programme run() de VTOM
  if ((ptr_fichier_trace_all = fopen(getenv("ORTTRACE_ALL_PROG")==NULL?"/ort/admin/expl/trace_all_prog.csv":getenv("ORTTRACE_ALL_PROG"),"a")) != NULL)
  {
    fprintf(ptr_fichier_trace_all,"%s|%s|%s|%s|%s|%s|%s|\n",
        NULLIF(date_debut_lib_exploit_milli),DateSystemMilli(),NULLIF(getenv("ORTMACHINE")),NULLIF(getenv("ORTCOST")),
        NULLIF(getenv("LOGNAME")),NULLIF(getenv("TOM_JOB")),
        nom_programme);
    fclose(ptr_fichier_trace_all);
  } */
  now = time((long *)0);
  tp = localtime(&now);
  SupChemin(nomprog,nom);
  fprintf(f,"%02d/%02d/%04d %02d:%02d:%02d FIN   PROGRAMME %s\n",
          tp->tm_mday,tp->tm_mon+1,1900+tp->tm_year,tp->tm_hour,tp->tm_min,
          tp->tm_sec,nom);
  fflush(f);
}

/* Retourne le pointeur sur la variable globale date_lib_exploit contenant
   la date system */
char *DateSystem(void)
{
struct tm *tp;
long now;

  now = time((long *)0);
  tp = localtime(&now);
  sprintf(date_lib_exploit,"%02d/%02d/%04d %02d:%02d:%02d",
          tp->tm_mday,tp->tm_mon+1,1900+tp->tm_year,tp->tm_hour,tp->tm_min,
          tp->tm_sec);
  return date_lib_exploit;
}


/*************************************************************
 * Gestion des reprises                                      *
 *************************************************************/

int InitNomReprise(char *nom,char *nomprog, char *modeOriginal)
{
  int i;
  char nom_prog[100];
  char *modeExec = NULL;
  char *mode=NULL;

  memset(nom_prog, '\0', sizeof(nom_prog));

  if (modeOriginal!=NULL){
    mode=(char*)malloc((strlen(modeOriginal)+1)*sizeof(char));
    if (mode==NULL)
     return -4;
    strcpy(mode,modeOriginal);
  }
  SupChemin(nomprog,nom_prog);
  for (i=0;i<strlen(mode);i++) {
    mode[i] = toupper(mode[i]);
  }
  if ((modeExec = getenv("TO_JOB_NAME")) == NULL) {
    /* Variable TO_JOB_NAME non positionnée pour XOS */
    if ((modeExec = getenv("TOM_JOB")) == NULL) {
      /* Variable TOM_JOB non positionnée pour VTOM */
	free(mode);
      return -1;
    }
  }

  if (strcmp(mode,"DROP") == 0) {
    if (getenv("REPDROP") == NULL) {
      /* Variable REPDROP non positionnée */
      free(mode);
      return -2;
    }
    strcpy(nom,getenv("REPDROP"));
    strcat(nom,"/");
    strcat(nom,modeExec);
    strcat(nom,"_");
    strcat(nom,nom_prog);
    strcat(nom,".reprise");
  }
  else if (strcmp(mode,"KEEP") == 0) {
    if (getenv("REPKEEP") == NULL) {
      /* Variable REPKEEP non positionnée */
      free(mode);
      return -3;
    }
    strcpy(nom,getenv("REPKEEP"));
    strcat(nom,"/");
    strcat(nom,modeExec);
    strcat(nom,"_");
    strcat(nom,nom_prog);
    strcat(nom,".reprise");
  }
  else {
    free(mode);
    return -4;
  }
  free(mode);
  return 0;
}

int EcritReprise(const char *nom, char *fmt, ...)
{
va_list pa;
FILE *prep;

  if ((prep = fopen(nom,"w")) == NULL) {
     return -1;
  }
  va_start(pa,fmt);
  vfprintf(prep,fmt,pa);
  va_end(pa);
  fclose(prep);
  return 0;
}

int LitReprise(const char *nom, char *fmt, ...)
{
va_list pa;
FILE *prep;
char *p;
int vali;
long vall;

  if ((prep = fopen(nom,"r")) == NULL) {
     return -1;
  }
  va_start(pa,fmt);
  for (p = fmt; *p; p++) {
    if (*p != '%') {
      continue;
    }
    switch (*++p) {
      case 'l' :
        if (*++p == 'd') {
          vall = va_arg(pa, long);
          fscanf(prep,"%ld",vall);
        }
        else {
          fputc(*p, prep);
        }
        break;
      case 'd' :
        vali = va_arg(pa, int);
        fscanf(prep,"%d",vali);
        break;
      default :
        break;
    }
  }

  va_end(pa);
  fclose(prep);
  return 0;
}

int SupReprise(const char *nom)
{
/* char command[500];

  strcpy(command,"/bin/rm ");
  strcat(command,nom);
  strcat(command," 2>/dev/null");
  return system(command); */
  return unlink(nom);
}

/*************************************************************
 * Gestion du fichier contenant les logs                     *
 *************************************************************/

int OuvreLog(char *nom_fichier,char *nomprog)
{
  char *modeExec = NULL;

  SupChemin(nomprog,nom_programme);
  if ((ptr_fichier_log = fopen(nom_fichier,"a")) == NULL) {
    return 1;
  }

  /* Recherche du type d'environnement : VTOM ($TOM_JOB définie) ou XOS ($TO_JOB_NAME définie)*/
  if ( (modeExec = getenv("TO_JOB_NAME")) == NULL)
    modeExec = getenv("TOM_JOB");

  fprintf(ptr_fichier_log,"%s|%s|%s|  DEBUT DU PROGRAMME|\n",DATE,NULLIF(modeExec),nom_programme);
  fflush(ptr_fichier_log);
  fichier_log_ouvert = 1;
  return 0;
}

void FermeLog(void)
{
  char *modeExec = NULL;

  /* Recherche du type d'environnement : VTOM ($TOM_JOB définie) ou XOS ($TO_JOB_NAME définie)*/
  if ( (modeExec = getenv("TO_JOB_NAME")) == NULL)
    modeExec = getenv("TOM_JOB");

  fprintf(ptr_fichier_log,"%s|%s|%s|    FIN DU PROGRAMME|\n",DATE,NULLIF(modeExec),nom_programme);
  fclose(ptr_fichier_log);
  fichier_log_ouvert = 0;
}

void Log(char *rubrique,char *formatOriginal,...)
{
  va_list pa;
  int i;
  char *modeExec = NULL;
  char *format = NULL;

  /* Recherche du type d'environnement : VTOM ($TOM_JOB définie) ou XOS ($TO_JOB_NAME définie)*/
  if ( (modeExec = getenv("TO_JOB_NAME")) == NULL)
    modeExec = getenv("TOM_JOB");

  if (formatOriginal!=NULL){
    format=(char*)malloc((strlen(formatOriginal)+1)*sizeof(char));
    if (format==NULL)
      printf("Erreur memoire lors du traitement du message\n");
    strcpy(format,formatOriginal);
  }

  if (fichier_log_ouvert == 1) {
    va_start(pa,formatOriginal);
    fprintf(ptr_fichier_log,"%s|%s|%s|%20s|",DATE,NULLIF(modeExec),nom_programme,rubrique);
    for (i=0;i<strlen(format);i++) {
      if (format[i] == '\n') format[i] = ' ';
    }
    vfprintf(ptr_fichier_log,format,pa);
    fflush(ptr_fichier_log);
    fprintf(ptr_fichier_log,"\n");
    va_end(pa);
  }
  else {
    printf("Le fichier LOG n'est pas ouvert, ecriture impossible\n");
  }
}

void PLog(char *rubrique,char *format,  ...)
{
  char *fmt_text; /* Variable temporaire necessaire si multi appel de Plog */
  va_list pa;
  int i;
  char *modeExec = NULL;

  /* Recherche du type d'environnement : VTOM ($TOM_JOB définie) ou XOS ($TO_JOB_NAME définie)*/
  if ( (modeExec = getenv("TO_JOB_NAME")) == NULL)
    modeExec = getenv("TOM_JOB");

  fmt_text=(char *)malloc(strlen(format)+1);
  memcpy(fmt_text,format,strlen(format));
  fmt_text[strlen(format)]=0;

  if (fichier_log_ouvert == 1) {
    va_start(pa, format );
    printf("%s ",DATE);
    vprintf(format, pa);
    fflush(stdout);
    fprintf(ptr_fichier_log,"%s|%s|%s|%20s|",DATE,NULLIF(modeExec),nom_programme,rubrique);
    for (i=0;i<strlen(fmt_text);i++) {
      if (fmt_text[i] == '\n') fmt_text[i] = ' ';
    }
    vfprintf(ptr_fichier_log,fmt_text,pa);
    fflush(ptr_fichier_log);
    fprintf(ptr_fichier_log,"\n");
    va_end(pa);
  }
  else {
    printf("Le fichier LOG n'est pas ouvert, ecriture impossible\n");
  }
  free(fmt_text);
}

/*************************************************************
 * Ecriture de messages sur la sortie standart et flush      *
 *************************************************************/

/* Identique fprintf avec fflush en plus */
void PrintExploit(char* format,...) {
va_list pa ;

  va_start(pa, format);
  vprintf(format, pa);
  fflush(stdout);
  va_end(pa);
}

/*************************************************************
 * Test d'ecriture dans tous les fichiers ouverts            *
 *************************************************************/

/* Cette fonction -1 en cas de problème et la chaîne de
   caractères mess contient le message d'erreur
   sinon elle retourne 1 si OK */
int TestEcriture(char *mess)
{
int i;

  if (fflush(NULL) < 0) {
    strcpy(mess,strerror(errno));
    for (i=0;i<strlen(mess);i++) mess[i] = toupper(mess[i]);
    return -1;
  }
  if ( errno == ENOSPC || errno == EFBIG ) {
    strcpy(mess,strerror(errno));
    for (i=0;i<strlen(mess);i++) mess[i] = toupper(mess[i]);
    fflush(NULL);
    return(-1);
  }
  if (fflush(NULL) < 0) {
    strcpy(mess,strerror(errno));
    for (i=0;i<strlen(mess);i++) mess[i] = toupper(mess[i]);
    return -1;
  }

  return 0;
}

