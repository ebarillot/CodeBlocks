#include <stdio.h>
#include <stdlib.h>


struct params {
  double  a;
  double  b;
  double  sig;
  double  d;
  double  real_size_factor;
  double  real_size;
  int     discret_size; // size of the 2D grid
  double  dx;           // space step
  double  T;            // total time
  double  dt;           // time step h <= (dx^2/2D) avec D~1/d (d est le plus petit coeff de diffusion)
  int     n;
};


void init_params(struct params* p)
{
  // parametres du modele de Lengyel-Epstein
  p->a = 9.;
  p->b = 0.14;
  p->sig = 50.;
  p->d = 1.07;

  p->real_size_factor = 1;
  p->real_size = 70.*p->real_size_factor;
  p->discret_size = 100*p->real_size_factor;    // size of the 2D grid
  p->dx = p->real_size/p->discret_size;         // space step
  // p->T = 1.0;                                //total time
  p->T = 30000.0;                               // total time
  p->dt = 2. * p->d*(p->dx)*(p->dx)/2./p->sig;  // time step h <= (dx^2/2D) avec D~1/d (d est le plus petit coeff de diffusion)
  p->n = (int)(p->T/p->dt);                     //
}

double* laplacian(double* lap, double* u, int nx, int ny, double dx)
{
//  u_top    = u[0:-2, 1:-1]
//  u_left   = u[1:-1, 0:-2]
//  u_bottom = u[2:, 1:-1]
//  u_right  = u[1:-1, 2:]
//  u_center = u[1:-1, 1:-1]
//  (u_top + u_left + u_bottom + u_right - 4 * u_center) / dx*dx

  return lap;

}


int main()
{
    printf("Hello world!\n");
    return 0;
}
