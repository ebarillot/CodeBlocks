#ifndef _Counts_struct_H
#define _Counts_struct_H

#include "Counts_defines.h"

typedef struct _Counts {
  int   count [N_Counts_max];
  char  name  [N_Counts_max][L_Counts_name+1];
  int   nb;
} Counts;


#endif
