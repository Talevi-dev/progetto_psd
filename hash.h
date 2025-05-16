typedef struct hash* tabella_hash;

#include "attivita.h"

#define MAX_HASH 100

tabella_hash nuova_hash(int dimensione);

void inserisci_hash(tabella_hash ht, attivita elem);

void cancella_hash(tabella_hash ht, attivita da_eliminare);

void stampa_hash(tabella_hash ht, int priorita);

void distruggi_hash(tabella_hash ht);

void salva_hash(tabella_hash ht, const char *nome_file);

void carica_hash(tabella_hash ht, const char *nome_file);

void report_settimanale_hash(tabella_hash ht);

int cerca_hash(tabella_hash ht, char* nome, attivita** out);

int hash_fun(tabella_hash ht, char* nome);

int ottieni_num_elem(tabella_hash ht);






