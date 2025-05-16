#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "utils.h"
#include "attivita.h"


// Definizione della struttura dati che rappresenta un attività
struct act{
    char nome[MAX_NOME];            // Nome dell'attivita
    char corso[MAX_NOME];           // Nome del corso 
    char descrizione[MAX_DESC];     // Descrizine dell'attività
    int priorita;                   // Priorità, assume valori: 1 (Bassa), 2 (Media), 3 (Alta)
    int status;                     // Status, assume valori: 0 (In corso), 1 (Completata), -1(In ritardo)
    time_t tempo_completato;        // Tempo, in secondi, dello studio già completato sull'attività
    time_t tempo_stimato;           // Tempo, in secondi, stimato per portare a termine l'attività
    time_t scadenza;                // Data di scadenza: in secodi trascorsi dal 1 Gennaio 1970
};

/*
* Funzione che crea una nuova attività a partire dai parametri forniti in input
* Restituisce un puntatore alla nuova struttura allocata dinamicamente
*/
attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t cmp, time_t stm, time_t scd){
    attivita nuova = (attivita)malloc(sizeof(struct act));
    if (nuova == NULL){
        printf("Errore: impossibile allocare memoria per la nuova attività.\n");
        exit(EXIT_FAILURE);
    }
    
    //Utilizzo di strncpy per assicurarsi una copia corretta delle stringhe negli appositi campi
    strncpy(nuova->nome, n, MAX_NOME - 1);
    nuova->nome[MAX_NOME - 1] = '\0';

    strncpy(nuova->corso, c, MAX_NOME - 1);
    nuova->corso[MAX_NOME - 1] = '\0';

    strncpy(nuova->descrizione, d, MAX_DESC - 1);
    nuova->descrizione[MAX_DESC - 1] = '\0';

    nuova->priorita = p;
    nuova->status = x;
    nuova->tempo_completato = cmp;
    nuova->tempo_stimato = stm;
    nuova->scadenza = scd;

    return nuova;
}

