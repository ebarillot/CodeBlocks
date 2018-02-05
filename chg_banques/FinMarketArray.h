#ifndef FinMarketArray_H
#define FinMarketArray_H

#include "Entreprise.h"
#include "DirigBV.h"
#include "Bilan.h"
#include "FinBilseq.h"

/* #define _AVEC_BV */
#undef _AVEC_BV

#ifdef _AVEC_BV
  #define	NOM_TAB_FIN_MARKET	"fin_market_maj_bv"
  #define	TAB_FIN_MARKET		fin_market_maj_bv
#else
  #define	NOM_TAB_FIN_MARKET	"fin_market_maj"
  #define	TAB_FIN_MARKET		fin_market_maj
#endif

/* le nb de lignes dans le tableau traitées lors des opérations par
 * tableau avec Oracle (array-fetch, array-insert) */
#define	N_FinMarketArray	100

/*
 * requete qui permet de générer les déclarations à partir des colonnes de
 * la base
 *
 * select '#define	L_'||lower(column_name)||'		'||
 * data_length from user_tab_columns where table_name='FIN_MARKET_MAJ'
 * order by column_id
 *
 */
#define L_entnum                15
#define L_entnumtyp             22
#define L_etanic                5
/* données entreprises */
#define L_entbaseeur            1
#define L_entcreannee           22
#define L_entcremois            22
#define L_entcatjurcod          22
#define L_entcatjur2c           22
#define L_enttraeffcod          22
#define L_enteff                22
#define L_enttracap             22
#define L_enttraca              22
#define L_entrestrarapcab       22
#define L_enttracae             22
#define L_entnbreta             22
#define L_entnotort             22
#define L_entcotalp             1
#define L_entdenoff             260
#define L_etastatutcod          12
#define L_etaapecod             12
#define L_etaape3c              3
#define L_etaape2c              2
#define L_etatraeffcod          22
#define L_etaeff                22
#define L_etapayscod            22
#define L_etaregion             2
#define L_etadepart             3
#define L_etaarrond             4
#define L_etacanton             5
#define L_etacommune            5
#define L_etacodevoie           9
#define L_etapostal             5
#define L_etatel                1
#define L_etafax                1
#define L_etaobjsoc1            2000
#define L_etaobjsoc2            2000
#define L_etaobjsoc3            2000
/* les bilans */
#define L_ratbilseq           22
#define L_ratdatord           20
#define L_ratsup              12
#define L_ratdatclo           20
#define L_ratcloannee         22
#define L_ratduree            22
#define L_rattop              4
#define L_ratmax              4
#define L_ratoribilcod        12
#define L_ratdevisecod        22
#define L_ratmotnonsaicod     12
#define L_ratvalcod           12
#define L_ratunimoncod        12
#define L_ratrapgestcod       12
#define L_ratrapcomcod        12
#define L_ratannexecod        12
#define L_ratnafcod           12
#define L_ratgreffecod        22
#define L_ratnumcod           12
#define L_rattypbilcod        12
#define L_ratdatordbil        20
#define L_ratr1               22
#define L_ratr11              22
#define L_ratr13              22
#define L_ratr14              22
#define L_ratr16              22
#define L_ratr18              22
#define L_ratr19              22
#define L_ratr20              22
#define L_ratr21              22
#define L_ratr22              22
#define L_ratr28              22
#define L_ratr29              22
#define L_ratr30              22
#define L_ratr31              22
#define L_ratr32              22
#define L_ratr33              22
#define L_ratr35              22
#define L_ratr36              22
#define L_bilscbf             22
#define L_bilscbf3            22
#define L_bilscbh             22
#define L_bilscbh3            22
#define L_bilscbt             22
#define L_bilscbt3            22
#define L_bilscbx             22
#define L_bilscbx3            22
#define L_bilsccd             22
#define L_bilsccd3            22
#define L_bilsccf             22
#define L_bilsccf3            22
#define L_bilscco             22
#define L_bilscco3            22
#define L_bilscda             22
#define L_bilscdr             22
#define L_bilscdu             22
#define L_bilscdv             22
#define L_bilscdx             22
#define L_bilscdy             22
#define L_bilscfj             22
#define L_bilscfk             22
#define L_bilscfl             22
#define L_bilscfs             22
#define L_bilscft             22
#define L_bilscfu             22
#define L_bilscfv             22
#define L_bilscgg             22
#define L_bilscgv             22
#define L_bilscgw             22
#define L_bilschj             22
#define L_bilschn             22
#define L_bilschp             22
#define L_bilschq             22
#define L_bilsc0r             22
#define L_bilscua             22
#define L_bilscva             22
#define L_bilscvu             22
#define L_bilscvv             22
#define L_bilscvg             22
#define L_bilscvg2            22
#define L_bilscvh             22
#define L_bilscvh2            22
#define L_bilsc8a2            22
#define L_bilscvi             22
#define L_bilscyp             22
#define L_bilscyq             22
#define L_bilscyr             22
#define L_bilscys             22
#define L_bilscyt             22
#define L_bilscyu             22
/* les fonctions opérationnelles */
#define L_dirfamille            10
#define L_dirdir                10
#define L_dirfinadmper          10
#define L_dircommarcominf       10
#define L_dirteccomass          10
#define L_dirbvdate             30
#define L_dirflag               10


/* requete qui sert à générer les déclarations
 * select 'char    '||lower(column_name)||'[L_'||lower(column_name)||'+1]'
 *  from user_tab_columns
 *  where table_name='FIN_MARKET_MAJ'
 * order by column_id
 */
