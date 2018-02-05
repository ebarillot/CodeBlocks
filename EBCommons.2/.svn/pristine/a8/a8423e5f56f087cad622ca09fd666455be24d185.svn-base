/**********************************************************
 * Librairie C EXPLOIT  mise en exploitation d'un prog. C *
 **********************************************************/
#ifndef _LIB_EXPL_
#define _LIB_EXPL_
#include <stdio.h>
#include <errno.h>

/**********************************************************
 * Définition des codes retour en fonction de l'évenement *
 **********************************************************/

/* Fin normale du programme */
#define FIN_PROG		exit(0) 
/* Fin provoqué par l'utilisateur (kill -16) */
#define FIN_UTILISATEUR		exit(220) 
/* Fin provoqué par une erreur ORACLE grave */
#define FIN_ERREUR_ORACLE	exit(221) 
/* Fin provoqué par une erreur apllicative grave */
#define FIN_ERREUR_APPLICATIVE	exit(222) 

/* Code fin normale du programme */
#define CODE_FIN_PROG			0 
/* Code fin provoqué par l'utilisateur (kill -16) */
#define CODE_FIN_UTILISATEUR		220 
/* Code fin provoqué par une erreur ORACLE grave */
#define CODE_FIN_ERREUR_ORACLE		221 
/* Code fin provoqué par une erreur apllicative grave */
#define CODE_FIN_ERREUR_APPLICATIVE	222 

//////////////////////////////////////////////////////////////////////////////////
// 2014-07-02 E.Barillot
// récupère le nom du programme pour le transmettre à une bibliothèque ou un module
// qui a besoin de logger dans le même fichier
char* Log_getNomProgramme(void);
//////////////////////////////////////////////////////////////////////////////////
// 2014-07-01 E.Barillot
// récupère le fd du fichier Log pour le transmettre à une bibliothèque ou un module
// qui a besoin de logger dans le même fichier
FILE* Log_getFd(void);

/////////////////////////////////////////////////////////////////////////////////////////
// 2015-01-06 E.Barillot
// positionne le nom du programme pour le Log
void Log_setNomProgramme (char* nomProgramme);

/////////////////////////////////////////////////////////////////////////////////////////
// 2015-01-06 E.Barillot
// positionne le fd du fichier Log pour en utiliser un déjà ouvert
void Log_setFd (FILE *ptr);


/*************************************************************
 * Gestion des reprises                                      *
 *************************************************************/

/* Retourne en paramétre le nom du fichier de reprise */
int InitNomReprise(char *,char *,char *);
/* Ecriture dans le fichier reprise */
int EcritReprise(const char *, char *, ...);
/* lecture dans la fichier reprise */
int LitReprise(const char *, char *, ...);
/* suppression du fichier reprise */
int SupReprise(const char *);

/*************************************************************
 * Gestion du fichier contenant les logs                     *
 *************************************************************/

/* Ouverture du fichier log en append, retourne 1 en cas d'erreur */
int OuvreLog(char *,char *);
/* Fermeture du fichier log */
void FermeLog(void);
/* Ecriture dans le fichier log */
void Log(char *,char *,...);

/* Ecriture dans le fichier log + sortie standard */
void PLog(char *,char *,...);

void PrintExploit(char *,...);
/* Identique à printf avec la DATE en plus */
#define Print printf("%s ",DATE);PrintExploit

/* Debut D. GAUFFIER le 22.11.2010 */
/*#define Erreur printf("%s Source: %s N°ligne: %ld ",DATE,__FILE__,__LINE__);PrintExploit
#ifdef DEBUG
#define Debug  printf("%s Source: %s N°ligne: %ld ",DATE,__FILE__, __LINE__);PrintExploit
#else
#define Debug
#endif*/

#define Erreur(...) { long myerrno=errno; \
                  printf("%s Source: %s N°ligne: %ld ",DATE,__FILE__,__LINE__); \
                  errno=myerrno; \
                  PrintExploit(__VA_ARGS__); }
#ifdef DEBUG
#define Debug(...) { long myerrno=errno; \
                  printf("%s Source: %s N°ligne: %ld ",DATE,__FILE__,__LINE__); \
                  errno=myerrno; \
                  PrintExploit(__VA_ARGS__); }
#else
#define Debug
#endif
/* Fin D. GAUFFIER */

/*************************************************************
 * Ecriture de fin et debut de programme, date system        *
 *************************************************************/

/* Ecriture du debut du programme */
void DebutProg_exploit(FILE *,char *,char *,char *);
#ifdef DEBUG
#define DebutProg(f,s) DebutProg_exploit(f,s,__DATE__,__TIME__);Print("LANCEMENT EN MODE DEBUG\n")
#else
#define DebutProg(f,s) DebutProg_exploit(f,s,__DATE__,__TIME__)
#endif

/* Ecriture de la fin du programme */
void FinProg(FILE *,char *);

/* Retourne la date system */
char *DateSystem(void);
#define DATE DateSystem()

/* Test d'écriture */
int TestEcriture(char *);
int TestEcritureAll(char *);

#endif
