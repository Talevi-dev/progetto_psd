#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "attivita.h"

void stampa_attivita(attivita a){
    printf("Nome: %s\n", ottieni_nome(a));
    printf("Corso: %s\n", ottieni_corso(a));
    printf("Descrizione: %s\n", ottieni_descriione(a));

    int p = ottieni_priorita(a);
    printf("Priorità: ");
    if ((p) <= 3){
        printf("Bassa(%d)\n", p);
    }else if ((p) <= 6){
        printf("Media(%d)\n", p);
    }else{
        printf("Alta(%d)\n", p);
    }

    int s = ottieni_status(a);
    printf("Status: ");
    if (s == 0){
        printf("In corso\n");
    }else if (s == 1){
        printf("Completata\n");
    }else{
        printf("Scaduta\n");
    }

    printf("Tempo stimato: %ld\n", ((ottieni_durata(a))/3600));
    time_t scd = ottieni_scadenza(a);
    printf("Scadenza: %s\n", ctime(&scd));
}

int confronta_attivita(attivita a, attivita b){
    return (!strcmp(ottieni_nome(a), ottieni_nome(b)) && 
            !strcmp(ottieni_corso(a), ottieni_corso(b)) && 
            !strcmp(ottieni_descriione(a), ottieni_nome(b)));
}