typedef struct {
  char	entnum[N_FinMarketArray][L_entnum+1];
  char	entnumtyp[N_FinMarketArray][L_entnumtyp+1];
  char	etanic[N_FinMarketArray][L_etanic+1];
  /* données entreprises */
  char	entbaseeur[N_FinMarketArray][L_entbaseeur+1];
  char	entcreannee[N_FinMarketArray][L_entcreannee+1];
  char	entcremois[N_FinMarketArray][L_entcremois+1];
  char	entcatjurcod[N_FinMarketArray][L_entcatjurcod+1];
  char	entcatjur2c[N_FinMarketArray][L_entcatjur2c+1];
  char	enttraeffcod[N_FinMarketArray][L_enttraeffcod+1];
  char	enteff[N_FinMarketArray][L_enteff+1];
  char	enttracap[N_FinMarketArray][L_enttracap+1];
  char	enttraca[N_FinMarketArray][L_enttraca+1];
  char	entrestrarapcab[N_FinMarketArray][L_entrestrarapcab+1];
  char	enttracae[N_FinMarketArray][L_enttracae+1];
  char	entnbreta[N_FinMarketArray][L_entnbreta+1];
  char	entnotort[N_FinMarketArray][L_entnotort+1];
  char	entcotalp[N_FinMarketArray][L_entcotalp+1];
  char	entdenoff[N_FinMarketArray][L_entdenoff+1];
  char	etastatutcod[N_FinMarketArray][L_etastatutcod+1];
  char	etaapecod[N_FinMarketArray][L_etaapecod+1];
  char	etaape3c[N_FinMarketArray][L_etaape3c+1];
  char	etaape2c[N_FinMarketArray][L_etaape2c+1];
  char	etatraeffcod[N_FinMarketArray][L_etatraeffcod+1];
  char	etaeff[N_FinMarketArray][L_etaeff+1];
  char	etapayscod[N_FinMarketArray][L_etapayscod+1];
  char	etaregion[N_FinMarketArray][L_etaregion+1];
  char	etadepart[N_FinMarketArray][L_etadepart+1];
  char	etaarrond[N_FinMarketArray][L_etaarrond+1];
  char	etacanton[N_FinMarketArray][L_etacanton+1];
  char	etacommune[N_FinMarketArray][L_etacommune+1];
  char	etacodevoie[N_FinMarketArray][L_etacodevoie+1];
  char	etapostal[N_FinMarketArray][L_etapostal+1];
  char	etatel[N_FinMarketArray][L_etatel+1];
  char	etafax[N_FinMarketArray][L_etafax+1];
  char	etaobjsoc1[N_FinMarketArray][L_etaobjsoc1+1];
  char	etaobjsoc2[N_FinMarketArray][L_etaobjsoc2+1];
  char	etaobjsoc3[N_FinMarketArray][L_etaobjsoc3+1];
  /* le dernier bilan */
  char	a0ratbilseq[N_FinMarketArray][L_ratbilseq+1];
  char	a0ratdatord[N_FinMarketArray][L_ratdatord+1];
  char	a0ratsup[N_FinMarketArray][L_ratsup+1];
  char	a0ratdatclo[N_FinMarketArray][L_ratdatclo+1];
  char	a0ratcloannee[N_FinMarketArray][L_ratcloannee+1];
  char	a0ratduree[N_FinMarketArray][L_ratduree+1];
  char	a0rattop[N_FinMarketArray][L_rattop+1];
  char	a0ratmax[N_FinMarketArray][L_ratmax+1];
  char	a0ratoribilcod[N_FinMarketArray][L_ratoribilcod+1];
  char	a0ratdevisecod[N_FinMarketArray][L_ratdevisecod+1];
  char	a0ratmotnonsaicod[N_FinMarketArray][L_ratmotnonsaicod+1];
  char	a0ratvalcod[N_FinMarketArray][L_ratvalcod+1];
  char	a0ratunimoncod[N_FinMarketArray][L_ratunimoncod+1];
  char	a0ratrapgestcod[N_FinMarketArray][L_ratrapgestcod+1];
  char	a0ratrapcomcod[N_FinMarketArray][L_ratrapcomcod+1];
  char	a0ratannexecod[N_FinMarketArray][L_ratannexecod+1];
  char	a0ratnafcod[N_FinMarketArray][L_ratnafcod+1];
  char	a0ratgreffecod[N_FinMarketArray][L_ratgreffecod+1];
  char	a0ratnumcod[N_FinMarketArray][L_ratnumcod+1];
  char	a0rattypbilcod[N_FinMarketArray][L_rattypbilcod+1];
  char	a0ratdatordbil[N_FinMarketArray][L_ratdatordbil+1];
  char	a0ratr1[N_FinMarketArray][L_ratr1+1];
  char	a0ratr11[N_FinMarketArray][L_ratr11+1];
  char	a0ratr13[N_FinMarketArray][L_ratr13+1];
  char	a0ratr14[N_FinMarketArray][L_ratr14+1];
  char	a0ratr16[N_FinMarketArray][L_ratr16+1];
  char	a0ratr18[N_FinMarketArray][L_ratr18+1];
  char	a0ratr19[N_FinMarketArray][L_ratr19+1];
  char	a0ratr20[N_FinMarketArray][L_ratr20+1];
  char	a0ratr21[N_FinMarketArray][L_ratr21+1];
  char	a0ratr22[N_FinMarketArray][L_ratr22+1];
  char	a0ratr28[N_FinMarketArray][L_ratr28+1];
  char	a0ratr29[N_FinMarketArray][L_ratr29+1];
  char	a0ratr30[N_FinMarketArray][L_ratr30+1];
  char	a0ratr31[N_FinMarketArray][L_ratr31+1];
  char	a0ratr32[N_FinMarketArray][L_ratr32+1];
  char	a0ratr33[N_FinMarketArray][L_ratr33+1];
  char	a0ratr35[N_FinMarketArray][L_ratr35+1];
  char	a0ratr36[N_FinMarketArray][L_ratr36+1];
  char	a0bilscbf[N_FinMarketArray][L_bilscbf+1];
  char	a0bilscbf3[N_FinMarketArray][L_bilscbf3+1];
  char	a0bilscbh[N_FinMarketArray][L_bilscbh+1];
  char	a0bilscbh3[N_FinMarketArray][L_bilscbh3+1];
  char	a0bilscbt[N_FinMarketArray][L_bilscbt+1];
  char	a0bilscbt3[N_FinMarketArray][L_bilscbt3+1];
  char	a0bilscbx[N_FinMarketArray][L_bilscbx+1];
  char	a0bilscbx3[N_FinMarketArray][L_bilscbx3+1];
  char	a0bilsccd[N_FinMarketArray][L_bilsccd+1];
  char	a0bilsccd3[N_FinMarketArray][L_bilsccd3+1];
  char	a0bilsccf[N_FinMarketArray][L_bilsccf+1];
  char	a0bilsccf3[N_FinMarketArray][L_bilsccf3+1];
  char	a0bilscco[N_FinMarketArray][L_bilscco+1];
  char	a0bilscco3[N_FinMarketArray][L_bilscco3+1];
  char	a0bilscda[N_FinMarketArray][L_bilscda+1];
  char	a0bilscdr[N_FinMarketArray][L_bilscdr+1];
  char	a0bilscdu[N_FinMarketArray][L_bilscdu+1];
  char	a0bilscdv[N_FinMarketArray][L_bilscdv+1];
  char	a0bilscdx[N_FinMarketArray][L_bilscdx+1];
  char	a0bilscdy[N_FinMarketArray][L_bilscdy+1];
  char	a0bilscfj[N_FinMarketArray][L_bilscfj+1];
  char	a0bilscfk[N_FinMarketArray][L_bilscfk+1];
  char	a0bilscfl[N_FinMarketArray][L_bilscfl+1];
  char	a0bilscfs[N_FinMarketArray][L_bilscfs+1];
  char	a0bilscft[N_FinMarketArray][L_bilscft+1];
  char	a0bilscfu[N_FinMarketArray][L_bilscfu+1];
  char	a0bilscfv[N_FinMarketArray][L_bilscfv+1];
  char	a0bilscgg[N_FinMarketArray][L_bilscgg+1];
  char	a0bilscgv[N_FinMarketArray][L_bilscgv+1];
  char	a0bilscgw[N_FinMarketArray][L_bilscgw+1];
  char	a0bilschj[N_FinMarketArray][L_bilschj+1];
  char	a0bilschn[N_FinMarketArray][L_bilschn+1];
  char	a0bilschp[N_FinMarketArray][L_bilschp+1];
  char	a0bilschq[N_FinMarketArray][L_bilschq+1];
  char	a0bilsc0r[N_FinMarketArray][L_bilsc0r+1];
  char	a0bilscua[N_FinMarketArray][L_bilscua+1];
  char	a0bilscva[N_FinMarketArray][L_bilscva+1];
  char	a0bilscvu[N_FinMarketArray][L_bilscvu+1];
  char	a0bilscvv[N_FinMarketArray][L_bilscvv+1];
  char	a0bilscvg[N_FinMarketArray][L_bilscvg+1];
  char	a0bilscvg2[N_FinMarketArray][L_bilscvg2+1];
  char	a0bilscvh[N_FinMarketArray][L_bilscvh+1];
  char	a0bilscvh2[N_FinMarketArray][L_bilscvh2+1];
  char	a0bilsc8a2[N_FinMarketArray][L_bilsc8a2+1];
  char	a0bilscvi[N_FinMarketArray][L_bilscvi+1];
  char	a0bilscyp[N_FinMarketArray][L_bilscyp+1];
  char	a0bilscyq[N_FinMarketArray][L_bilscyq+1];
  char	a0bilscyr[N_FinMarketArray][L_bilscyr+1];
  char	a0bilscys[N_FinMarketArray][L_bilscys+1];
  char	a0bilscyt[N_FinMarketArray][L_bilscyt+1];
  char	a0bilscyu[N_FinMarketArray][L_bilscyu+1];
  /* le bilan n-1 */
  char	a1ratbilseq[N_FinMarketArray][L_ratbilseq+1];
  char	a1ratdatord[N_FinMarketArray][L_ratdatord+1];
  char	a1ratsup[N_FinMarketArray][L_ratsup+1];
  char	a1ratdatclo[N_FinMarketArray][L_ratdatclo+1];
  char	a1ratcloannee[N_FinMarketArray][L_ratcloannee+1];
  char	a1ratduree[N_FinMarketArray][L_ratduree+1];
  char	a1rattop[N_FinMarketArray][L_rattop+1];
  char	a1ratmax[N_FinMarketArray][L_ratmax+1];
  char	a1ratoribilcod[N_FinMarketArray][L_ratoribilcod+1];
  char	a1ratdevisecod[N_FinMarketArray][L_ratdevisecod+1];
  char	a1ratmotnonsaicod[N_FinMarketArray][L_ratmotnonsaicod+1];
  char	a1ratvalcod[N_FinMarketArray][L_ratvalcod+1];
  char	a1ratunimoncod[N_FinMarketArray][L_ratunimoncod+1];
  char	a1ratrapgestcod[N_FinMarketArray][L_ratrapgestcod+1];
  char	a1ratrapcomcod[N_FinMarketArray][L_ratrapcomcod+1];
  char	a1ratannexecod[N_FinMarketArray][L_ratannexecod+1];
  char	a1ratnafcod[N_FinMarketArray][L_ratnafcod+1];
  char	a1ratgreffecod[N_FinMarketArray][L_ratgreffecod+1];
  char	a1ratnumcod[N_FinMarketArray][L_ratnumcod+1];
  char	a1rattypbilcod[N_FinMarketArray][L_rattypbilcod+1];
  char	a1ratdatordbil[N_FinMarketArray][L_ratdatordbil+1];
  char	a1ratr1[N_FinMarketArray][L_ratr1+1];
  char	a1ratr11[N_FinMarketArray][L_ratr11+1];
  char	a1ratr13[N_FinMarketArray][L_ratr13+1];
  char	a1ratr14[N_FinMarketArray][L_ratr14+1];
  char	a1ratr16[N_FinMarketArray][L_ratr16+1];
  char	a1ratr18[N_FinMarketArray][L_ratr18+1];
  char	a1ratr19[N_FinMarketArray][L_ratr19+1];
  char	a1ratr20[N_FinMarketArray][L_ratr20+1];
  char	a1ratr21[N_FinMarketArray][L_ratr21+1];
  char	a1ratr22[N_FinMarketArray][L_ratr22+1];
  char	a1ratr28[N_FinMarketArray][L_ratr28+1];
  char	a1ratr29[N_FinMarketArray][L_ratr29+1];
  char	a1ratr30[N_FinMarketArray][L_ratr30+1];
  char	a1ratr31[N_FinMarketArray][L_ratr31+1];
  char	a1ratr32[N_FinMarketArray][L_ratr32+1];
  char	a1ratr33[N_FinMarketArray][L_ratr33+1];
  char	a1ratr35[N_FinMarketArray][L_ratr35+1];
  char	a1ratr36[N_FinMarketArray][L_ratr36+1];
  char	a1bilscbf[N_FinMarketArray][L_bilscbf+1];
  char	a1bilscbf3[N_FinMarketArray][L_bilscbf3+1];
  char	a1bilscbh[N_FinMarketArray][L_bilscbh+1];
  char	a1bilscbh3[N_FinMarketArray][L_bilscbh3+1];
  char	a1bilscbt[N_FinMarketArray][L_bilscbt+1];
  char	a1bilscbt3[N_FinMarketArray][L_bilscbt3+1];
  char	a1bilscbx[N_FinMarketArray][L_bilscbx+1];
  char	a1bilscbx3[N_FinMarketArray][L_bilscbx3+1];
  char	a1bilsccd[N_FinMarketArray][L_bilsccd+1];
  char	a1bilsccd3[N_FinMarketArray][L_bilsccd3+1];
  char	a1bilsccf[N_FinMarketArray][L_bilsccf+1];
  char	a1bilsccf3[N_FinMarketArray][L_bilsccf3+1];
  char	a1bilscco[N_FinMarketArray][L_bilscco+1];
  char	a1bilscco3[N_FinMarketArray][L_bilscco3+1];
  char	a1bilscda[N_FinMarketArray][L_bilscda+1];
  char	a1bilscdr[N_FinMarketArray][L_bilscdr+1];
  char	a1bilscdu[N_FinMarketArray][L_bilscdu+1];
  char	a1bilscdv[N_FinMarketArray][L_bilscdv+1];
  char	a1bilscdx[N_FinMarketArray][L_bilscdx+1];
  char	a1bilscdy[N_FinMarketArray][L_bilscdy+1];
  char	a1bilscfj[N_FinMarketArray][L_bilscfj+1];
  char	a1bilscfk[N_FinMarketArray][L_bilscfk+1];
  char	a1bilscfl[N_FinMarketArray][L_bilscfl+1];
  char	a1bilscfs[N_FinMarketArray][L_bilscfs+1];
  char	a1bilscft[N_FinMarketArray][L_bilscft+1];
  char	a1bilscfu[N_FinMarketArray][L_bilscfu+1];
  char	a1bilscfv[N_FinMarketArray][L_bilscfv+1];
  char	a1bilscgg[N_FinMarketArray][L_bilscgg+1];
  char	a1bilscgv[N_FinMarketArray][L_bilscgv+1];
  char	a1bilscgw[N_FinMarketArray][L_bilscgw+1];
  char	a1bilschj[N_FinMarketArray][L_bilschj+1];
  char	a1bilschn[N_FinMarketArray][L_bilschn+1];
  char	a1bilschp[N_FinMarketArray][L_bilschp+1];
  char	a1bilschq[N_FinMarketArray][L_bilschq+1];
  char	a1bilsc0r[N_FinMarketArray][L_bilsc0r+1];
  char	a1bilscua[N_FinMarketArray][L_bilscua+1];
  char	a1bilscva[N_FinMarketArray][L_bilscva+1];
  char	a1bilscvu[N_FinMarketArray][L_bilscvu+1];
  char	a1bilscvv[N_FinMarketArray][L_bilscvv+1];
  char	a1bilscvg[N_FinMarketArray][L_bilscvg+1];
  char	a1bilscvg2[N_FinMarketArray][L_bilscvg2+1];
  char	a1bilscvh[N_FinMarketArray][L_bilscvh+1];
  char	a1bilscvh2[N_FinMarketArray][L_bilscvh2+1];
  char	a1bilsc8a2[N_FinMarketArray][L_bilsc8a2+1];
  char	a1bilscvi[N_FinMarketArray][L_bilscvi+1];
  char	a1bilscyp[N_FinMarketArray][L_bilscyp+1];
  char	a1bilscyq[N_FinMarketArray][L_bilscyq+1];
  char	a1bilscyr[N_FinMarketArray][L_bilscyr+1];
  char	a1bilscys[N_FinMarketArray][L_bilscys+1];
  char	a1bilscyt[N_FinMarketArray][L_bilscyt+1];
  char	a1bilscyu[N_FinMarketArray][L_bilscyu+1];
  /* le bilan n-2 */
  char	a2ratbilseq[N_FinMarketArray][L_ratbilseq+1];
  char	a2ratdatord[N_FinMarketArray][L_ratdatord+1];
  char	a2ratsup[N_FinMarketArray][L_ratsup+1];
  char	a2ratdatclo[N_FinMarketArray][L_ratdatclo+1];
  char	a2ratcloannee[N_FinMarketArray][L_ratcloannee+1];
  char	a2ratduree[N_FinMarketArray][L_ratduree+1];
  char	a2rattop[N_FinMarketArray][L_rattop+1];
  char	a2ratmax[N_FinMarketArray][L_ratmax+1];
  char	a2ratoribilcod[N_FinMarketArray][L_ratoribilcod+1];
  char	a2ratdevisecod[N_FinMarketArray][L_ratdevisecod+1];
  char	a2ratmotnonsaicod[N_FinMarketArray][L_ratmotnonsaicod+1];
  char	a2ratvalcod[N_FinMarketArray][L_ratvalcod+1];
  char	a2ratunimoncod[N_FinMarketArray][L_ratunimoncod+1];
  char	a2ratrapgestcod[N_FinMarketArray][L_ratrapgestcod+1];
  char	a2ratrapcomcod[N_FinMarketArray][L_ratrapcomcod+1];
  char	a2ratannexecod[N_FinMarketArray][L_ratannexecod+1];
  char	a2ratnafcod[N_FinMarketArray][L_ratnafcod+1];
  char	a2ratgreffecod[N_FinMarketArray][L_ratgreffecod+1];
  char	a2ratnumcod[N_FinMarketArray][L_ratnumcod+1];
  char	a2rattypbilcod[N_FinMarketArray][L_rattypbilcod+1];
  char	a2ratdatordbil[N_FinMarketArray][L_ratdatordbil+1];
  char	a2ratr1[N_FinMarketArray][L_ratr1+1];
  char	a2ratr11[N_FinMarketArray][L_ratr11+1];
  char	a2ratr13[N_FinMarketArray][L_ratr13+1];
  char	a2ratr14[N_FinMarketArray][L_ratr14+1];
  char	a2ratr16[N_FinMarketArray][L_ratr16+1];
  char	a2ratr18[N_FinMarketArray][L_ratr18+1];
  char	a2ratr19[N_FinMarketArray][L_ratr19+1];
  char	a2ratr20[N_FinMarketArray][L_ratr20+1];
  char	a2ratr21[N_FinMarketArray][L_ratr21+1];
  char	a2ratr22[N_FinMarketArray][L_ratr22+1];
  char	a2ratr28[N_FinMarketArray][L_ratr28+1];
  char	a2ratr29[N_FinMarketArray][L_ratr29+1];
  char	a2ratr30[N_FinMarketArray][L_ratr30+1];
  char	a2ratr31[N_FinMarketArray][L_ratr31+1];
  char	a2ratr32[N_FinMarketArray][L_ratr32+1];
  char	a2ratr33[N_FinMarketArray][L_ratr33+1];
  char	a2ratr35[N_FinMarketArray][L_ratr35+1];
  char	a2ratr36[N_FinMarketArray][L_ratr36+1];
  char	a2bilscbf[N_FinMarketArray][L_bilscbf+1];
  char	a2bilscbf3[N_FinMarketArray][L_bilscbf3+1];
  char	a2bilscbh[N_FinMarketArray][L_bilscbh+1];
  char	a2bilscbh3[N_FinMarketArray][L_bilscbh3+1];
  char	a2bilscbt[N_FinMarketArray][L_bilscbt+1];
  char	a2bilscbt3[N_FinMarketArray][L_bilscbt3+1];
  char	a2bilscbx[N_FinMarketArray][L_bilscbx+1];
  char	a2bilscbx3[N_FinMarketArray][L_bilscbx3+1];
  char	a2bilsccd[N_FinMarketArray][L_bilsccd+1];
  char	a2bilsccd3[N_FinMarketArray][L_bilsccd3+1];
  char	a2bilsccf[N_FinMarketArray][L_bilsccf+1];
  char	a2bilsccf3[N_FinMarketArray][L_bilsccf3+1];
  char	a2bilscco[N_FinMarketArray][L_bilscco+1];
  char	a2bilscco3[N_FinMarketArray][L_bilscco3+1];
  char	a2bilscda[N_FinMarketArray][L_bilscda+1];
  char	a2bilscdr[N_FinMarketArray][L_bilscdr+1];
  char	a2bilscdu[N_FinMarketArray][L_bilscdu+1];
  char	a2bilscdv[N_FinMarketArray][L_bilscdv+1];
  char	a2bilscdx[N_FinMarketArray][L_bilscdx+1];
  char	a2bilscdy[N_FinMarketArray][L_bilscdy+1];
  char	a2bilscfj[N_FinMarketArray][L_bilscfj+1];
  char	a2bilscfk[N_FinMarketArray][L_bilscfk+1];
  char	a2bilscfl[N_FinMarketArray][L_bilscfl+1];
  char	a2bilscfs[N_FinMarketArray][L_bilscfs+1];
  char	a2bilscft[N_FinMarketArray][L_bilscft+1];
  char	a2bilscfu[N_FinMarketArray][L_bilscfu+1];
  char	a2bilscfv[N_FinMarketArray][L_bilscfv+1];
  char	a2bilscgg[N_FinMarketArray][L_bilscgg+1];
  char	a2bilscgv[N_FinMarketArray][L_bilscgv+1];
  char	a2bilscgw[N_FinMarketArray][L_bilscgw+1];
  char	a2bilschj[N_FinMarketArray][L_bilschj+1];
  char	a2bilschn[N_FinMarketArray][L_bilschn+1];
  char	a2bilschp[N_FinMarketArray][L_bilschp+1];
  char	a2bilschq[N_FinMarketArray][L_bilschq+1];
  char	a2bilsc0r[N_FinMarketArray][L_bilsc0r+1];
  char	a2bilscua[N_FinMarketArray][L_bilscua+1];
  char	a2bilscva[N_FinMarketArray][L_bilscva+1];
  char	a2bilscvu[N_FinMarketArray][L_bilscvu+1];
  char	a2bilscvv[N_FinMarketArray][L_bilscvv+1];
  char	a2bilscvg[N_FinMarketArray][L_bilscvg+1];
  char	a2bilscvg2[N_FinMarketArray][L_bilscvg2+1];
  char	a2bilscvh[N_FinMarketArray][L_bilscvh+1];
  char	a2bilscvh2[N_FinMarketArray][L_bilscvh2+1];
  char	a2bilsc8a2[N_FinMarketArray][L_bilsc8a2+1];
  char	a2bilscvi[N_FinMarketArray][L_bilscvi+1];
  char	a2bilscyp[N_FinMarketArray][L_bilscyp+1];
  char	a2bilscyq[N_FinMarketArray][L_bilscyq+1];
  char	a2bilscyr[N_FinMarketArray][L_bilscyr+1];
  char	a2bilscys[N_FinMarketArray][L_bilscys+1];
  char	a2bilscyt[N_FinMarketArray][L_bilscyt+1];
  char	a2bilscyu[N_FinMarketArray][L_bilscyu+1];
  /* les fonctions operationelles */
  char	dirfamille[N_FinMarketArray][L_dirfamille+1];
  char	dirdir[N_FinMarketArray][L_dirdir+1];
  char	dirfinadmper[N_FinMarketArray][L_dirfinadmper+1];
  char	dircommarcominf[N_FinMarketArray][L_dircommarcominf+1];
  char	dirteccomass[N_FinMarketArray][L_dirteccomass+1];
  char	dirbvdate[N_FinMarketArray][L_dirbvdate+1];
  char	dirflag[N_FinMarketArray][L_dirflag+1];
} FinMarketArrayData;

