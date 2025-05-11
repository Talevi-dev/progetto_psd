typedef struct act *attivita;

#define MAX_NOME 50+1
#define MAX_DESC 300+1 

attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t cmp, time_t stm, time_t scd);

attivita input_attivita();

char *ottieni_nome(attivita a);

char *ottieni_corso(attivita a);

char *ottieni_descrizione(attivita a);

int ottieni_priorita(attivita a);

int ottieni_status(attivita a);

time_t ottieni_tempo_completato(attivita a);

time_t ottieni_tempo_stimato(attivita a);

time_t ottieni_scadenza(attivita a);

void aggiorna_nome(attivita a, char *nuovo);

void aggiorna_corso(attivita a, char *nuovo);

void aggiorna_descrizione(attivita a, char *nuovo);

void aggiorna_priorita(attivita a, int nuovo);

void aggiorna_status(attivita a);

void aggiorna_tempo_completato(attivita a, int o);

void aggiorna_tempo_stimato(attivita a, int o);

void aggiorna_scadenza(attivita a, time_t nuovo);

int sono_uguali(attivita a, attivita b);