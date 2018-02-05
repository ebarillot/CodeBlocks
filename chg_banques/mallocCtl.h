#ifndef	_MALLOC_CTL_API_H
#define	_MALLOC_CTL_API_H

#if defined _SOLARIS || defined _LINUX
#include <sys/resource.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


#define	GC_OK		1
#define	GC_KO		0

#define	enteteOn	1
#define	enteteOff	0

#define	traceMem	3

/************************************************************************/
/* Prototypes								*/
/************************************************************************/
void stockAdressesOn (void);
void stockAdressesOnReinitCount (void);
void stockAdressesOff (void);
int getMallocCount (void);

/* allocation memoire */
void*	mallocCtl (long		sz,
		   char*	varNom,
		   char*	foncNom);

void*	reallocCtl (void*	mem,
		    long	sz,
		    char*	varNom,
		    char*	foncNom);

/* desallocation memoire */
int	freeCtl (void*	mem, char* foncNom);

int affAdrmem (int gc, int entete);

#endif
