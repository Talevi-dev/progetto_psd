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
    time_t durata;
    time_t scadenza;
};

attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t drt, time_t scd){
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
    nuova->durata = drt;
    nuova->scadenza = scd;

    return nuova;
}

attivita input_attivita(){
    char nome[MAX_NOME], corso[MAX_NOME], descrizione[MAX_DESC], data[MAX_DATA];
    int priorita, ore;
    time_t durata = 0;
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
        scanf("%d", &priorita);
        if (priorita < 10 && priorita > 0){
            break;
        } 
        printf("Errore: digitare un intero compreso tra 1 e 9\n");
    }

    while (1){
        printf("Inserire il tempo stimato per completare l'attività in ore.\n");
        scanf("%d", &ore);
        if (ore >= 1){
            durata += (ore*3600);
            break;
        } 
    }

    while (1){
        printf("Inserire la data di scadenza, utilizzare il formato dd/mm/yyyy hh/mm.\n");
        input_stringa(data, MAX_DATA);
        if (controlla_input_data(data)){
            time_t ora_attuale = time(NULL);
            if ((scadenza = converti_data(data)) > ora_attuale){
                break;
            }
            printf("Errore, la scadenza inserita è già passata\n");
        } 
    }
    
    return crea_attivita(nome, corso, descrizione, priorita, 0, durata, scadenza); 
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

time_t ottieni_durata(attivita a){
    return a -> durata;
}

time_t ottieni_scadenza(attivita a){
    return a -> scadenza;
}

void aggiorna_nome(attivita a){
    char nuovo[MAX_NOME];
    while (input_stringa(nuovo, MAX_NOME)){
        printf("Attenzione, il nome dell' attività è troppo lungo ed è stato troncato.\n");
    }
}

void aggiorna_corso(attivita a){
    char nuovo[MAX_NOME];
    while (input_stringa(nuovo, MAX_NOME)){
        printf("Attenzione, il nome del corso è troppo lungo ed è stato troncato.\n");
    }
}

void aggiorna_descrizione(attivita a){
    char nuovo[MAX_DESC];
    while (input_stringa(nuovo, MAX_DESC)){
        printf("Attenzione, la descrizione è troppo lunga ed è stato troncato.\n");
    }
}

/*void stampa_attivita(attivita a){
    printf("Nome: %s\n", a ->nome);
    printf("Corso: %s\n", a -> corso);
    printf("Descrizione: %s\n", a-> descrizione);

    printf("Priorità: ");
    if ((a -> priorita) <= 3){
        printf("Bassa(%d)\n", a -> priorita);
    }else if ((a -> priorita) <= 6){
        printf("Media(%d)\n", a -> priorita);
    }else{
        printf("Alta(%d)\n", a -> priorita);
    }

    printf("Status: ");
    if ((a -> status) == 0){
        printf("In corso\n");
    }else if ((a -> status) == 1){
        printf("Completata\n");
    }else{
        printf("Scaduta\n");
    }

    printf("Tempo stimato: %ld\n", ((a->durata)/3600));
    printf("Scadenza: %s\n", ctime(&(a->scadenza)));
}*/