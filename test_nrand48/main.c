#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double cal(long N);

int main(int argc, char* argv[])
{
    long N = atol(argv[1]);
    printf("Hello world!\n");
    printf("RAND_MAX=%d\n",RAND_MAX);
    printf("rand()=%d\n",rand());
    //printf("%f\n",cal(N));

    return 0;
}


double cal(long N)
{
    double x  = 0.L;
    double y  = 0.L;
    double x2 = 0.L;
    double y2 = 0.L;
    double z  = 0.L;
    x  = 0.L;
    y  = 0.L;
    x2 = 0.L;
    y2 = 0.L;
    z  = (double)(N)*rand()/RAND_MAX;
    return z*(x+y)/(x2+y2);
}
