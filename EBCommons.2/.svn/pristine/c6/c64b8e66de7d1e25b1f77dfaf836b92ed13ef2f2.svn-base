#ifndef BatchHelper_H
#define BatchHelper_H


#ifndef __BatchHelper_C__
// la variable n'est déclarée externe que en dehors de BatchHelper.c
// Elle permet à un programme qui est interrompu par un signal Linux / Unix
// de fournir une fonction à appeler pour transmettre ce signal
extern void (*BatchHelper_hook_setFlagKill)(int);
#endif


// Piège à signaux
void BatchHelper_setSignalstraps (void);

#endif
