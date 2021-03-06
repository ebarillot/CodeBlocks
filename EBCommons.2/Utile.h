#ifndef util_H
#define util_H

#define LOG   "LOG"
#define ERR   "ERR"
#define BAD   "BAD"
#define WARN  "WARN"

void        Utile_masquerSignaux (void);
void        Utile_demasquerSignaux (void);

int         Utile_flushEcritures (void);
int         Utile_flushAll (void);
void        Utile_rtrim (char* str);
void        Utile_lpad (char* str, int c, int len);
void        Utile_supspace (char* str);

char*       Utile_newString(long lg);
char*       Utile_reallocString(char* str, long lg);
// Isole un champ csv dans une chaine, � partir de la position *ptr1
// Renvoie le valeur dans tokenOut
int String_nextToken (  char*   tokenOut    // valeur du champ isol�, en sortie de fonction
                      , int     tokenOutMax // longueur max du receptacle tokenOut
                      , short*  ind         // indicateur de presence de valeur
                      , int*    no_champ    // num�ro du champ, incr�ment� � chaque appel
                      , char**  ptr1        // pointeur au d�but d'un champ � isoler dans une chaine
                                            // modifi� en sortie, pointe sur le d�but du champ suivant
                      , char*   csvsep);    // s�parateur csv
// Convertit un token en type long
int Token_toLong (long* field_N, char* tokenIn, short* ind);
// Convertit un token en type long long
int Token_toLongLong (long long* field_N, char* tokenIn, short* ind);
// Convertit un token en type char* (pour la beaut� du geste :)
int Token_toChar (char* fieldOut, int fieldOutMax, char* tokenIn, short* ind);
// test de la errno produite par les fonctions de conversion entre chaines et num�riques
int Utile_errno_test(void);
// Remise � 0 de errno
// A utiliser avant d'appeler une fonction de conversion strtol() par exemple
int Utile_errno_reset(void);


char*       Utile_noEOL (char* str);
char*       Utile_convPct1 (char* str);
char*       Utile_convCommaToPoint(char* in);
char*       Utile_dropComma(char* in);
int         Utile_isComment(char* in, char* tagComment);

char*       Utile_getDateHeure (void);
char*       Utile_truncDateHeureToYYYYMMDD (char* dateHeure);
char*       Utile_getDateYYYYMMDD (void);
char*       Utile_getAnneeMoisJour (void);
char*       Utile_getHeure (void);
char*       Utile_getDateSeconde          (void);
int         Utile_getMonthsFromNow (char* dateYYYYMMDD);
int         Utile_getMonthsBetween (char* dateYYYYMMDD_1, char* dateYYYYMMDD_2);
int         Utile_getDaysBetween          (char* dateYYYYMMDD_1, char* dateYYYYMMDD_2);
int         Utile_cmpDates                (char* dateYYYYMMDD_1, char* dateYYYYMMDD_2);
int         Utile_hasDateFormat           (char* date, char* dateFormat);
char*       Utile_convDate14To8 (char* str);
char*       Utile_convDateSlashToYYYYMMDD (char* str);
char*       Utile_convDateYYYYMMDDToDDsMMsYYYY (char* str);
char*       Utile_convDateYYYYsMMsDDToDDsMMsYYYY (char* str);
char*       Utile_convDateDDsMMsYYYYToYYYYsMMsDD (char* str);
char*       Utile_convDateYYYYToYYYYMMDD (char* str);
char*       Utile_convDateYYYYMMDDToSlash (char* str);
char*       Utile_convDateDDsMMsYYToYYYYsMMsDD (char* str);

int         Utile_getIntMAX (void);
int         Utile_getIntMIN (void);
long        Utile_getLongMAX (void);
long        Utile_getLongMIN (void);
long long   Utile_getLongLongMAX (void);
long long   Utile_getLongLongMIN (void);

unsigned int         Utile_getUIntMAX (void);
unsigned int         Utile_getUIntMIN (void);
unsigned long        Utile_getULongMAX (void);
unsigned long        Utile_getULongMIN (void);
unsigned long long   Utile_getULongLongMAX (void);
unsigned long long   Utile_getULongLongMIN (void);


// D�doublonnage d'une collection (fonction generique)
//  void*   coll      collection: vecteur de structures le plus souvent
//  size_t  szElem    taille d'un elemt
//  size_t  nbIn      nb elemt en entr�e (evec doublons)
//  size_t* nbOut     nb elemt en sortie (uniques)
//  size_t* isDoub    vecteur d'indicateurs: � 1 si l'�l�ment de m�me indice de la collection est un doublon
//  FunCmp* cmpsort   la fonction de comparaison de 2 elemt de la collection pour le tri
//  FunCmp* similarity  la fonction de similarit� de 2 elemt de la collection pour la recherche de doublons
//  La fonction de tri et celle de recherche de similarit� peuvent �tre diff�rentes.
//  m�me si elles sont tr�s proches.
//
typedef int (FunCmp)(const void*, const void*);

int Utile_dedoub(
    void*   coll
  , size_t  szElem
  , size_t  nbIn
  , size_t* nbOut
  , size_t* isDoub
  , FunCmp* cmpsort
  , FunCmp* similarity
);

#endif
