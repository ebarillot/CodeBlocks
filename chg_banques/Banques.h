#ifndef Banques_H
#define Banques_H

#include "ConnexionOracle.h"
/* le nb de lignes dans le tableau traitées lors des opérations par
 * tableau avec Oracle (array-fetch, array-insert) */
#define	N_banques	50000

/*
 * requete qui permet de générer les déclarations à partir des colonnes de
 * la base
 *
 select '#define	L_'||lower(column_name)||'		'||
 data_length from user_tab_columns where table_name='BANQUES'
 order by column_id
 *
 */
#define L_bancod                5
#define L_bannom                40
#define L_bannom10              10
#define L_banentnumtyp          22
#define L_banentnum             15
#define L_bansup                12
#define L_banoricod             12
#define L_bansupportcod         12
#define L_bannumsupport         30
#define L_bannuminf             16
#define L_bandateff             7
#define L_baneffpre             1
#define L_banpoidateff          12
#define L_bandatpub             7
#define L_banpubpre             1
#define L_bandatord             7
#define L_bantypcharcod         12
#define L_bannomtrt             50
#define L_banredactcod          12

/* requete qui sert à générer les déclarations
 select 'char    '||lower(column_name)||'[L_'||lower(column_name)||'+1]'
  from user_tab_columns
  where table_name='BANQUES'
 order by column_id
 */
typedef struct {
  char    bancod[N_banques][L_bancod+1];
  char    bannom[N_banques][L_bannom+1];
  char    bannom10[N_banques][L_bannom10+1];
  char    banentnumtyp[N_banques][L_banentnumtyp+1];
  char    banentnum[N_banques][L_banentnum+1];
  char    bansup[N_banques][L_bansup+1];
  char    banoricod[N_banques][L_banoricod+1];
  char    bansupportcod[N_banques][L_bansupportcod+1];
  char    bannumsupport[N_banques][L_bannumsupport+1];
  char    bannuminf[N_banques][L_bannuminf+1];
  char    bandateff[N_banques][L_bandateff+1];
  char    baneffpre[N_banques][L_baneffpre+1];
  char    banpoidateff[N_banques][L_banpoidateff+1];
  char    bandatpub[N_banques][L_bandatpub+1];
  char    banpubpre[N_banques][L_banpubpre+1];
  char    bandatord[N_banques][L_bandatord+1];
  char    bantypcharcod[N_banques][L_bantypcharcod+1];
  char    bannomtrt[N_banques][L_bannomtrt+1];
  char    banredactcod[N_banques][L_banredactcod+1];
} BanquesData;

typedef struct {
  short    bancod[N_banques];
  short    bannom[N_banques];
  short    bannom10[N_banques];
  short    banentnumtyp[N_banques];
  short    banentnum[N_banques];
  short    bansup[N_banques];
  short    banoricod[N_banques];
  short    bansupportcod[N_banques];
  short    bannumsupport[N_banques];
  short    bannuminf[N_banques];
  short    bandateff[N_banques];
  short    baneffpre[N_banques];
  short    banpoidateff[N_banques];
  short    bandatpub[N_banques];
  short    banpubpre[N_banques];
  short    bandatord[N_banques];
  short    bantypcharcod[N_banques];
  short    bannomtrt[N_banques];
  short    banredactcod[N_banques];
} BanquesInd;

typedef struct {
  BanquesData data;
  BanquesInd  ind;
  int           nbData;
  int           nbFound;
} Banques;


Banques*	newBanques(void);
int	freeBanques(Banques* banques);

int	getIndBanques(Banques* banques, int iInd);
int	getMaxBanques(void);
int	nbDataBanques(Banques* banques);
int	nbFoundBanques(Banques* banques);
int	afficheBanques(Banques* banques);
int	resetBanques(Banques*	banques);
int	selectBanques(ConnexionOracle*	cox, /* in */
		      Banques*	banques); /* out */

#endif
