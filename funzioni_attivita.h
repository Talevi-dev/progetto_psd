#include "attivita.h"
#include "hash.h"

void stampa_attivita(attivita a);

void stampa_attivita_corta(attivita a);

void elimina_attivita(tabella_hash ht, char* nome);

void visualizza_attivita(attivita* elenco, int quanti);

void modifica_attivita(tabella_hash ht, char* nome, int mod);

attivita scegli_attivita(attivita* elenco, int quanti, int* annullata);


