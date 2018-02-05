/************************************************************************/
/*									*/
/* Création: 02/06/2010 E.Barillot					*/
/*									*/
/*									*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>	/* pour gérer les va_list */
#include <unistd.h>

#include "chg_banques.h"
#include "mallocCtl.h"
#include "Cerr.h"/* les classes d'erreur */
#include "utile.h"
#include "Fichier.h"

#include <exploit.h>

#ifdef _USE_ORA
  #include "Banques.h"
  #include "ConnexionOracle.h"
#endif

/************************************************************************/
/* macros								*/
/************************************************************************/
#define L_BUFFER	100000

/************************************************************************/
/* prototypes								*/
/************************************************************************/

int traitements(Contexte* ctx);


/************************************************************************/
/* variables globales							*/
/************************************************************************/


/************************************************************************/
/* message d'aide							*/
/************************************************************************/
static void description_utilisation (char* programme)
{
  printf ("-----------------------------------------------------------\n");
  printf (" UTILISATION: %s \n", programme);
  printf ("  Paramètres obligatoires :\n");
  printf ("     -i <fichier en entree> nom fichier à charger\n");
  printf ("     -o <fichier resultat> nom fichier resultat\n");
  printf ("  Options :\n");
  printf ("     -t <nb> niveau de trace, 0 par defaut\n");
  printf ("-----------------------------------------------------------\n");
  exit(CODE_FIN_ERREUR_APPLICATIVE);
}

/************************************************************************/
/* entrée programme							*/
/* Appel de la fonction de traitement en mode BATCH			*/
/************************************************************************/
int main (int argc, char* argv[])
{
  char      nomFonction[] = "main";
  int       ret = 0;
  Contexte  ctx;
  char*     nomFichierIn = NULL;
  static char messageLastError[L_messageErreur+1];

  IN(nomFonction);
  memset (&ctx,0,sizeof(Contexte));
  memset(messageLastError,0,(L_messageErreur+1));

  if (argc < 2) {
    description_utilisation (argv[0]);
    goto XIT;
  }
  /* analyse des arguments */
  {
    int c;
    extern char *optarg;
    while ((c = getopt (argc, argv, "t:i:o:")) != EOF) {
      switch (c) {
      case 't' :
        ctx.traceLvl = atoi(optarg);
        if (ctx.traceLvl < 0) ctx.traceLvl = 0;
        if (ctx.traceLvl > 9) ctx.traceLvl = 9;
        break;
      case 'i' :
        strcpy(ctx.fichierInp, optarg);
        break;
      case 'o' :
        strcpy(ctx.fichierResult, optarg);
        break;
      default :
        printf ("option non reconnue %s\n", optarg);
        description_utilisation (argv[0]);
        break;
      }
    }
  }
  if (ctx.fichierInp[0] == 0x00) {
    description_utilisation (argv[0]);
    goto XIT;
  }
  if (ctx.fichierResult[0] == 0x00) {
    description_utilisation (argv[0]);
    goto XIT;
  }

  /* systeme de trace */
  initNiveauTrace (ctx.traceLvl);	/* pour afficheMessage() */
  if (ctx.traceLvl >= 2) stockAdressesOn();
  afficheMessage(0,LOG,"fichier resultat = %s", ctx.fichierResult);

  /* les traitements */
  ret = traitements(&ctx);
  afficheMessage(0, LOG, "Nombre de lignes lues dans le fichier : %d", ctx.nbLus_fic);
  afficheMessage(0, LOG, "Nombre de lignes lues en base : %d", ctx.nbLus_tab);

 XIT:
  /* on conserve la dernière erreur au cas où elle serait grave */
  sprintf(messageLastError,"Last error: (%d) (%s) (%s)\n",
    getLastError()->classe,
    getLastError()->fonction,
    getLastError()->message);
  printErrors();
  removeErrors();
  afficheMessage(2,LOG,"Solde alloc: %d",getMallocCount());
  affAdrmem(GC_OK,enteteOn);
  stockAdressesOff();
  OUT(nomFonction,ret);
  if (ret) {
    afficheMessage(0,LOG,"%s",messageLastError);
    FIN_ERREUR_APPLICATIVE;
  }
  else {
    FIN_PROG;
  }
}

