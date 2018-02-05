#ifndef	chg_banques_H
#define	chg_banques_H


#define	NOM_BASE_REFORT	"REFORT" /* le tag qui est dans connexion.ora */

#define	L_datedeb	30
#define	L_datefin	30
#define	L_fichierChemin	500

typedef struct {
  int	traceLvl;
  char	fichierInp[L_fichierChemin+1];
  char	fichierResult[L_fichierChemin+1];
  unsigned int sleep;
  int nbLus_fic;
  int nbLus_tab;
  int nbSucces;
  int nbVide;
  int nbErreur;
} Contexte;

#endif
