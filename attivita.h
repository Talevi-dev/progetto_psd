typedef struct act *attivita;

#define MAX_NOME 50+1
#define MAX_DESC 300+1 

attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t drt, time_t scd);

attivita input_attivita();

char *ottieni_nome(attivita a);

char *ottieni_corso(attivita a);

char *ottieni_descrizione(attivita a);

int ottieni_priorita(attivita a);

int ottieni_status(attivita a);

time_t ottieni_durata(attivita a);

time_t ottieni_scadenza(attivita a);

void aggiorna_nome(attivita a);

void aggiorna_corso(attivita a);

void aggiorna_descrizione(attivita a);