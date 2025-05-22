#include <time.h>

#define MAX_DATA 16+1
/*
* Svuota il buffer di input per evitare letture indesiderate
*/
void pulisci_buffer();
/*
* Pulisce il contenuto del terminale, adattandosi al sistema operativo in uso
*/
void pulisci_schermo();
/* 
* Legge una stringa in input e controlla che sia della dimensione corretta
* Restituisce 0 se l'input è corretto e 1 se la stringa è troppo lunga
*/
int input_stringa(char *buff, int len);
/* 
* Legge un intero in input e controlla che sia compreso tra x e y
* Restituisce 0 se l'input è corretto e 1 se l'input è fuori dal range
*/
int input_intero(int *input, int x, int y);
/*
* Controlla che la stringa in input sia nel formato corretto "dd/mm/yyyy hh:mm"
* Restituisce 1 se il formato è corretto e 0 altrimenti
*/
int controlla_input_data(char *data);
/*
* Verifica se una data cade nella settimana corrente
* Restituisce 1 se è cosi e zero altrimenti
*/
int controllo_settimana(time_t scadenza);
/*
* Converte un stringa nel formato dd/mm/yyyy hh:mm in un valore time_t
* Restituisce il valore converitito
*/
time_t converti_data (char *data);
