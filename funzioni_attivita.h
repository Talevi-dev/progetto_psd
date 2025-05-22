#include "attivita.h"
#include "hash.h"

//Stampa a schermo tutti i dettagli dell'attivita in input in un formato leggibile
void stampa_attivita(attivita a);

//Stampa sul file passato in input tutti i dettagli dell'attivita in input in un formato leggibile
void stampa_attivita_file(attivita a, FILE *fd);

//Consente all'utente di eliminare un'attività dalla tabella hash ht in base al nome fornito. 
//Se vengono trovate più attività con quel nome, l'utente può scegliere quale eliminare. 
//La funzione gestisce anche la visualizzazione dell'elenco delle attività trovate e la conferma dell'eliminazione
void elimina_attivita(tabella_hash ht, char* nome);

//Consente all'utente di modificare un dei campi dell'attività, identificata dal nome in input. 
//L'operazione viene eseguita solo dopo che l’utente seleziona l’attività tra tutte quelle con lo stesso nome
void modifica_attivita(tabella_hash ht, char* nome, int mod);

//Stampa a schermo le attivita presenti nell'array passato in input
void visualizza_attivita(attivita* elenco, int quanti);

//Permette all'utente di selezionare un'attività tra quelle presenti nella lista data in input. 
//Restituisce l'attività selezionata o NULL se l'operazione è annullata
attivita scegli_attivita(attivita* elenco, int quanti);