/************************************************************************/
/* Les traitements							*/
/************************************************************************/
int traitements(Contexte* ctx)
{
  char      nomFonction[] = "traitements";
  int       ret = 0;
#ifdef _USE_ORA
  Banques*  banques = NULL;
  ConnexionOracle* cox = NULL;
#endif
#define	L_buffer	10000
  char	buffer[L_buffer+1];
  Fichier*	fresult = NULL;
  Fichier*	finp = NULL;
#ifdef useChrono
  Chrono*	chrono = NULL;
#endif


  IN(nomFonction);
  memset(buffer,0,L_buffer);
#ifdef useChrono
  chrono = newChrono();
#endif

  /* ouverture du fichier de resultats */
  afficheMessage(0,LOG,"Ouverture fichier de sortie");
  if (ctx->fichierResult[0] != 0x00) {
    if (!(fresult = newFichier(ctx->fichierResult))) goto XIT;
    if ((ret = setFichier(fresult,ctx->fichierResult,NULL,NULL,"w")) != 0) goto XIT;
    ret = ouvreFichier(fresult);
  }
  if (fresult) {
    if ((ret = ecrireFichier(fresult,"==Debut\n")) != 0) goto XIT;
  }
#ifdef _USE_ORA
  /* connexion à la base Oracle */
  if ((cox = newConnexionOracle(NOM_BASE_REFORT)) == NULL) {
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  afficheMessage(0,LOG,"Connexion à la base ORACLE %s ... ", NOM_BASE_REFORT);
  if ((ret = connectOracle(cox)) != 0) goto XIT;
  afficheMessage (0,LOG,"... OK.");
  if (ret = setFormatDateOracle_fullDesc(cox)) {
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (ret = setLanguageOracle(cox)) {
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
#endif

  /* chargement du fichier en memoire */
  afficheMessage(0,LOG,"Chargement du fichier d'entree en memoire");
  {
    #define L_ligne 10000
    char ligne[L_ligne+1];
    memset(ligne,L_ligne,0);
    if (ctx->fichierInp[0] != 0x00) {
      if (!(finp = newFichier(ctx->fichierInp))) goto XIT;
      if ((ret = setFichier(finp,ctx->fichierInp,NULL,NULL,"r")) != 0) goto XIT;
      if ((ret = ouvreFichier(finp)) != 0) goto XIT;
    }
    do {
      ret = fetchFichier(finp, ligne, L_ligne, O_NONL);
      if (ret != NO_MORE_DATA) {
        ctx->nbLus_fic++;
        if ((ret = ecrireFichier(fresult,"type=%c, longueur=%d\n",ligne[0], strlen(ligne))) != 0) goto XIT;
      }
    } while (ret != NO_MORE_DATA);
    #undef L_ligne
  }

#ifdef _USE_ORA
#ifdef useChrono
    startChrono(chrono);
#endif
  /* chargement de la table en memoire */
  if (!(banques = newBanques())) goto XIT;
  if (ret = selectBanques(cox,		/* IN */
                          banques)) {	/* OUT */
    goto XIT;
  }
#ifdef useChrono
  afficheMessage(0,"DUREE","Duree select : %.3f milli-sec",
                (float)topChrono(chrono)/(float)1000);
#endif
#endif

#ifdef useChrono
  startChrono(chrono);
#endif
#if 0
  afficheMessage(2,LOG,"** Avant tri");
  /* tri des lignes */
  if (ret = Qsort(tabData,
		  (unsigned int)(L_ligneTab+1),
		  (unsigned int)nbData,
		  0,
		  getIndexTabIndex(tabIndexQsort),
		  (int(*)(void*, void*))strcmp)) {
    keepError(CLA_APP,nomFonction,"erreur sur Qsort");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  afficheMessage(2,LOG,"** Après tri");
#ifdef useChrono
  afficheMessage(0,"DUREE","Duree Qsort : %.3f milli-sec",
                (float)topChrono(chrono)/(float)1000);
#endif
#endif

  if ((ret = ecrireFichier(fresult,
          "== Fin (%d lus fichier; %d lus table)\r\n",
          ctx->nbLus_fic, ctx->nbLus_tab)) != 0) goto XIT;

 XIT:
  if (ctx->fichierResult[0] != 0x00) {
    int	estOuvert = 0;
    int ret2 = 0;
    if (fresult) {
      estOuvert = estOuvertFichier(fresult);
      if (estOuvert) {
        if ((ret2 = fermeFichier(fresult)) != 0) ret = ret2;
      }
      freeFichier(fresult);
    }
  }
#ifdef _USE_ORA
  freeBanques(banques);
  freeConnexionOracle(cox);
#endif
#ifdef useChrono
  if (chrono) free(chrono);
#endif
  OUT(nomFonction,ret);
  return ret;
#undef	L_buffer
}
