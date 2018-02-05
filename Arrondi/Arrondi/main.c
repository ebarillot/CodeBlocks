#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Arrondi un nombre en fonction du nb de chiffres significatifs souhaités
// Le chiffre significatif le plus à droite est 0 ou 5
// L'arrondi se fait toujours par excès
void Arrondi5ll (long long nbbrut, long long* nbarrondi, const unsigned int nbsignif);
// la même chose mais avec une chaine de caracteres en entree
void Arrondi5c (char *zone, const unsigned int nbsignif);
// la même chose mais avec une chaine de caracteres en entree
// et qui travaille sans la convertir en nombre
void Arrondi5s (char *zone, const unsigned int nbsignif);

int main()
{
  printf("Hello world!\n");
  unsigned int nbsignif  = 2;   // nb de chiffres significatifs qu'on veut garder: 100 = 10^2 => 2 chiffres
  {
    printf("----------\n");
    char nombre[] = "126556";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "123556";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "120200";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "162543";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "121";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "-162543";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "-123556";
    Arrondi5c (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "123556";
    Arrondi5s (nombre, nbsignif);
  }
  {
    printf("----------\n");
    char nombre[] = "-123556";
    Arrondi5s (nombre, nbsignif);
  }
  return 0;
}


// Arrondi un nombre en fonction du nb de chiffres significatifs souhaités
// Le chiffre significatif le plus à droite est 0 ou 5
// L'arrondi se fait toujours par excès
// Le nombre à arrondir est dans une chaine de caracteres en entree
// et travaille sans la convertir en nombre
void Arrondi5s (char *zone, const unsigned int nbsignif)
{
  printf("Avant l'arrondi = %s\n",zone);
  if (zone == NULL || strlen(zone) == 0) return;
  {
    int i;
    for (i=0;i<strlen(zone);i++)
    {
      if (!isdigit(zone[i]))
      {
        if (i == 0 && zone[i] == '-') continue;
        printf("Le champ à arrondir n'est pas un nombre : %s\n", zone);
        // EB : Il se passe quoi ensuite pour l'appelant qui n'est pas
        // prévenu que l'arrondi n'est pas possible ???
        return;
      }
    }
  }
  {
    char out[100];    // pour ecrire provisoirement le nombre arrondi
    short un = 0;     // pour une éventuelle retenue finale
    memset(out,0,100);
    unsigned int deb = 0;
    unsigned int fin = nbsignif;
    if (zone[0]=='-') // nb négatif
    {
      deb = 1;
      fin = nbsignif+1;
      out[0] = '-';
    }
    if (zone[fin] >= 5)
    {
      out[fin] = '0';
      int i = fin-1;
      do
      {
        un = 0;
        out[i] = (zone[i]<'9')? zone[i]+1 : '0', un=1;  // attention au 9
        i -=1;
      } while (i>0 && un == 1);
      if (un == 1)
      {
        // on decale tous les chiffres d'une position vers la droite
        for (i=deb;i<strlen(out); i++)
        {
          out[i+1] = out[i];
        }
        out[deb] = '1';
      }
    }
    else
    {
      zone[fin] = '5';
    }
    // on complete le nombre avec des 0
    int i;
    for(i=strlen(out);i<strlen(zone);i++)
    {
      out[i] = '0';
    }
    strcpy(zone,out);
  }
  printf("Apres l'arrondi = %s\n",zone);
  return;
}

//********************************************************************
// Nouvelle version, à mon avis (EB) plus claire et plus facilement
// maintenable
// Ca me semble plus simple de calculer un arrondi avec des opérations
// mathématiques, aptes par nature à manipuler des nombres
//********************************************************************
void Arrondi5c (char *zone, const unsigned int nbsignif)
{
  long long nbbrut      = 0;
  long long nbarrondi   = 0;

  printf("Avant l'arrondi = %s\n",zone);
  if (zone == NULL || strlen(zone) == 0) return;
  {
    int i;
    for (i=0;i<strlen(zone);i++)
    {
      if (!isdigit(zone[i]))
      {
        if (i == 0 && zone[i] == '-') continue;
        printf("Le champ à arrondir n'est pas un nombre : %s\n", zone);
        // EB : Il se passe quoi ensuite pour l'appelant qui n'est pas
        // prévenu que l'arrondi n'est pas possible ???
        return;
      }
    }
  }
  // conversion du champ dont on est sûr qu'il ne contient que des chiffres en vrai nombre
  nbbrut = strtoll(zone,NULL,10);
  printf("nbbrut = %lld\n",nbbrut);
  Arrondi5ll(nbbrut, &nbarrondi, nbsignif);
  sprintf(zone, "%lld", nbarrondi);
  return;
}

//********************************************************************
// Arrondi un nombre en fonction du nb de chiffres significatifs souhaités
// Le chiffre significatif le plus à droite est 0 ou 5
// L'arrondi se fait toujours par excès
// EB règle issue du doc ChiffresCles_V0.5.doc
//  ex : CA de 234 529 000 euros ou  CA de 67 829 000 euros
//  Prendre les 2 premiers chiffres (ex= 23 ou 67)
//  Les arrondir au nombre multiple de 5 ou de 10, directement supérieur (ex = 25 ou 70)
//  {directement inférieur pour les RN négatifs (-23 ou -67 -> -20 ou -60)}
void Arrondi5ll (long long nbbrut, long long* nbarrondi, const unsigned int nbsignif)
{
  printf("Avant l'arrondi = %lld\n",nbbrut);
  long long   _nbbrut = llabs(nbbrut);
  if (_nbbrut < 1000LL)
  {
    *nbarrondi = 0;   // arrondi de force à 0
  }
  else
  {
    long long       _nbsigne      = (nbbrut>=0)?+1:-1;
    long long       _nbreduit     = 0;
    long long       _nbreduitdiv5 = 0;
    long long       og            = 1;
    const long long og_max        = 1000000000000000000;   // 10^18, avant qu'un montant atteigne ce nombre ...
    long long       og_reduit     = 0;
    long long       signif        = 0;
    long long       plus1         = 0;
    { // calcul du facteur qui correspond au nb de chiffres significatifs
      int i;
      signif = 1;
      for (i=0;i<nbsignif;i++) {signif *= 10;}
    }
    // calcul de l'ordre de grandeur (=og) du nombre
    // la limite og_max est là pour éviter une boucle infinie dans les cas pathologiques
    while (((long long)(_nbbrut/og)) > 0 && og < og_max) {og*=10;}
    og_reduit = og/signif;
    _nbreduit = _nbbrut/og_reduit;
    _nbreduitdiv5 = (long long)(_nbreduit/5);
    plus1 = (_nbsigne<0)? 0 : 1;
    _nbreduit = (_nbreduitdiv5+plus1)*5;
    *nbarrondi = _nbsigne * _nbreduit * og_reduit;
  }
  printf("Apres l'arrondi = %lld\n",*nbarrondi);
  return;
}
