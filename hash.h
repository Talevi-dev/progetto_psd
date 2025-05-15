typedef struct hash* tabella_hash;

#include "attivita.h"

tabella_hash nuova_hash();

int hash_fun(tabella_hash ht, char* nome);

void inserisci_hash(tabella_hash ht, attivita elem);

int cerca(tabella_hash ht, char* nome, attivita** out);

void distruggi_hash(tabella_hash ht);

void stampa_hash(tabella_hash ht, int priorita);

void cancella(tabella_hash ht, attivita da_eliminare);