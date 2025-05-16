typedef struct hash* tabella_hash;

#include "attivita.h"

tabella_hash nuova_hash();

int ottieni_num_elem(tabella_hash ht);

int hash_fun(tabella_hash ht, char* nome);

void inserisci_hash(tabella_hash ht, attivita elem);

int cerca_hash(tabella_hash ht, char* nome, attivita** out);

void distruggi_hash(tabella_hash ht);

void stampa_hash(tabella_hash ht, int priorita);

void cancella_hash(tabella_hash ht, attivita da_eliminare);

void salva_hash(tabella_hash ht, const char *nome_file);

void carica_hash(tabella_hash ht, const char *nome_file);

void report_settimanale_hash(tabella_hash ht);