#ifndef Message_H
#define Message_H

#define ETAPE "ETAPE"
#define LOG   "LOG"
#define ERR   "ERR"
#define BAD   "BAD"
#define WARN  "WARN"

#define TAG_MALLOC  "MALLOC"
#define TAG_FREE    "FREE"

void    Message_setContexte (int flagLog, int debug, int debugMem);
void    Message_setFunLog (void (*funLog)());
void    Message_setFunLogDefault (void);
void    Message_setFunPrint (void (*funPrint)());
void    Message_setFunPrintDefault (void);
int     Message_print (char* tag, char* fmt, ...);
int     Message_printDebug (char* tag, char* fmt, ...);


// affichage particulier des adresses mémoires
#define  Message_printDebugMem(TAG,var,nbelem,szelem,size) \
  Message_printDebug(TAG,"%s(), %s : %p-%p, %d x %d = %d",nomFonction,#var,(void*)var,((void*)var)+(size),(nbelem),(szelem),(size))


#endif
