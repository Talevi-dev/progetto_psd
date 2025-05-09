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
    time_t scadenza;
};

attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t scd){
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
    nuova->scadenza = scd;

    return nuova;
}

attivita input_attivita(){
    char nome[MAX_NOME], corso[MAX_NOME], descrizione[MAX_DESC], data[MAX_DATA];
    int priorita;
    time_t scadenza;

    printf("Inserire il nome dell' attività.\n");
    if (input_stringa(nome, MAX_NOME)){
        printf("Attenzione, il nome dell' attività è troppo lungo ed è stato troncato.\n");
    }

    printf("Inserire il nome del corso.\n");
    if (input_stringa(corso, MAX_NOME)){
        printf("Attenzione, il nome del corso è troppo lunga ed è stato troncato.\n");
    }

    printf("Inserire la descrizione dell' attività.\n");
    if (input_stringa(descrizione, MAX_DESC)){
        printf("Attenzione, la descrizione è troppo lunga ed è stata troncata.\n");
    }
    
    while (1){
        printf("Inserire la priorità: bassa [1|2|3], media [3|5|6], alta [7|8|9]");
        scanf("%d", priorita);
        if (priorita < 10 && priorita > 0){
            break;
        } 
        printf("Errore: digitare un intero compreso tra 1 e 9\n");
    }

    while (1){
        printf("Inserire la data di scadenza, utilizzare il formato dd/mm/yyyy hh/mm.\n");
        input_stringa(data, MAX_DATA);
        if (controlla_input_data(data)){
            scadenza = converti_data(data);
            break;
        } 
    }
    
    return crea_attivita(nome, corso, descrizione, priorita, 0, scadenza); 
}
