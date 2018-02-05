/************************************************************************/
/*    ooo___                                                            */
/*   o  /o  )                                                           */
/*   o   o--\   TELEMATIQUE - CENTRE INFORMATIQUE - ROCHECORBON         */
/*    ooo    \                                                          */
/************************************************************************/
/* NOM        :                                                         */
/* TYPE       :                                                         */
/* PROJET     : REN2		                                        */
/************************************************************************/
/* OBJET : Classe Fichier						*/
/************************************************************************/
/*  ____________________________________________________________________*/
/* |        |         |            |                                   |*/
/* |  DATE  | Version |  Auteur    | Commentaires                      |*/
/* |--------|---------|------------|-----------------------------------|*/
/* |25/08/03|   01    | E.BARILLOT | Création                          |*/
/*  ------------------------------------------------------------------- */
/*                                                                      */
/************************************************************************/

#ifndef	Fichier_H
#define	Fichier_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Cerr.h"
#include "Astring.h"

#define	STAT_NO_FILE	-1


#define	O_NL	1
#define	O_NONL	0

#define L_FichierNom		500
#define L_FichierChemin		1000
#define L_FichierNomComplet	L_FichierChemin+L_FichierNom
#define L_FichierMode		10
typedef struct {
  char		nomComplet[L_FichierNomComplet+1];
  char		nom[L_FichierNom+1];
  char		chemin[L_FichierChemin+1];
  char		mode[L_FichierMode+1];
  struct stat	stat;
  int		exists;	/* positionné après un appel à statFichier() */
  FILE*		fd;
  int		nbFetch;
  Astring*	ctu;
} Fichier;


Fichier*	newFichier(char* foncNom);
int	freeFichier(Fichier*	fichier);
int	setFichier(Fichier*	fichier,
		   char*	nomComplet,
		   char*	nom,
		   char*	chemin,
		   char*	mode);
int	ouvreFichier(Fichier*	fichier);
int	fetchFichier(Fichier*	fichier, char*	out, int lout, int nl);
int	loadCtuFichier(Fichier*	fichier);
int	loadCtuSepFichier(Fichier* fichier, char sep);
int	allocCtuFichier(Fichier*	fichier);
int	fseekFichier(Fichier*	fichier, long offset);
int	ftellFichier(Fichier*	fichier, long* offset);
int	fermeFichier(Fichier*	fichier);
int	estOuvertFichier(Fichier*	fichier);
int	nbFetchFichier(Fichier*	fichier);
char*	toStringFichier(Fichier*	fichier);
int	ecrireFichier(Fichier*	fichier, char* fmt, ...);
int	flushFichier(Fichier*	fichier);
int	statFichier(Fichier*	fichier);
Astring*	getCtuFichier(char*	nomFichier);
int	getFileAsSepBuffer(char** buffer,
			   long* lenout,
			   char* nomFichier,
			   char* sep);
int	existeFichier(int* exist, long* size, char* dir, char* name);

#endif