/*
* Funzione che chiede ad un utente di inserire i dati di una nuova attivita.
* Realizza controlli su ogni input e restituisce un puntatore all'attività creata 
*/
attivita input_attivita(){
    char nome[MAX_NOME], corso[MAX_NOME], descrizione[MAX_DESC], data[MAX_DATA];
    int priorita, ore;

    // Time_t indica il tempo trascorso in secondi dal 1 gennaio 1970 
    time_t stima = 0;
    time_t scadenza;

    printf("==================================================================================================\n");
    printf("|                            Inserire il nome dell' attività                                     |\n");
    printf("==================================================================================================\n");

    // Input del nome e controllo sulla strigna inserita
    while (input_stringa(nome, MAX_NOME)){
        printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    system("clear");
    printf("==================================================================================================\n");
    printf("|                               Inserire il nome del corso                                       |\n");
    printf("==================================================================================================\n");

    // Input del corso e controllo sulla strigna inserita
    while (input_stringa(corso, MAX_NOME)){
        printf("Attenzione, il nome del corso è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    system("clear");
    printf("==================================================================================================\n");
    printf("|                           Inserire la descrizione dell' attività                               |\n");
    printf("==================================================================================================\n");

    // Input della descrizione e controllo sulla strigna inserita
    while (input_stringa(descrizione, MAX_DESC)){
        printf("Attenzione, la descrizione è troppo lunga, riprovare.(max 300 caratteri)\n");
    }
    
    system("clear");
    printf("==================================================================================================\n");
    printf("|                Inserire la priorità:  | Bassa [1] | Media [2] | Alta [3] |                     |\n");
    printf("==================================================================================================\n");

    // Input della priorita e controllo sull'intero inserito
    while (input_intero(&priorita, 1, 3)){
        printf("Errore: digitare un intero compreso tra 0 e 10\n");
    }

    system("clear");
    printf("==================================================================================================\n");
    printf("|                    Inserire il tempo stimato per completare l'attività in ore                  |\n");
    printf("==================================================================================================\n");

    // Input delle ore stimate e controllo sull'intero inserito
    while (input_intero(&ore, 1, INT_MAX)){
        printf("Errore: inserire un numero di ore maggiore di 0.\n");
    }

    // Conversione da ore a secondi per rendere il dato congruo al tipo time_t 
    stima = ore * 3600;

    pulisci_buffer();
    system("clear");
    printf("==================================================================================================\n");
    printf("|             Inserire la data di scadenza, utilizzare il formato dd/mm/yyyy hh:mm               |\n");
    printf("==================================================================================================\n");

    while (1){

        input_stringa(data, MAX_DATA);

        // Controllo della correttezza del formato della data
        if (controlla_input_data(data)){

            // Coversione della data inserita da stringa a time_t
            scadenza = converti_data(data);

            // Calcolo di data e orario correnti
            time_t ora_attuale = time(NULL);

            // Confronto tra scadenza e ora corrente per verificare che la data non sia già passata
            if (scadenza > ora_attuale){
                break;
            }
            printf("Errore: la data inserita è gia passata.\n");
        } 
    }

    system("clear");
    
    // Creazione e restituzione dell'attivitaìà
    return crea_attivita(nome, corso, descrizione, priorita, 0, 0, stima, scadenza); 
}

// Funzione che restituisce il nome di una data attività
char *ottieni_nome(attivita a){
    return a -> nome;
}

// Funzione che restituisce il nome del corso di una data attività
char *ottieni_corso(attivita a){
    return a -> corso;
}

// Funzione che restituisce la descrizione di una data attività
char *ottieni_descrizione(attivita a){
    return a -> descrizione;
}

// Funzione che restituisce la priorità di una data attività
int ottieni_priorita(attivita a){
    return a -> priorita;
}

// Funzione che restituisce lo status di una data attività
int ottieni_status(attivita a){
    return a -> status;
}

// Funzione che restituisce il tempo completato di una data attività
time_t ottieni_tempo_completato(attivita a){
    return a -> tempo_completato;
}

// Funzione che restituisce il tempo stimato di una data attività
time_t ottieni_tempo_stimato(attivita a){
    return a -> tempo_stimato;
}

// Funzione che restituisce la scadenza di una data attività
time_t ottieni_scadenza(attivita a){
    return a -> scadenza;
}

// Funzione che aggiorna il nome del corso di una data attività
void aggiorna_corso(attivita a, char *nuovo){
    strncpy(a->corso, nuovo, MAX_NOME - 1);
    a->corso[MAX_NOME - 1] = '\0';
}

// Funzione che aggiorna la descrizione di una data attività
void aggiorna_descrizione(attivita a, char *nuovo){
    strncpy(a->descrizione, nuovo, MAX_DESC - 1);
    a->descrizione[MAX_DESC - 1] = '\0';
}

// Funzione che aggiorna la priorità di una data attività
void aggiorna_priorita(attivita a, int p){
    a -> priorita = p;
}

// Funzione che aggiorna lo status di una data attività
void aggiorna_status(attivita a){

    // Calcolo di data e orario correnti
    time_t ora_attuale = time(NULL);
    
    // Controllo se l'attivita è stata portat a tremine
    if ((a -> tempo_completato) >= (a -> tempo_stimato)){
        a -> status = 1;

    // Controllo se l'attività è in ritardo
    }else if(((a -> scadenza) < ora_attuale)){
        a -> status = -1;

    // L'attivita è in corso
    }else{
        a -> status = 0;
    }
}

// Funzione che aggiorna il tempo completato di una data attività
void aggiorna_tempo_completato(attivita a, int o){
    a -> tempo_completato += (o * 3600);
}

// Funzione che aggiorna il tempo stimato di una data attività
void aggiorna_tempo_stimato(attivita a, int o){
    (a -> tempo_stimato) = (o * 3600);
}

// Funzione che aggiorna la scadenza di una data attività
void aggiorna_scadenza(attivita a, time_t nuova){
    a -> scadenza = nuova;
}