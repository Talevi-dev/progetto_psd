#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "attivita.h"

void stampa_attivita(attivita a){
    printf("==================================================================================================\n");
    printf("|Nome: %s | Corso: %s\n", ottieni_nome(a), ottieni_corso(a));
    printf("==================================================================================================\n");
    printf("|Descrizione: %s \n", ottieni_descrizione(a));
    printf("==================================================================================================\n");

    int p = ottieni_priorita(a);
    printf("|Priorità: ");
    if ((p) == 3){
        printf("Alta(%d) |", p);
    }else if ((p) == 2){
        printf("Media(%d) |", p);
    }else{
        printf("Bassa(%d) |", p);
    }

    printf(" Percentuale di progresso: %.2f%% |", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100);

    int s = ottieni_status(a);
    printf(" Status: ");
    if (s == 0){
        printf("In corso |\n");
    }else if (s == 1){
        printf("Completata |\n");
    }else{
        printf("Scaduta |\n");
    }
    printf("==================================================================================================\n");
    printf("| Tempo completato: %ld ", ((ottieni_tempo_completato(a))/3600));
    printf("| Tempo stimato: %ld ", ((ottieni_tempo_stimato(a))/3600));
    
    time_t scd = ottieni_scadenza(a);
    printf("| Scadenza: %s", ctime(&scd));
    printf("==================================================================================================\n");
    printf("\n");
}

void stampa_attivita_corta(attivita a){
    printf("==================================================================================================\n");
    printf("| Nome: %s | Corso: %s | Priorità: ", ottieni_nome(a), ottieni_corso(a));

    int p = ottieni_priorita(a);
    if ((p) == 3){
        printf("Alta(%d) ", p);
    }else if ((p) == 2){
        printf("Media(%d) ", p);
    }else{
        printf("Bassa(%d) ", p);
    }

    int s = ottieni_status(a);
    printf("| Status: ");
    if (s == 0){
        printf("In corso ");
    }else if (s == 1){
        printf(" Completata ");
    }else{
        printf(" Scaduta ");
    }

    time_t scd = ottieni_scadenza(a);
    printf("| Progresso: %.2f%% | Scadenza %s", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100, ctime(&scd));
    printf("==================================================================================================\n");
}