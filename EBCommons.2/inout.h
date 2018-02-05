#ifndef InOut_H
#define InOut_H

#include "exploit.h"

#ifndef nomFonction
#define nomFonction   (char*)__func__
#endif

#if 0
void inFun (char* fun);
void outFun (char* fun, int ret);
void resetInOut (void);

#define IN() inFun(nomFonction)
#define OUT(ret) outFun(nomFonction, ret)

#else

#define IN()
#define OUT(ret)

#endif

#endif
