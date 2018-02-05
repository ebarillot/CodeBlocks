#ifndef connexionOracle_H
#define connexionOracle_H

#define	L_nomBase	200

typedef struct {
  char	nomBase[L_nomBase+1];	/* nom de la base */
  int	connecte;		/* connexion à la base oui/non */
} ConnexionOracle;


ConnexionOracle*	newConnexionOracle(char* nomBase);
int	freeConnexionOracle(ConnexionOracle* cox);
int	connectOracle(ConnexionOracle*	cox);
int	commitOracle(ConnexionOracle*	cox);
int	rollbackOracle(ConnexionOracle*	cox);
int	commitOracleQ(ConnexionOracle*	cox);
int	rollbackOracleQ(ConnexionOracle*	cox);
int	testConnexionOracle(ConnexionOracle*	cox);

char*	libelle_erreur_nconnexion(int ret);
int	estConnecte(ConnexionOracle*	cox);
char*	getNomBase(ConnexionOracle*	cox);

int	tkprofOracle(ConnexionOracle*	cox);
int	setFormatDateOracle_fullDesc(ConnexionOracle* cox);
int	setLanguageOracle(ConnexionOracle* cox);

#endif
