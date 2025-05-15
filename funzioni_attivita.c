#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "attivita.h"

void stampa_attivita(attivita a){
    printf("Nome: %s\n", ottieni_nome(a));
    printf("Corso: %s\n", ottieni_corso(a));
    printf("Descrizione: %s\n", ottieni_descrizione(a));

    int p = ottieni_priorita(a);
    printf("Priorità: ");
    if ((p) == 3){
        printf("Alta(%d)\n", p);
    }else if ((p) == 2){
        printf("Media(%d)\n", p);
    }else{
        printf("Bassa(%d)\n", p);
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
    printf("Percentuale di progresso: %.2f%%\n", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100);
    time_t scd = ottieni_scadenza(a);
    printf("Scadenza: %s\n", ctime(&scd));
}

void stampa_attivita2(attivita a){
    printf("|| Nome: %s||", ottieni_nome(a));
    printf("|| corso: %s||", ottieni_corso(a));

    int p = ottieni_priorita(a);
    printf("|| Priorità: ");
    if ((p) == 3){
        printf("Alta(%d)||", p);
    }else if ((p) == 2){
        printf("Media(%d)||", p);
    }else{
        printf("Bassa(%d)||", p);
    }

    int s = ottieni_status(a);
    printf("||Status: ");
    if (s == 0){
        printf("In cors||");
    }else if (s == 1){
        printf(" Completata ||");
    }else{
        printf(" Scaduta ||");
    }
    printf("|| Progresso: %.2f%% ||", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100);
    time_t scd = ottieni_scadenza(a);
    printf("|| Scadenza: %s||\n", ctime(&scd));
}