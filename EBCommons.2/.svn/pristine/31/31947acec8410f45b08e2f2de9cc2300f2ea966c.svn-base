#ifndef IAChar_struct_h
#define IAChar_struct_h

// Array of Immutable Strings => AIStrings
// Cette structure de donnée ne peux être renseignée qu'en append
// c'est à dire en ajoutant un élément
// Aucune mise à jour n'est possible sur les données déjà présentes


//////////////////////////////////////////////////////////////
// Structure pour une collection dynamique mais immuable
// d'enregistrementsde char*
typedef struct _AIStrings {
  int     nbAlloc   ; // nb alloué pour les collections de taille variable ci dessous
  int     nbUsed    ; // nb utilisé dans les collections de taille variable ci dessous
  int     icur      ; // position courante
  int*    offsets   ; // tableau d'offsets de taille nbAlloc
  size_t  volAlloc  ; // volume en octets alloué du buffer s
  size_t  volUsed   ; // volume en octets utilisé du buffer s
  char*   s         ; // le buffer contigu qui contient les morceaux de char*
} AIStrings;
//
//////////////////////////////////////////////////////////////


#endif
