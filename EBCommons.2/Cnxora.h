#ifndef Cnxora_H
#define Cnxora_H


// Valeurs de l'indicateur de connexion
#define NOT_CONNECTED             0
#define IS_CONNECTED              1

// Valeurs des flags de fin de connexion
#define ORA_SESSION_KEEP_OPEN     0
#define ORA_SESSION_CLOSE         1

// Valeurs des flags de fin de transaction
#define ORA_TRANSAC_COMMIT        0
#define ORA_TRANSAC_ROLLBACK      1
#define ORA_TRANSAC_NOTHING       2

/* pour les fetchs */
#define NO_MORE_DATA      -1
#define GET_MORE_DATA     0

#define Default_stats_percent             20


#define L_nomBase                         200

// Longueur du buffer de récupération des log PL/SQL
#define L_dbms_output_buffer  1000000

typedef struct {
  char  nomBase[L_nomBase+1]; /* nom de la base */
  int   connecte;             /* connexion à la base oui/non */
} Cnxora;


Cnxora*  Cnxora_new       (char* nomBase);
int   Cnxora_set          (Cnxora* cox, char* nomBase, int connecte);
int   Cnxora_connect      (Cnxora* cox);
char* Cnxora_libelle_erreur_nconnexion(int ret);
int   Cnxora_estConnecte  (Cnxora* cox);
char* Cnxora_getNomBase   (Cnxora* cox);
int   Cnxora_commit       (Cnxora* cox);
int   Cnxora_rollback     (Cnxora* cox);
int   Cnxora_commitQ      (Cnxora* cox);
int   Cnxora_rollbackQ    (Cnxora* cox);
int	  Cnxora_tkprof       (Cnxora* cox);
int   Cnxora_getNlsParameters (Cnxora* cox, char* date_format, char* numeric_characters);
int   Cnxora_setNlsParameters (Cnxora* cox, char* date_format, char* numeric_characters);
int   Cnxora_getFormatDate    (Cnxora* cox, char* formatDateOracle);
int   Cnxora_setFormatDate    (Cnxora* cox, char* formatDateOracle);
int   Cnxora_setFormatDateContextObject (
              Cnxora* cox,
              char *new_format,
              char *new_lang);
int   Cnxora_setNumericCharacters (Cnxora*  cox);

int   Cnxora_dbms_output_enable (Cnxora*  cox);
int   Cnxora_get_dbms_output (Cnxora*  cox, char* dbms_output_buffer, int* n_lines);
int   Cnxora_dbms_output_disable (Cnxora*  cox);


// calcul des statistiques
int   Cnxora_gather_table_stats (Cnxora* cox, char* userora, char* tabora);
int   Cnxora_gather_index_stats (Cnxora* cox, char* userora, char* indora);

#endif