typedef struct {
  short	entnum[N_FinMarketArray];
  short	entnumtyp[N_FinMarketArray];
  short	etanic[N_FinMarketArray];
  /* données entreprises */
  short	entbaseeur[N_FinMarketArray];
  short	entcreannee[N_FinMarketArray];
  short	entcremois[N_FinMarketArray];
  short	entcatjurcod[N_FinMarketArray];
  short	entcatjur2c[N_FinMarketArray];
  short	enttraeffcod[N_FinMarketArray];
  short	enteff[N_FinMarketArray];
  short	enttracap[N_FinMarketArray];
  short	enttraca[N_FinMarketArray];
  short	entrestrarapcab[N_FinMarketArray];
  short	enttracae[N_FinMarketArray];
  short	entnbreta[N_FinMarketArray];
  short	entnotort[N_FinMarketArray];
  short	entcotalp[N_FinMarketArray];
  short	entdenoff[N_FinMarketArray];
  short	etastatutcod[N_FinMarketArray];
  short	etaapecod[N_FinMarketArray];
  short	etaape3c[N_FinMarketArray];
  short	etaape2c[N_FinMarketArray];
  short	etatraeffcod[N_FinMarketArray];
  short	etaeff[N_FinMarketArray];
  short	etapayscod[N_FinMarketArray];
  short	etaregion[N_FinMarketArray];
  short	etadepart[N_FinMarketArray];
  short	etaarrond[N_FinMarketArray];
  short	etacanton[N_FinMarketArray];
  short	etacommune[N_FinMarketArray];
  short	etacodevoie[N_FinMarketArray];
  short	etapostal[N_FinMarketArray];
  short	etatel[N_FinMarketArray];
  short	etafax[N_FinMarketArray];
  short	etaobjsoc1[N_FinMarketArray];
  short	etaobjsoc2[N_FinMarketArray];
  short	etaobjsoc3[N_FinMarketArray];
  /* le dernier bilan */
  short	a0ratbilseq[N_FinMarketArray];
  short	a0ratdatord[N_FinMarketArray];
  short	a0ratsup[N_FinMarketArray];
  short	a0ratdatclo[N_FinMarketArray];
  short	a0ratcloannee[N_FinMarketArray];
  short	a0ratduree[N_FinMarketArray];
  short	a0rattop[N_FinMarketArray];
  short	a0ratmax[N_FinMarketArray];
  short	a0ratoribilcod[N_FinMarketArray];
  short	a0ratdevisecod[N_FinMarketArray];
  short	a0ratmotnonsaicod[N_FinMarketArray];
  short	a0ratvalcod[N_FinMarketArray];
  short	a0ratunimoncod[N_FinMarketArray];
  short	a0ratrapgestcod[N_FinMarketArray];
  short	a0ratrapcomcod[N_FinMarketArray];
  short	a0ratannexecod[N_FinMarketArray];
  short	a0ratnafcod[N_FinMarketArray];
  short	a0ratgreffecod[N_FinMarketArray];
  short	a0ratnumcod[N_FinMarketArray];
  short	a0rattypbilcod[N_FinMarketArray];
  short	a0ratdatordbil[N_FinMarketArray];
  short	a0ratr1[N_FinMarketArray];
  short	a0ratr11[N_FinMarketArray];
  short	a0ratr13[N_FinMarketArray];
  short	a0ratr14[N_FinMarketArray];
  short	a0ratr16[N_FinMarketArray];
  short	a0ratr18[N_FinMarketArray];
  short	a0ratr19[N_FinMarketArray];
  short	a0ratr20[N_FinMarketArray];
  short	a0ratr21[N_FinMarketArray];
  short	a0ratr22[N_FinMarketArray];
  short	a0ratr28[N_FinMarketArray];
  short	a0ratr29[N_FinMarketArray];
  short	a0ratr30[N_FinMarketArray];
  short	a0ratr31[N_FinMarketArray];
  short	a0ratr32[N_FinMarketArray];
  short	a0ratr33[N_FinMarketArray];
  short	a0ratr35[N_FinMarketArray];
  short	a0ratr36[N_FinMarketArray];
  short	a0bilscbf[N_FinMarketArray];
  short	a0bilscbf3[N_FinMarketArray];
  short	a0bilscbh[N_FinMarketArray];
  short	a0bilscbh3[N_FinMarketArray];
  short	a0bilscbt[N_FinMarketArray];
  short	a0bilscbt3[N_FinMarketArray];
  short	a0bilscbx[N_FinMarketArray];
  short	a0bilscbx3[N_FinMarketArray];
  short	a0bilsccd[N_FinMarketArray];
  short	a0bilsccd3[N_FinMarketArray];
  short	a0bilsccf[N_FinMarketArray];
  short	a0bilsccf3[N_FinMarketArray];
  short	a0bilscco[N_FinMarketArray];
  short	a0bilscco3[N_FinMarketArray];
  short	a0bilscda[N_FinMarketArray];
  short	a0bilscdr[N_FinMarketArray];
  short	a0bilscdu[N_FinMarketArray];
  short	a0bilscdv[N_FinMarketArray];
  short	a0bilscdx[N_FinMarketArray];
  short	a0bilscdy[N_FinMarketArray];
  short	a0bilscfj[N_FinMarketArray];
  short	a0bilscfk[N_FinMarketArray];
  short	a0bilscfl[N_FinMarketArray];
  short	a0bilscfs[N_FinMarketArray];
  short	a0bilscft[N_FinMarketArray];
  short	a0bilscfu[N_FinMarketArray];
  short	a0bilscfv[N_FinMarketArray];
  short	a0bilscgg[N_FinMarketArray];
  short	a0bilscgv[N_FinMarketArray];
  short	a0bilscgw[N_FinMarketArray];
  short	a0bilschj[N_FinMarketArray];
  short	a0bilschn[N_FinMarketArray];
  short	a0bilschp[N_FinMarketArray];
  short	a0bilschq[N_FinMarketArray];
  short	a0bilsc0r[N_FinMarketArray];
  short	a0bilscua[N_FinMarketArray];
  short	a0bilscva[N_FinMarketArray];
  short	a0bilscvu[N_FinMarketArray];
  short	a0bilscvv[N_FinMarketArray];
  short	a0bilscvg[N_FinMarketArray];
  short	a0bilscvg2[N_FinMarketArray];
  short	a0bilscvh[N_FinMarketArray];
  short	a0bilscvh2[N_FinMarketArray];
  short	a0bilsc8a2[N_FinMarketArray];
  short	a0bilscvi[N_FinMarketArray];
  short	a0bilscyp[N_FinMarketArray];
  short	a0bilscyq[N_FinMarketArray];
  short	a0bilscyr[N_FinMarketArray];
  short	a0bilscys[N_FinMarketArray];
  short	a0bilscyt[N_FinMarketArray];
  short	a0bilscyu[N_FinMarketArray];
  /* le bilan n-1 */
  short	a1ratbilseq[N_FinMarketArray];
  short	a1ratdatord[N_FinMarketArray];
  short	a1ratsup[N_FinMarketArray];
  short	a1ratdatclo[N_FinMarketArray];
  short	a1ratcloannee[N_FinMarketArray];
  short	a1ratduree[N_FinMarketArray];
  short	a1rattop[N_FinMarketArray];
  short	a1ratmax[N_FinMarketArray];
  short	a1ratoribilcod[N_FinMarketArray];
  short	a1ratdevisecod[N_FinMarketArray];
  short	a1ratmotnonsaicod[N_FinMarketArray];
  short	a1ratvalcod[N_FinMarketArray];
  short	a1ratunimoncod[N_FinMarketArray];
  short	a1ratrapgestcod[N_FinMarketArray];
  short	a1ratrapcomcod[N_FinMarketArray];
  short	a1ratannexecod[N_FinMarketArray];
  short	a1ratnafcod[N_FinMarketArray];
  short	a1ratgreffecod[N_FinMarketArray];
  short	a1ratnumcod[N_FinMarketArray];
  short	a1rattypbilcod[N_FinMarketArray];
  short	a1ratdatordbil[N_FinMarketArray];
  short	a1ratr1[N_FinMarketArray];
  short	a1ratr11[N_FinMarketArray];
  short	a1ratr13[N_FinMarketArray];
  short	a1ratr14[N_FinMarketArray];
  short	a1ratr16[N_FinMarketArray];
  short	a1ratr18[N_FinMarketArray];
  short	a1ratr19[N_FinMarketArray];
  short	a1ratr20[N_FinMarketArray];
  short	a1ratr21[N_FinMarketArray];
  short	a1ratr22[N_FinMarketArray];
  short	a1ratr28[N_FinMarketArray];
  short	a1ratr29[N_FinMarketArray];
  short	a1ratr30[N_FinMarketArray];
  short	a1ratr31[N_FinMarketArray];
  short	a1ratr32[N_FinMarketArray];
  short	a1ratr33[N_FinMarketArray];
  short	a1ratr35[N_FinMarketArray];
  short	a1ratr36[N_FinMarketArray];
  short	a1bilscbf[N_FinMarketArray];
  short	a1bilscbf3[N_FinMarketArray];
  short	a1bilscbh[N_FinMarketArray];
  short	a1bilscbh3[N_FinMarketArray];
  short	a1bilscbt[N_FinMarketArray];
  short	a1bilscbt3[N_FinMarketArray];
  short	a1bilscbx[N_FinMarketArray];
  short	a1bilscbx3[N_FinMarketArray];
  short	a1bilsccd[N_FinMarketArray];
  short	a1bilsccd3[N_FinMarketArray];
  short	a1bilsccf[N_FinMarketArray];
  short	a1bilsccf3[N_FinMarketArray];
  short	a1bilscco[N_FinMarketArray];
  short	a1bilscco3[N_FinMarketArray];
  short	a1bilscda[N_FinMarketArray];
  short	a1bilscdr[N_FinMarketArray];
  short	a1bilscdu[N_FinMarketArray];
  short	a1bilscdv[N_FinMarketArray];
  short	a1bilscdx[N_FinMarketArray];
  short	a1bilscdy[N_FinMarketArray];
  short	a1bilscfj[N_FinMarketArray];
  short	a1bilscfk[N_FinMarketArray];
  short	a1bilscfl[N_FinMarketArray];
  short	a1bilscfs[N_FinMarketArray];
  short	a1bilscft[N_FinMarketArray];
  short	a1bilscfu[N_FinMarketArray];
  short	a1bilscfv[N_FinMarketArray];
  short	a1bilscgg[N_FinMarketArray];
  short	a1bilscgv[N_FinMarketArray];
  short	a1bilscgw[N_FinMarketArray];
  short	a1bilschj[N_FinMarketArray];
  short	a1bilschn[N_FinMarketArray];
  short	a1bilschp[N_FinMarketArray];
  short	a1bilschq[N_FinMarketArray];
  short	a1bilsc0r[N_FinMarketArray];
  short	a1bilscua[N_FinMarketArray];
  short	a1bilscva[N_FinMarketArray];
  short	a1bilscvu[N_FinMarketArray];
  short	a1bilscvv[N_FinMarketArray];
  short	a1bilscvg[N_FinMarketArray];
  short	a1bilscvg2[N_FinMarketArray];
  short	a1bilscvh[N_FinMarketArray];
  short	a1bilscvh2[N_FinMarketArray];
  short	a1bilsc8a2[N_FinMarketArray];
  short	a1bilscvi[N_FinMarketArray];
  short	a1bilscyp[N_FinMarketArray];
  short	a1bilscyq[N_FinMarketArray];
  short	a1bilscyr[N_FinMarketArray];
  short	a1bilscys[N_FinMarketArray];
  short	a1bilscyt[N_FinMarketArray];
  short	a1bilscyu[N_FinMarketArray];
  /* le bilan n-2 */
  short	a2ratbilseq[N_FinMarketArray];
  short	a2ratdatord[N_FinMarketArray];
  short	a2ratsup[N_FinMarketArray];
  short	a2ratdatclo[N_FinMarketArray];
  short	a2ratcloannee[N_FinMarketArray];
  short	a2ratduree[N_FinMarketArray];
  short	a2rattop[N_FinMarketArray];
  short	a2ratmax[N_FinMarketArray];
  short	a2ratoribilcod[N_FinMarketArray];
  short	a2ratdevisecod[N_FinMarketArray];
  short	a2ratmotnonsaicod[N_FinMarketArray];
  short	a2ratvalcod[N_FinMarketArray];
  short	a2ratunimoncod[N_FinMarketArray];
  short	a2ratrapgestcod[N_FinMarketArray];
  short	a2ratrapcomcod[N_FinMarketArray];
  short	a2ratannexecod[N_FinMarketArray];
  short	a2ratnafcod[N_FinMarketArray];
  short	a2ratgreffecod[N_FinMarketArray];
  short	a2ratnumcod[N_FinMarketArray];
  short	a2rattypbilcod[N_FinMarketArray];
  short	a2ratdatordbil[N_FinMarketArray];
  short	a2ratr1[N_FinMarketArray];
  short	a2ratr11[N_FinMarketArray];
  short	a2ratr13[N_FinMarketArray];
  short	a2ratr14[N_FinMarketArray];
  short	a2ratr16[N_FinMarketArray];
  short	a2ratr18[N_FinMarketArray];
  short	a2ratr19[N_FinMarketArray];
  short	a2ratr20[N_FinMarketArray];
  short	a2ratr21[N_FinMarketArray];
  short	a2ratr22[N_FinMarketArray];
  short	a2ratr28[N_FinMarketArray];
  short	a2ratr29[N_FinMarketArray];
  short	a2ratr30[N_FinMarketArray];
  short	a2ratr31[N_FinMarketArray];
  short	a2ratr32[N_FinMarketArray];
  short	a2ratr33[N_FinMarketArray];
  short	a2ratr35[N_FinMarketArray];
  short	a2ratr36[N_FinMarketArray];
  short	a2bilscbf[N_FinMarketArray];
  short	a2bilscbf3[N_FinMarketArray];
  short	a2bilscbh[N_FinMarketArray];
  short	a2bilscbh3[N_FinMarketArray];
  short	a2bilscbt[N_FinMarketArray];
  short	a2bilscbt3[N_FinMarketArray];
  short	a2bilscbx[N_FinMarketArray];
  short	a2bilscbx3[N_FinMarketArray];
  short	a2bilsccd[N_FinMarketArray];
  short	a2bilsccd3[N_FinMarketArray];
  short	a2bilsccf[N_FinMarketArray];
  short	a2bilsccf3[N_FinMarketArray];
  short	a2bilscco[N_FinMarketArray];
  short	a2bilscco3[N_FinMarketArray];
  short	a2bilscda[N_FinMarketArray];
  short	a2bilscdr[N_FinMarketArray];
  short	a2bilscdu[N_FinMarketArray];
  short	a2bilscdv[N_FinMarketArray];
  short	a2bilscdx[N_FinMarketArray];
  short	a2bilscdy[N_FinMarketArray];
  short	a2bilscfj[N_FinMarketArray];
  short	a2bilscfk[N_FinMarketArray];
  short	a2bilscfl[N_FinMarketArray];
  short	a2bilscfs[N_FinMarketArray];
  short	a2bilscft[N_FinMarketArray];
  short	a2bilscfu[N_FinMarketArray];
  short	a2bilscfv[N_FinMarketArray];
  short	a2bilscgg[N_FinMarketArray];
  short	a2bilscgv[N_FinMarketArray];
  short	a2bilscgw[N_FinMarketArray];
  short	a2bilschj[N_FinMarketArray];
  short	a2bilschn[N_FinMarketArray];
  short	a2bilschp[N_FinMarketArray];
  short	a2bilschq[N_FinMarketArray];
  short	a2bilsc0r[N_FinMarketArray];
  short	a2bilscua[N_FinMarketArray];
  short	a2bilscva[N_FinMarketArray];
  short	a2bilscvu[N_FinMarketArray];
  short	a2bilscvv[N_FinMarketArray];
  short	a2bilscvg[N_FinMarketArray];
  short	a2bilscvg2[N_FinMarketArray];
  short	a2bilscvh[N_FinMarketArray];
  short	a2bilscvh2[N_FinMarketArray];
  short	a2bilsc8a2[N_FinMarketArray];
  short	a2bilscvi[N_FinMarketArray];
  short	a2bilscyp[N_FinMarketArray];
  short	a2bilscyq[N_FinMarketArray];
  short	a2bilscyr[N_FinMarketArray];
  short	a2bilscys[N_FinMarketArray];
  short	a2bilscyt[N_FinMarketArray];
  short	a2bilscyu[N_FinMarketArray];
  /* les fonctions operationelles */
  short	dirfamille[N_FinMarketArray];
  short	dirdir[N_FinMarketArray];
  short	dirfinadmper[N_FinMarketArray];
  short	dircommarcominf[N_FinMarketArray];
  short	dirteccomass[N_FinMarketArray];
  short	dirbvdate[N_FinMarketArray];
  short	dirflag[N_FinMarketArray];
} FinMarketArrayInd;

