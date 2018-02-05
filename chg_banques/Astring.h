#ifndef _Apchar_H
#define _Apchar_H

#include "Cerr.h"

/* les modes pour setAstring() */
#define	O_COPY	0
#define	O_CAT	1

/* tailles par défaut */
#define	L_volAstring	10000	/* paquet d'octets */
#define	N_ptrAstring	1000	/* paquet de pointeurs */

/* pointeur de char */
typedef struct {
  char* s;
} pchar;
/* tableau de pointeur de char */
typedef struct {
  pchar* a;
  int	 nused;
  int	 nalloc;
} Apchar;

/* un tableau de chaines de caractères : un buffer + un tableau de pointeurs */
typedef struct {
  char*  s;	/* le contenu */
  long  lalloc;	/* la longueur totale allouée */
  long  lused;	/* la longueur totale occupée */
  Apchar* ap;	/* le tableau de pointeurs */
} Astring;

Apchar*	newApchar(int n);
Apchar*	reallocApchar(Apchar* ap, int n);
int	freeApchar(Apchar* ap);
int	clearApchar(Apchar* ap);
int	setApchar(Apchar* ap, int i, char* s);
int	getNallocApchar(Apchar* ap);
int	getNusedApchar(Apchar* ap);

Astring*	newAstring(long vol, int np);
int	freeAstring(Astring* as);
int	setAstring(Astring* as, char* buffin, int mode);
int	sepAstring(Astring* as, char sep);
long	getLenAstring(Astring* as);
long	getNlinesAstring(Astring* as);
long	getStringAstring(Astring* as, char* buffout, long lmax, char* sep);
char*	toStringAstring(Astring* as, char* sep);
int	afficheAstring(int level, Astring* as);
int	rtrimAstring(Astring* as);
int	addElemAstring(Astring* as, char* elem);
char*	getElemAstring(Astring* as, int ielem);

#endif
