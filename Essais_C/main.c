#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <ctype.h>

void fonctionFils (char* p1)
{
  p1 = toupper((int)p1);
  /* toupper() est une fonction standard C pour le passage en majuscule */
}

void fonctionPere (void)
{
  const char* toto = "toto";
  printf("toto=%s\n",toto);
  fonctionFils(toto);
  printf("toto=%s\n",toto);
}


int main (int argc, char ** argv)
{
  fonctionPere();
  return 0;
}
