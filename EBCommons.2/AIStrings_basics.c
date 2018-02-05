#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <exploit.h>  /* pour CODE_FIN_ERREUR_APPLICATIVE */

#include "Erreur.h"
#include "Message.h"
#include "inout.h"
#include "Utile.h"
#include "File32.h"

// Acces au contexte "Library"
#include "AIStrings_struct.h"
#include "AIStrings_basics.h"



//************************************************************************
// taille statique (hors malloc des pointeurs internes) des types du module
//************************************************************************
void AIStrings_printSizeof(void)
{
  IN();
  Message_printDebug(LOG,"Empreinte mémoire des types IAChar_**** :");
  Message_printDebug(LOG,"sizeof(AIStrings) = %d", sizeof(AIStrings));
  OUT(0);
  return;
}

//************************************************************************
// constructeur
//************************************************************************
AIStrings* AIStrings_new(int nb, size_t vol)
{
  AIStrings* ais = NULL;
  IN();
  if ((ais = malloc (sizeof (AIStrings))) == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Probleme d'allocation memoire");
  }
  else {
    memset (ais, 0, sizeof (AIStrings));
  }
  Message_printDebugMem(TAG_MALLOC,ais,1,sizeof(AIStrings),sizeof(AIStrings));
  if (ais != NULL) {
    ais->nbAlloc  = nb;
    ais->nbUsed   = 0;
    ais->icur     = 0;
    ais->volAlloc = vol+(size_t)(nb+nb);   // champs + separateurs 0x00 + un peu de marge
    ais->volUsed  = 0;
    // allocation de la collection de pointeurs
    if ((ais->offsets = (int*) malloc(sizeof(int)*nb)) == NULL) {
      Erreur_set (CLA_APP, nomFonction, "Probleme d'allocation memoire");
    }
    else {
      memset (ais->offsets, 0, sizeof(int)*nb);
    }
    Message_printDebugMem(TAG_MALLOC,ais->offsets,nb,sizeof(int),sizeof(int)*nb);
    // allocation du buffer de char
    if ((ais->s = malloc(ais->volAlloc)) == NULL) {
      Erreur_set (CLA_APP, nomFonction, "Probleme d'allocation memoire");
    }
    else {
      memset (ais->s, 0, ais->volAlloc);
      ais->offsets[0] = 0;
    }
    Message_printDebugMem(TAG_MALLOC,ais->s,1,ais->volAlloc,ais->volAlloc);
  }
  OUT(0);
  return ais;
}

