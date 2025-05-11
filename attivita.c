#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils_attivita.h"
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
    
    strcpy(nuova->nome, n);
    strcpy(nuova->corso, c);
    strcpy(nuova->descrizione, d);
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

    printf("Inserire il nome dell' attività.\n");
    while (input_stringa(nome, MAX_NOME)){
        printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    printf("Inserire il nome del corso.\n");
    while (input_stringa(corso, MAX_NOME)){
        printf("Attenzione, il nome del corso è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    printf("Inserire la descrizione dell' attività.\n");
    while (input_stringa(descrizione, MAX_DESC)){
        printf("Attenzione, la descrizione è troppo lunga, riprovare.(max 300 caratteri)\n");
    }
    
    while (1){
        printf("Inserire la priorità:\n| Bassa [0|1|2|3] | Media [4|5|6] | Alta [7|8|9|10] |\n");
        int r = scanf("%d", &priorita);
        if (priorita <= 10 && priorita >=  0 && r == 1){
            break;
        } 
        while (getchar() != '\n');
        printf("Errore: digitare un intero compreso tra 1 e 9\n");
    }

    while (1){
        printf("Inserire il tempo stimato per completare l'attività in ore.\n");
        int r = scanf("%d", &ore);
        if (ore >= 1 && r == 1){
            stima += (ore*3600);
            break;
        } 
        while (getchar() != '\n');
        printf("Errore: inserire un numero di ore maggiore di 0.\n");
    }

    while (getchar() != '\n');

    while (1){
        printf("Inserire la data di scadenza, utilizzare il formato dd/mm/yyyy hh/mm.\n");
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
    
    return crea_attivita(nome, corso, descrizione, priorita, 0, 0, stima, scadenza); 
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

void aggiorna_nome(attivita a, char *nuovo){
    strcpy(a->nome, nuovo);
}

void aggiorna_corso(attivita a, char *nuovo){
    strcpy(a->corso, nuovo);
}

void aggiorna_descrizione(attivita a, char *nuovo){
    strcpy(a->descrizione, nuovo);
}

void aggiorna_priorita(attivita a, int p){
    a -> priorita = p;
}

void aggiorna_status(attivita a){
    time_t ora_attuale = time(NULL);
    
    if ((a -> tempo_completato) == (a -> tempo_stimato)){
        a -> status = 1;
    }else if (((a -> scadenza) < ora_attuale)){
        a -> status = -1;
    }
}

void aggiorna_tempo_completato(attivita a, int o){
    a -> tempo_completato += (o * 3600);
}

void aggiorna_tempo_stimato(attivita a, int o){
    (a -> tempo_stimato) += (o * 3600);
}

void aggiorna_scadenza(attivita a, time_t nuova){
    a -> scadenza = nuova;
}