
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* pour gérer les va_list */
#include <exploit.h>  /* pour CODE_FIN_ERREUR_APPLICATIVE */
#include "Erreur.h"
#include "inout.h"
#include "Utile.h"
#include "Message.h"

#include "Counts_basics.h"


////////////////////////////////////////////////////////////////////////
// allocation contexte
int Counts_new  (Counts** counts)
{
  int   ret = 0;
  IN();
  *counts = (Counts*) malloc(sizeof(Counts));
  if (*counts == NULL) {
    Message_print(LOG, "%s() : Impossible d'allouer un contexte", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  Message_printDebugMem(TAG_MALLOC,*counts,1,sizeof(Counts),sizeof(Counts));
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// désallocation contexte
int Counts_free (Counts** counts)
{
  IN();
  int   ret = 0;
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"*counts",*counts);
  if (*counts) free(*counts);
  *counts = NULL;
 XIT:
  OUT(ret);
  return ret;
}

//*****************************************************************************
// creation / reset d'un compteur
int Counts_set (Counts* counts, char* name)
{
  int   ret = 0;
  int   idx = -1;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (strlen(name) >= L_Counts_name) {
    Erreur_set (CLA_APP, nomFonction, "nom trop long");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  idx = Counts_getIdxCountByName(counts, name);
  if (-1 == idx) {
    if (counts->nb < N_Counts_max) {
      idx = counts->nb;
      counts->nb += 1;
      strcpy(counts->name[idx], name);
      counts->count[idx] = 0;
    }
    else {
      Erreur_set (CLA_APP, nomFonction, "Le nb max [%d] de compteurs permis est atteint",counts->nb);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  else {
    Erreur_set (CLA_APP, nomFonction, "Le count existe déjà, name = %s, idx = %d",
                                      counts->name[idx], idx);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return idx;
}


//*****************************************************************************
int Counts_reset (Counts* counts, char* name)
{
  int   ret = 0;
  int   idx = -1;
  IN();
  idx = Counts_getIdxCountByName(counts, name);
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (-1 == idx) {
    Erreur_set (CLA_APP, nomFonction, "Le count n'existe pas, name = %s, idx = %d",
        counts->name[idx], idx);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else {
    counts->count[idx] = 0;
  }
  XIT:
  OUT(ret);
  return idx;
}


//*****************************************************************************
int Counts_resetAll (Counts* counts)
{
  IN();
  int   ret = 0;
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (counts,0,sizeof(Counts));
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
// conservé pour compatibilité ascendante
int Counts_setNewCount (Counts* counts, char* name)
{
  return Counts_set (counts, name);
}


//*****************************************************************************
int Counts_getValByName (Counts* counts, char* name)
{
  return Counts_getCountByName (counts, name);
}

//*****************************************************************************
int Counts_getCountByName (Counts* counts, char* name)
{
  int   ret = 0;
  int   idx = -1;
  int   nb  = 0;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  idx = Counts_getIdxCountByName(counts, name);
  if (idx >= 0) nb = counts->count[idx];
  else nb = 0;
 XIT:
  OUT(ret);
  return nb;
}


//*****************************************************************************
int Counts_getIdxCountByName (Counts* counts, char* name)
{
  int   ret = 0;
  int   idx = -1;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (NULL == name) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (counts->nb<=0) {
    idx = -1;
  }
  else {
    int i;
    for (i=0;i<counts->nb;i++)
    {
      if (strcmp(counts->name[i], name) == 0) {idx = i; break;}
    }
    if (i>=counts->nb) { idx = -1; } // pas trouvé
  }
 XIT:
  OUT(ret);
  if (ret != 0) idx = -1;
  return idx;
}


//*****************************************************************************
int Counts_setVal (Counts* counts, char* name, int nb)
{
  int   ret = 0;
  int   idx = -1;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  idx = Counts_getIdxCountByName(counts, name);
  if (-1 == idx) {
    idx = Counts_setNewCount(counts, name);
  }
  counts->count[idx] = nb;
 XIT:
  OUT(ret);
  return idx;
}


//*****************************************************************************
int Counts_plus1 (Counts* counts, char* name)
{
  int   ret = 0;
  int   idx = -1;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  idx = Counts_getIdxCountByName(counts, name);
  if (-1 == idx) {
    idx = Counts_setNewCount(counts, name);
  }
  counts->count[idx] += 1;
 XIT:
  OUT(ret);
  return idx;
}


//*****************************************************************************
int Counts_minus1 (Counts* counts, char* name)
{
  int   ret = 0;
  int   idx = -1;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  idx = Counts_getIdxCountByName(counts, name);
  if (-1 == idx) {
    idx = Counts_setNewCount(counts, name);
  }
  counts->count[idx] -= 1;
 XIT:
  OUT(ret);
  return idx;
}


//*****************************************************************************
// affichage des compteurs
int Counts_print (Counts* counts)
{
  IN();
  int   ret = 0;
  char  emptyName[] = "<unknown counter>";
  char* padding = "...........................................................";
  int padding_len = 0;
  padding_len = strlen (padding);
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (counts->nb>=0)
  {
    int i;
    int lmaxName = 0;
    int lName = 0;
    int lName_trunc = 0;
    for (i=0;i<counts->nb;i++)
    {
      lName = strlen(counts->name[i]);
      if (lName == 0) lName = strlen(emptyName);
      lmaxName = (lmaxName < lName) ? lName : lmaxName;
    }
    // au final, le nom ne doit pas dépasser la longueur du padding
    lmaxName = (lmaxName > padding_len) ? padding_len : lmaxName;

    Message_print (LOG, "[%d] Compteurs", counts->nb);
    //for (i=0;i<counts->nb;i++)
    //{
    //  Message_print (LOG, "[%2d] %-*s ... : %d", i, lmaxName, counts->name[i], counts->count[i]);
    //}
    for (i=0;i<counts->nb;i++)
    {
      lName = strlen(counts->name[i]);
      if (lName == 0) lName = strlen(emptyName);
      lName_trunc = (lName > lmaxName) ? lmaxName : lName;
      Message_print (LOG, "[%2d] %.*s %.*s : %10d", i, lmaxName, (counts->name[i] == "") ? emptyName : counts->name[i], 2+lmaxName-lName_trunc, padding, counts->count[i]);
    }
  }
 XIT:
  OUT(ret);
  return ret;
}



//*****************************************************************************
int Counts_getNb (Counts* counts)
{
  int   ret = 0;
  IN();
  if (NULL == counts) {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else
  {
    ret = counts->nb;
  }
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
// restitution du nom et de la valeur d'un compteur par son indice (de 0 à n-1)
int Counts_getCountByIdx (Counts* counts, int idx, char* name, int* val)
{
  int   ret = 0;
  IN();
  if (NULL == counts)
  {
    Erreur_set (CLA_APP, nomFonction, "counts est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (idx > (counts->nb-1))
  {
    Erreur_set (CLA_APP, nomFonction, "indice inconnu");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // pour faire simple, on parcourt le tableau jusqu'à tomber sur le bon indice
  int i = 0;
  while (i < idx && i < counts->nb) i++;
  if (i < counts->nb)
  {
    strcpy (name, counts->name[i]);
    *val = counts->count[i];
  }
 XIT:
  OUT(ret);
  return ret;
}

