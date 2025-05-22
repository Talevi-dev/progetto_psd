
// Puntatore a struct atc, una struttura dati che rappresenta un'attività
typedef struct act *attivita;

#define MAX_NOME 50+1
#define MAX_DESC 300+1 


//Crea una nuova attività a partire dai parametri forniti in input
//Restituisce un puntatore alla nuova struttura allocata dinamicamente
attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t cmp, time_t stm, time_t scd);

//Prende da tastiera i dati necessari per la creazione di un attività, 
//controllandone la validità. Restituisce un puntatore all'attività creata.
attivita input_attivita();

//Restituisce una stringa contenente il nome dell'attività data in input
char *ottieni_nome(attivita a);

//Restituisce una stringa contenente il nome del corso dell'attività data in input
char *ottieni_corso(attivita a);

//Restituisce una stringa contenente la descrizione dell'attività data in input
char *ottieni_descrizione(attivita a);

//Restituisce la priorità dell'attività data in input
int ottieni_priorita(attivita a);

// Restituisce lo status dell'attività data in input
int ottieni_status(attivita a);

//Restituisce il tempo completato dell'attività data in input
time_t ottieni_tempo_completato(attivita a);

// Restituisce il tempo stimato dell'attività data in input
time_t ottieni_tempo_stimato(attivita a);

// Restituisce la scadenza dell'attività data in input
time_t ottieni_scadenza(attivita a);

// Modifica il nome del corso dell' attività in input sostituendolo con la stringa in input
void aggiorna_corso(attivita a, char *nuovo);

// Modifica la descrizione dell' attività in input sostituendola con la stringa in input
void aggiorna_descrizione(attivita a, char *nuovo);

// Modifica la priorità dell' attività in input sostituendola con l'intero in input
void aggiorna_priorita(attivita a, int nuovo);

//Aggiorna automaticamente lo status di un'attività data in input 
//in base alla scadenza, al tempo stimato e al tempo completato
void aggiorna_status(attivita a);

//Modifica il tempo completato dell' attivita in input sostituendola 
//con l'intero in input moltiplicato per 3600 (converisione da ore in secondi)
void aggiorna_tempo_completato(attivita a, int o);

//Modifica il tempo stimato dell' attivita in input sostituendola 
//con l'intero in input moltiplicato per 3600 (converisione da ore in secondi)
void aggiorna_tempo_stimato(attivita a, int o);

// Modifica la scadenza dell' attività in input sostituendola con il valore time_t in input
void aggiorna_scadenza(attivita a, time_t nuovo);