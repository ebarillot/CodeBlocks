#ifndef BatchHelper_H
#define BatchHelper_H


#ifndef __BatchHelper_C__
// la variable n'est d�clar�e externe que en dehors de BatchHelper.c
// Elle permet � un programme qui est interrompu par un signal Linux / Unix
// de fournir une fonction � appeler pour transmettre ce signal
extern void (*BatchHelper_hook_setFlagKill)(int);
#endif


// Pi�ge � signaux
void BatchHelper_setSignalstraps (void);

#endif
