#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int couting_bits_KR_uli(unsigned long int v);
unsigned int couting_bits_KR_ui(unsigned int v);
unsigned int couting_bits_KR_uli_a(unsigned long int * v, unsigned int nw);


typedef struct {
#define byte(n) \
  unsigned b##n##0  : 1; \
  unsigned b##n##1  : 1; \
  unsigned b##n##2  : 1; \
  unsigned b##n##3  : 1; \
  unsigned b##n##4  : 1; \
  unsigned b##n##5  : 1; \
  unsigned b##n##6  : 1; \
  unsigned b##n##7  : 1
  byte(0);
  byte(1);
  byte(2);
  byte(3);
  byte(4);
  byte(5);
  byte(6);
  byte(7);
} Bv;

typedef union {
  unsigned int word32;
  unsigned long int word64;
  Bv bv;
} Value;

int main()
{
  #define N_word64 2
  Value v[N_word64];
  unsigned int  c = 0;
  memset(v,0,sizeof(unsigned long int)*N_word64);
  printf("sizeof(unsigned int) = %ld\n",sizeof(unsigned int));
  printf("sizeof(unsigned long int) = %ld\n",sizeof(unsigned long int));


  #define set_byte(v,n,x0,x1,x2,x3,x4,x5,x6,x7) \
  v.bv.b##n##0=x0; v.bv.b##n##1=x1; v.bv.b##n##2=x2; v.bv.b##n##3=x3, v.bv.b##n##4=x4; v.bv.b##n##5=x5; v.bv.b##n##6=x6; v.bv.b##n##7=x7
  set_byte(v[0],0,0,1,0,0,0,0,0,1);
  set_byte(v[0],1,0,0,1,0,0,0,0,1);
  set_byte(v[0],2,0,0,0,1,0,0,0,1);
  set_byte(v[0],3,0,0,0,0,1,0,0,1);
  set_byte(v[0],4,0,0,0,0,0,1,0,1);
  set_byte(v[0],5,0,0,0,0,0,0,1,1);
  set_byte(v[0],6,0,0,0,0,0,1,0,1);
  set_byte(v[0],7,1,0,0,0,1,0,0,1);

  set_byte(v[1],0,0,1,0,0,0,0,0,1);
  set_byte(v[1],1,0,0,1,0,0,0,0,1);
  set_byte(v[1],2,0,0,0,1,0,0,0,1);
  set_byte(v[1],3,0,0,0,0,1,0,0,1);
  set_byte(v[1],4,0,0,0,0,0,1,0,1);
  set_byte(v[1],5,0,0,0,0,0,0,1,1);
  set_byte(v[1],6,0,0,0,0,0,1,0,1);
  set_byte(v[1],7,1,0,0,0,1,0,0,1);

  #define print_byte(v,n) printf("%d %d %d %d %d %d %d %d\n", \
  v.bv.b##n##0,v.bv.b##n##1,v.bv.b##n##2,v.bv.b##n##3,v.bv.b##n##4,v.bv.b##n##5,v.bv.b##n##6,v.bv.b##n##7)
  {
    unsigned int inw = 0;
    for (inw=0; inw<N_word64; inw++) {
      print_byte(v[inw],0);
      print_byte(v[inw],1);
      print_byte(v[inw],2);
      print_byte(v[inw],3);
      print_byte(v[inw],4);
      print_byte(v[inw],5);
      print_byte(v[inw],6);
      print_byte(v[inw],7);
      printf("\n");
    }
  }

  printf("Couting bits set to 1\n");
  printf("v = %u\n",v[0].word32);
  c = couting_bits_KR_ui(v[0].word32);
  printf("c : %u\n", c);

  printf("v = %lu\n",v[0].word64);
  c = couting_bits_KR_uli(v[0].word64);
  printf("c : %u\n", c);

  {
    unsigned int inw = 0;
    for (inw=0; inw<N_word64; inw++) {
      printf("v[%u] = %lu\n",inw,v[inw].word64);
    }
  }
  c = couting_bits_KR_uli_a(&v[0].word64,N_word64);
  printf("c : %u\n", c);

  printf("\n");
  return 0;
}
