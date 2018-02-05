#include <signal.h>
#include <unistd.h>   // getpid()
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <time.h>   // pour timespec et clock_gettime()

// pour éviter de déclarer la variable comme extern ici
#define __LRUBuf_C__
#include "LRUBuf.h"
int  __LRUBuf_for_SIGUSR1_flag__  = 0;
char __LRUBuf_for_SIGUSR1_msg__[] = "Flag du module LRUBuf activé, impossible de gérer plus de buffers !";


// structure de l'unique buffer statique
#define L_lineSize    1000
#define N_lines       1000
static char _LRUBuf[N_lines][L_lineSize];     // _variable privée
size_t _cur_line = (size_t)(-1);              // _variable privée


// structure d'un buffer dynamique: nb de lignes et taille de chaque ligne sont dynamiques
// Cependant, toutes les lignes ont la même taille
typedef struct _LRUBufDyn {
  size_t  nlines;
  size_t  szline;
  size_t  cur;
  char*   buf;
} LRUBufDyn;
// tableau des id de buffers dynamiques
static LRUBufDyn _LRUBufDyn[N_LRUBufId];    // _variable privée
// indice du dernier buffer dynamique créé
LRUBufId _nbLRUBufId = (LRUBufId)(0);       // _variable privée
                                            // la valeur 0 est réservée pour le buffer par défaut



///////////////////////////////////////////////////////////////////////////////
LRUBufId LRUBuf_init(unsigned short nlines, unsigned short szline)
{
  if (_nbLRUBufId >= N_LRUBufId) {
    printf ("\nErreur: NO SPACE LEFT too many LRUBBuf requested [already %d/%d slots occupied] !\n", _nbLRUBufId, N_LRUBufId);
    fflush(stdout);
    __LRUBuf_for_SIGUSR1_flag__ = 1;
    kill(getpid(),SIGUSR1);
  }
  _nbLRUBufId += 1;
  _LRUBufDyn[_nbLRUBufId].nlines = nlines;
  _LRUBufDyn[_nbLRUBufId].szline = szline;
  _LRUBufDyn[_nbLRUBufId].cur    = (size_t)(-1);
  _LRUBufDyn[_nbLRUBufId].buf    = (char*)calloc(nlines,szline);
  return _nbLRUBufId;
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// libération d'un LRUBuf
void LRUBuf_free(LRUBufId id)
{
  if (_nbLRUBufId > N_LRUBufId) return;
  switch (id) {
    case DEFAULT_LRUBufId:
    {
      // on libère tous les LRUBuffers créés
      LRUBufId id;
      for (id=DEFAULT_LRUBufId+1;id<=_nbLRUBufId;id++) {
        if (_LRUBufDyn[id].buf !=  NULL) free(_LRUBufDyn[id].buf);
      }
      _nbLRUBufId = 0;
      memset(_LRUBufDyn, 0, sizeof(LRUBufDyn)*N_LRUBufId);
      return;
    }
    default:
      if (id > _nbLRUBufId) return;
      // on libère celui dont l'id est passé
      // ça ne change par contre rien au nb total de places proses dans le tableau des id
      // TODO : faire un genre de compactage des id ? => ça veut dire gérer des id internes
      // TODO   compactables et des id externes communiqués aux appelants
      free(_LRUBufDyn[id].buf);
      memset(&_LRUBufDyn[id], 0, sizeof(LRUBufDyn));
      return;
  }
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// nettoyage du buffer statique
void LRUBuf_clean(LRUBufId id)
{
  switch (id) {
    case DEFAULT_LRUBufId:
      {
        memset(_LRUBuf,0,N_lines*L_lineSize);
        return;
      }
    default:
      if (id > _nbLRUBufId) return;
      memset(_LRUBufDyn[id].buf,0,_LRUBufDyn[id].nlines*_LRUBufDyn[id].szline);
      return;
  }
}


///////////////////////////////////////////////////////////////////////////////
// retourne les dimensions du buffer LRU indiqué
size_t LRUBuf_get_nlines(LRUBufId id)
{
  switch (id) {
    case DEFAULT_LRUBufId:
    {
      return N_lines;
    }
    default:
      if (id > _nbLRUBufId) return;
      return _LRUBufDyn[_nbLRUBufId].nlines;
  }
}

size_t LRUBuf_get_lineSize(LRUBufId id)
{
  switch (id) {
    case DEFAULT_LRUBufId:
    {
      return L_lineSize;
    }
    default:
      if (id > _nbLRUBufId) return;
      return _LRUBufDyn[_nbLRUBufId].szline;
  }
}


///////////////////////////////////////////////////////////////////////////////
// Retourne une ligne dispo dans le buffer
char* LRUBuf_get(LRUBufId id)
{
  switch (id) {
    case DEFAULT_LRUBufId:
    {
      struct timespec  TM_beg;
      struct timespec  TM_end;
      if (_cur_line==0) clock_gettime(CLOCK_MONOTONIC, &TM_beg);
      if (_cur_line==N_lines-1) {
        clock_gettime(CLOCK_MONOTONIC, &TM_end);
        long long sec  = (long long) TM_end.tv_sec  - (long long) TM_beg.tv_sec;
//    long long nsec = (long long) TM_end.tv_nsec - (long long) TM_beg.tv_nsec ;
        if (sec <= 1) printf("*** WARNING TOO FAST use of %s () ***",__func__);
      }
      _cur_line = (_cur_line+1)%N_lines;  // Least recently used (ou Round Robin) c'est ici !
      memset(_LRUBuf[_cur_line],0,L_lineSize);
      return _LRUBuf[_cur_line];
    }
    default:
      if (id > _nbLRUBufId) return;
      struct timespec  TM_beg;
      struct timespec  TM_end;
      if (_LRUBufDyn[id].cur==0) clock_gettime(CLOCK_MONOTONIC, &TM_beg);
      if (_LRUBufDyn[id].cur == _LRUBufDyn[id].nlines-1) {
        clock_gettime(CLOCK_MONOTONIC, &TM_end);
        long long sec  = (long long) TM_end.tv_sec  - (long long) TM_beg.tv_sec  ;
        //    long long nsec = (long long) TM_end.tv_nsec - (long long) TM_beg.tv_nsec ;
        if (sec <= 1) printf("*** WARNING TOO FAST use of %s () ***",__func__);
      }
      _LRUBufDyn[id].cur = (_LRUBufDyn[id].cur+1)%_LRUBufDyn[id].nlines;  // Least recently used (ou Round Robin) c'est ici !
      memset(_LRUBufDyn[id].buf+_LRUBufDyn[id].cur*_LRUBufDyn[id].szline,0,_LRUBufDyn[id].szline);
      return _LRUBufDyn[id].buf+_LRUBufDyn[id].cur*_LRUBufDyn[id].szline;
  }
}
