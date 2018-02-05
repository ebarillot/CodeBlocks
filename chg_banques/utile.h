#ifndef util_H
#define util_H

#include <sys/time.h>
#include <unistd.h>


#define	L_afficheMessage	200000

#define	LOG	"LOG"
#define	MEM	"MEM"
#define	ERR	"ERR"
#define	BAD	"BAD"
#define	WARN	"WARNING"



#define	IN(fct)		afficheMessage(3,LOG,"{ -------- IN -------- %s\n",fct)
#define	OUT(fct,ret)	afficheMessage(3,LOG,"} -------- OUT -------- %s [%d]\n",fct,ret)
/* #define	IN(fct)		afficheMessage(2,LOG,">>>>>>>> IN >>>>>>>> %s\n",fct) */
/* #define	OUT(fct,ret)	afficheMessage(2,LOG,"<<<<<<<< OUT <<<<<<<< %s [%d]\n",fct,ret) */

int          initNiveauTrace (int niveau);
int          getNiveauTrace (void);
int          afficheMessage (int niveau, char* tag, char* fmt, ...);
int          flushEcritures (void);
char*        myRtrim (char* str);
char*        myLtrim (char* str);
void         masquerSignaux (void);
void         demasquerSignaux (void);
char*        getAnneeMoisJour (void);
char*        getHeure (void);
char*        newString (long lg);
char*	     reallocString(char* str, long lg);
char*        noEOL (char* str);
char*        convPct1 (char* str);
char*        convDate14To8 (char* str);
char*        convDateSlashToYYYYMMDD (char* str);
char*        convDateYYYYMMDDToSlash (char* str);
char*        convDateYYYYToYYYYMMDD (char* str);
char*        convCommaToPoint(char* in);
char*        dropComma(char* in);
char*        convTvaToInt(char* in);
char*        convTvaToComma(char* in);
char*        convTvaToPct1(char* in);

#endif
