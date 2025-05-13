#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "attivita.h"

void stampa_attivita(attivita a){
    printf("ID: %d\n", ottieni_ID(a));
    printf("Nome: %s\n", ottieni_nome(a));
    printf("Corso: %s\n", ottieni_corso(a));
    printf("Descrizione: %s\n", ottieni_descrizione(a));

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
    printf("Tempo completato: %ld\n", ((ottieni_tempo_completato(a))/3600));
    printf("Tempo stimato: %ld\n", ((ottieni_tempo_stimato(a))/3600));
    printf("Percentuale di progresso: %ld%%\n", (ottieni_tempo_completato(a) * 100) / ottieni_tempo_stimato(a));
    time_t scd = ottieni_scadenza(a);
    printf("Scadenza: %s\n", ctime(&scd));
}

int confronta_attivita(attivita a, attivita b){
    return (!strcmp(ottieni_nome(a), ottieni_nome(b)) && 
            !strcmp(ottieni_corso(a), ottieni_corso(b)) && 
            !strcmp(ottieni_descrizione(a), ottieni_descrizione(b)) &&
            (ottieni_ID(a) == ottieni_ID(b)) &&
            (ottieni_priorita(a) == ottieni_priorita(b)) &&
            (ottieni_status(a) == ottieni_status(b)) &&
            (ottieni_tempo_completato(a) == ottieni_tempo_completato(b)) &&
            (ottieni_tempo_stimato(a) == ottieni_tempo_stimato(b)) &&
            (ottieni_scadenza(a) == ottieni_scadenza(b)));
}

int confronta_attivita_priorita(attivita a, attivita b){
    if (ottieni_priorita(a) != ottieni_priorita(b)){
        return ottieni_priorita(a) - ottieni_priorita(b);
    }else{
        return ottieni_ID(a) - ottieni_ID(b);
    }
}