/* fonction qui emballe et facilite l'utilisation de qsort(3) */

#ifndef _QSORT_UTILE_H
#define	_QSORT_UTILE_H
int	Qsort (void*		data,
	       unsigned int	lgel,
	       unsigned int	nbel,
	       int		reverse,
	       int*		outIndex,
	       int 		(compare (void*, void*)));


/* dedoublonne les lignes de longueur lgel, en nombre nbel
 * si outIndex est non NULL, data n'est pas modifié en sortie
 * et outIndex contient un 1 pour chaque ligne qui est un doublon
 * si outIndex est NULL, le tableau data ne contient plus les lignes doublon
 */
int     Uniq (void*		data,		/* in out */
	      unsigned int	lgel,		/* in */
	      unsigned int*	nbel,		/* in out */
	      int*		outIndex);	/* out */
#endif
