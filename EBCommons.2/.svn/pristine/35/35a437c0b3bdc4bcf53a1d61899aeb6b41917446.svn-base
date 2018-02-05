#ifndef	_MALLOC_CTL_API_H
#define	_MALLOC_CTL_API_H

#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


#define	HEAD_OK	1
#define	NO_HEAD	0
#define	GC_OK	1
#define	NO_GC	0

/************************************************************************/
/* Prototypes								*/
/************************************************************************/
/* allocation memoire */
int	MallocCtl (void**	mem,
		   long		sz,
		   char*	varNom,
		   char*	foncNom,
		   char*	foncMessageErreur);

int	ReallocCtl (void**	mem,
		    long	sz,
		    char*	varNom,
		    char*	foncNom,
		    char*	foncMessageErreur);

/* desallocation memoire */
int	FreeCtl (void**		mem,
		 char*		foncNom,
		 char*		foncMessageErreur);


/* affichage du solde malloc/free */
int	GetMemoryCount (void);

/* affichage des variables allouées et de leur état */
int	AffAdresses (FILE* fd, int gc, int entete);

/* Pour surveiller les adresses en memoire */
void	StockAdressesOn (void);

/* Pour ne pas surveiller les adresses en memoire */
void	StockAdressesOff (void);

/* Affichage de l'état instantané de la mémoire allouée */
int	AffMemoire (FILE*	fd);


/* codes erreurs */
#define	MALLOC_ALLOCATION_IMPOSSIBLE	-1
#define	GARBAGE_COLLECTION_IMPOSSIBLE	-2
#define	POINTEUR_NULL			-3
#define	POINTEUR_NON_NULL		-4
#define	TAILLE_INCORRECTE		-5
#define	ADRESSE_INCONNUE		-6
#define	ADRESSE_DEJA_LIBRE		-7

#endif
