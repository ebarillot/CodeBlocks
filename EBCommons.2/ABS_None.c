
#include "ABS.h"

// Implémentation d'une valeur None sous la forme d'un ensemble de 8 octets
char __ABS_None[8] = "None\0\0\0";  // 4 +3 zéros +1 fin de chaine
any_t ABS_None = (any_t)__ABS_None;

