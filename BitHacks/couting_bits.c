#include <stdio.h>
#include <stdlib.h>


unsigned int couting_bits_KR_ui(unsigned int v)
{
  unsigned int c = 0;
  for (c=0; v; c++) {
    v &= v - 1;
  }
  return c;
}

unsigned int couting_bits_KR_uli(unsigned long int v)
{
  unsigned int c = 0;
  for (c=0; v; c++) {
    v &= v - 1;
  }
  return c;
}

unsigned int couting_bits_KR_uli_a(unsigned long int * v, unsigned int nw)
{
  unsigned int ctemp = 0; // up to 4 billion bits
  unsigned int c = 0; // up to 4 billion bits
  unsigned int inw = 0;
  for (inw=0; inw<nw; inw++) {
    for (ctemp=0; v[inw]; ctemp++) {
      v[inw] &= v[inw] - 1;
    }
    c += ctemp;
  }
  return c;
}
