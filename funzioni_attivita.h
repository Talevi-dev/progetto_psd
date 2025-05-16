#include "attivita.h"
#include "hash.h"

void stampa_attivita(attivita a);

void elimina_attivita(tabella_hash ht, char* nome);

void modifica_attivita(tabella_hash ht, char* nome, int mod);

void visualizza_attivita(attivita* elenco, int quanti);

attivita scegli_attivita(attivita* elenco, int quanti, int* annullata);