typedef struct {
  FinMarketArrayData data;
  FinMarketArrayInd  ind;
  int           nbData;
  int           nbDelete;
  int           nbInsert;
  int           nbTruncate;
} FinMarketArray;


FinMarketArray*	newFinMarketArray(void);
void		setFinMarketArray(FinMarketArray* finMarketArray,
				  Entreprise*     entrep,
				  DirigBV*        dirigbv,
				  Bilan*          bilans[]);
int		deleteFinMarketArray(FinMarketArray* finMarketArray,
				     char* laBase,
				     FinBilseq* finBilseq);
int		truncateFinMarketArray(FinMarketArray* finMarketArray,
				       char*      laBase,
				       FinBilseq* finBilseq);
int		flushFinMarketArray(FinMarketArray*	finMarketArray,
				    char*		laBase);
int		putFinMarketArray(FinMarketArray*	finMarketArray,
				  char*			laBase,
				  Entreprise*		entrep,
				  DirigBV*		dirigbv,
				  Bilan*		bilan[]);
int		updateFinMarketDirigBV(DirigBV*	dirigbv,
				       char*	laBase);
int		nbInsertFinMarketArray(FinMarketArray* finMarketArray);
int		nbDeleteFinMarketArray(FinMarketArray* finMarketArray);
int		afficheFinMarketArray(FinMarketArray*	finMarketArray);

#endif