//************************************************************************
// ajoute un char* (= 1 ligne)  au AIStrings
int AIStrings_append(AIStrings *ais, char *sIn0)
{
  IN();
  int ret = 0;
  if (sIn0 == NULL) {
    Erreur_set(CLA_APP, nomFonction, "Aucune donnée à insérer");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (ais->nbUsed + 1 > ais->nbAlloc) {
    Erreur_set(CLA_APP, nomFonction, "Tableau de char* alloué insuffisant");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // on efface la fin de ligne
  size_t sIn_sz0 = strlen(sIn0);
  char *sIn = NULL;
  sIn = calloc(sIn_sz0,sizeof(char)+3); // un peu de marge pour le caractere de fin de ligne
  if (sIn == NULL) {
    Erreur_set(CLA_APP, nomFonction, "Allocation %s impossible", sIn);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memcpy(sIn, sIn0, (size_t)sIn_sz0);
  if (sIn[sIn_sz0-1] == '\n' || sIn[sIn_sz0-1] == '\r')
    sIn[sIn_sz0-1] = 0x00;
  if (sIn[sIn_sz0-2] == '\n' || sIn[sIn_sz0-2] == '\r')
    sIn[sIn_sz0-2] = 0x00;
  // nouvelle taille
  size_t sIn_sz1 = strlen(sIn);
  int sep_vol = +1;  // pour separateur 0x00
  if (ais->volUsed + sIn_sz1 + sep_vol > ais->volAlloc) {
    Erreur_set(CLA_APP, nomFonction, "Espace alloué insuffisant");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // on ajoute la chaine à la fin
  strncpy(&ais->s[ais->offsets[ais->nbUsed]], sIn, sIn_sz1);
  // on positionne l'offset de l'élément suivant, dans la limite de la taille du tableau des offsets
  if ((ais->nbUsed+1)<ais->nbAlloc) {
    ais->offsets[ais->nbUsed+1] = ais->offsets[ais->nbUsed] + sIn_sz1 + sep_vol;
  }
  ais->nbUsed +=1;
XIT:
  if (sIn != NULL) free(sIn);
  OUT(ret);
  return ret;
}

//************************************************************************
// destructeur
//************************************************************************
int AIStrings_free(AIStrings **ais)
{
  IN();
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"(*iac)->s",(*ais)->s);
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"(*iac)->offsets",(*ais)->offsets);
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"*iac",*ais);
  if (*ais != NULL) {
    if ((*ais)->offsets != NULL) {
      // destruction des messages d'erreur éventuels
      free ((*ais)->offsets);
    }
    if ((*ais)->s != NULL) {
      // destruction des messages d'erreur éventuels
      free ((*ais)->s);
    }
    free (*ais);
    *ais = NULL;
  }
  OUT(0);
  return 0;
}

//************************************************************************
//* Recycle l'espace memoire occupé par une structure de données         *
//* précedemment allouée                                                 *
//* Permet d'éviter d'allouer et de désallouer constamment la même       *
//* structure de données, car cela coûte un appel système et de          *
//* réutiliser l'espace mémoire en le nettoyant                          *
//************************************************************************
int AIStrings_recycle(AIStrings *ais)
{
  int   ret = 0;
  IN();
  if (ais == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Impossible de recycler une structure NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  else {
    ais->nbUsed   = 0;
    ais->icur     = 0;
    ais->volUsed  = 0;
    if (ais->offsets != NULL && ais->nbAlloc > 0) {
      memset (ais->offsets, 0, sizeof (int)*ais->nbAlloc);
    }
    if (ais->s != NULL && ais->volAlloc > 0) {
      memset (ais->s, 0, ais->volAlloc);
    }
  }
  OUT(ret);
  return ret;
}


//************************************************************************
// retourne la ligne suivante
char* AIStrings_fetch(AIStrings *ais)
{
  IN();
  int ret = 0;
  char* s = NULL;
  if (ais->icur > ais->nbUsed) {
    s = NULL;
    Erreur_set(CLA_APP, nomFonction, "Plus de données");
    ret = NO_MORE_DATA;
  }
  else {
    s = &ais->s[ais->offsets[ais->icur]];
    ais->icur += 1; // l'indice courant pointe sur l'élément suivant
  }
  OUT(ret);
  return s;
}


//************************************************************************
// remet l'indice de position au début
int AIStrings_rewind(AIStrings *ais)
{
  IN();
  int ret = 0;
  ais->icur = 0;
  OUT(ret);
  return ret;
}

//************************************************************************
// retourne l'indice de l'élément courant
int AIStrings_getIcur(AIStrings *ais)
{
  IN();
  int ret = ais->icur;
  OUT(ret);
  return ret;
}

//************************************************************************
// retourne l'indice du dernier element
int AIStrings_getLastPos(AIStrings *ais)
{
  IN();
  int ret = ais->nbUsed-1;
  OUT(ret);
  return ret;
}

///////////////////////////////////////////////////////////////////////////////
// retourne le dernier element
char* AIStrings_getLast(AIStrings *ais)
{
  IN();
  int ret = 0;
  char* p = &ais->s[ais->offsets[ais->nbUsed-1]];
  OUT(ret);
  return p;
}

///////////////////////////////////////////////////////////////////////////////
// retourne la ligne à la position de l'indice
char* AIStrings_getDataAt(AIStrings *ais, int i)
{
  IN();
  int ret = 0;
  char* p = NULL;
  if (i > ais->nbUsed) {
    Erreur_set(CLA_APP, nomFonction, "Aucune donnée à la position %d", i);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  else {
    p = ais->s + ais->offsets[i];
  }
  OUT(ret);
  return p;
}

///////////////////////////////////////////////////////////////////////////////
// retourne le nb d'enregs
int AIStrings_getNb(AIStrings *ais)
{
  IN();
  int ret = ais->nbUsed;
  OUT(ret);
  return ret;
}


