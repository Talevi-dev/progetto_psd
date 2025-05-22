#include "attivita.h"

// Puntatore a struct hash, una struttura dati che rappresenta una tabella hash
typedef struct hash* tabella_hash;

#define MAX_HASH 100

// Crea una nuova tabella hash della dimensione data in input
tabella_hash nuova_hash(int dimensione);

// Aggiunge un elemento nella tabella hash, gestendo eventuali collisioni tramite il concatenamento
void inserisci_hash(tabella_hash ht, attivita elem);

//Rimuove dalla tabella hash l'attivià data in input 
void cancella_hash(tabella_hash ht, attivita da_eliminare);

//Percorre la tabella hash nella sua interezza e stampa ogni attivita 
//con priorita uguale a quella data in input. Non applica filtri se la priorità è 0
void stampa_hash(tabella_hash ht, int priorita);

// Liberare tutta la memoria allocata dinamicamente per la tabella hash e le sue attivita
void distruggi_hash(tabella_hash ht);

// Salva il contenuto della tabella hash su di un file dato in input
void salva_hash(tabella_hash ht, const char *nome_file);
 
//Ricostruisce le attivita salvate in un file dato in input, 
//ne aggiorna lo status e le inserisce nella tabella hash data in input
void carica_hash(tabella_hash ht, const char *nome_file);

//Genera un report delle settimanta corrente indicando 
//le attivita in corso, completate e in ritardo e lo salva sul file dato in input
void report_settimanale_hash(tabella_hash ht, const char *nome_file);

//Scorre l'intera tabella hash e rimuove o tutte le attivita completate, 
//o tutte le attività in ritardo o entrambe le categorie
void pulizia_hash(tabella_hash ht, int mod);

//Cerca tutte le attività nella tabella hash che hanno nome uguale alla stringa passta in input, 
//e le inserisce nell'array di attività dato in input. Restituisce il numero di attivita trovate
int cerca_hash(tabella_hash ht, char* nome, attivita** risultati);

//Calcola una funzione di hash basata su una somma pesata dei caratteri nella stringa nome, 
//utilizzando un fattore moltiplicativo variabile e un modulo con la dimensione della tabella
int hash_fun(tabella_hash ht, char* nome);

//Restituisce il numero di elementi presenti nella tabella hash
int ottieni_num_elem(tabella_hash ht);






