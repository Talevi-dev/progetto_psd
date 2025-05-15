#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "utils.h"
#include "attivita.h"

struct act{
    char nome[MAX_NOME];
    char corso[MAX_NOME];
    char descrizione[MAX_DESC];
    int priorita;
    int status;
    time_t tempo_completato;
    time_t tempo_stimato;
    time_t scadenza;
};

attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t cmp, time_t stm, time_t scd){
    attivita nuova = (attivita)malloc(sizeof(struct act));
    if (nuova == NULL){
        printf("Errore: impossibile allocare memoria per la nuova attività.\n");
        exit(EXIT_FAILURE);
    }
    
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

attivita input_attivita(){
    char nome[MAX_NOME], corso[MAX_NOME], descrizione[MAX_DESC], data[MAX_DATA];
    int priorita, ore;
    time_t stima = 0;
    time_t scadenza;

    printf("==================================================================================================\n");
    printf("|                            Inserire il nome dell' attività                                     |\n");
    printf("==================================================================================================\n");
    while (input_stringa(nome, MAX_NOME)){
        printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    system("clear");
    printf("==================================================================================================\n");
    printf("|                               Inserire il nome del corso                                       |\n");
    printf("==================================================================================================\n");
    while (input_stringa(corso, MAX_NOME)){
        printf("Attenzione, il nome del corso è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    system("clear");
    printf("==================================================================================================\n");
    printf("|                           Inserire la descrizione dell' attività                               |\n");
    printf("==================================================================================================\n");
    while (input_stringa(descrizione, MAX_DESC)){
        printf("Attenzione, la descrizione è troppo lunga, riprovare.(max 300 caratteri)\n");
    }
    
    system("clear");
    printf("==================================================================================================\n");
    printf("|                Inserire la priorità:  | Bassa [1] | Media [2] | Alta [3] |                     |\n");
    printf("==================================================================================================\n");
    while (input_intero(&priorita, 1, 3)){
        printf("Errore: digitare un intero compreso tra 0 e 10\n");
    }

    system("clear");
    printf("==================================================================================================\n");
    printf("|                    Inserire il tempo stimato per completare l'attività in ore                  |\n");
    printf("==================================================================================================\n");
    while (input_intero(&ore, 1, INT_MAX)){
        printf("Errore: inserire un numero di ore maggiore di 0.\n");
    }

    stima = ore * 3600;

    pulisci_buffer();
    system("clear");
    printf("==================================================================================================\n");
    printf("|             Inserire la data di scadenza, utilizzare il formato dd/mm/yyyy hh/mm               |\n");
    printf("==================================================================================================\n");
    while (1){
        input_stringa(data, MAX_DATA);
        if (controlla_input_data(data)){
            time_t ora_attuale = time(NULL);
            scadenza = converti_data(data);
            if (scadenza > ora_attuale){
                break;
            }
            printf("Errore: la data inserita è gia passata.\n");
        } 
    }
    system("clear");
    
    return crea_attivita(nome, corso, descrizione, priorita, 0, 0, stima, scadenza); 
}

void copia_attivita(attivita a, attivita b) {
    strncpy(a->nome, b->nome, MAX_NOME - 1);
    a->nome[MAX_NOME - 1] = '\0';

    strncpy(a->corso, b->corso, MAX_NOME - 1);
    a->corso[MAX_NOME - 1] = '\0';

    strncpy(a->descrizione, b->descrizione, MAX_DESC - 1);
    a->descrizione[MAX_DESC - 1] = '\0';

    a->priorita = b->priorita;
    a->status = b->status;
    a->tempo_completato = b->tempo_completato;
    a->tempo_stimato = b->tempo_stimato;
    a->scadenza = b->scadenza;
}


char *ottieni_nome(attivita a){
    return a -> nome;
}

char *ottieni_corso(attivita a){
    return a -> corso;
}

char *ottieni_descrizione(attivita a){
    return a -> descrizione;
}

int ottieni_priorita(attivita a){
    return a -> priorita;
}

int ottieni_status(attivita a){
    return a -> status;
}

time_t ottieni_tempo_completato(attivita a){
    return a -> tempo_completato;
}

time_t ottieni_tempo_stimato(attivita a){
    return a -> tempo_stimato;
}

time_t ottieni_scadenza(attivita a){
    return a -> scadenza;
}

void aggiorna_corso(attivita a, char *nuovo){
    strncpy(a->corso, nuovo, MAX_NOME - 1);
    a->corso[MAX_NOME - 1] = '\0';
}

void aggiorna_descrizione(attivita a, char *nuovo){
    strncpy(a->descrizione, nuovo, MAX_DESC - 1);
    a->descrizione[MAX_DESC - 1] = '\0';
}

void aggiorna_priorita(attivita a, int p){
    a -> priorita = p;
}

void aggiorna_status(attivita a){
    time_t ora_attuale = time(NULL);
    
    if ((a -> tempo_completato) >= (a -> tempo_stimato)){
        a -> status = 1;
    }else if(((a -> scadenza) < ora_attuale)){
        a -> status = -1;
    }else{
        a -> status = 0;
    }
}

void aggiorna_tempo_completato(attivita a, int o){
    a -> tempo_completato += (o * 3600);
}

void aggiorna_tempo_stimato(attivita a, int o){
    (a -> tempo_stimato) = (o * 3600);
}

void aggiorna_scadenza(attivita a, time_t nuova){
    a -> scadenza = nuova;
}