#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#include "utils.h"
#include "attivita.h"
#include "hash.h"
#include "funzioni_attivita.h"

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
        printf("In corso \n");
    }else if (s == 1){
        printf("Completata \n");
    }else{
        printf("In ritardo \n");
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
        printf(" In ritardo ");
    }

    time_t scd = ottieni_scadenza(a);
    printf("| Progresso: %.2f%% | Scadenza %s", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100, ctime(&scd));
    printf("==================================================================================================\n");
}

void elimina_attivita(tabella_hash ht, char* nome){
    attivita* elenco;
    int quanti = cerca_hash(ht, nome, &elenco);

    if (quanti == 0) {
        printf("Nessuna attività trovata con nome '%s'\n", nome);
        free(elenco);
        sleep(2);
        return;
    }

    visualizza_attivita(elenco, quanti);

    int annullata;
    attivita da_eliminare = scegli_attivita(elenco, quanti, &annullata);
    if (annullata) {
        free(elenco);
        return;
    }
    
    cancella_hash(ht, da_eliminare);
    free(elenco);

    printf("==================================================================================================\n");
    printf("|                                   Attivita eliminata                                           |\n");
    printf("==================================================================================================\n");
    sleep(2);
}

void modifica_attivita(tabella_hash ht, char* nome, int mod){
    attivita* elenco;
    int quanti = cerca_hash(ht, nome, &elenco);

    if (quanti == 0) {
        printf("Nessuna attività trovata con nome '%s'\n", nome);
        free(elenco);
        sleep(2);
        return;
    }

    visualizza_attivita(elenco, quanti);

    int annullata;
    attivita da_modificare = scegli_attivita(elenco, quanti, &annullata);
    if (annullata) {
        free(elenco);
        return;
    }

    pulisci_buffer();
    switch (mod)
    {
    case 1:
        char nuovo_crs[MAX_NOME];
        printf("Inserire il nuovo nome del corso.\n");
        while (input_stringa(nuovo_crs, MAX_NOME)){
            printf("Attenzione, il nome del corso è troppo lungo, riprovare.(max 50 caratteri)\n");
        }
        aggiorna_corso(da_modificare, nuovo_crs);
        break;
    
    case 2:
        char nuovo_dsc[MAX_DESC];
        printf("Inserire la nuova descrizone.\n");
        while (input_stringa(nuovo_dsc, MAX_DESC)){
            printf("Attenzione, la descrizione è troppo lunga, riprovare.(max 50 caratteri)\n");
        }
        aggiorna_descrizione(da_modificare, nuovo_dsc);
        break;

    case 3:
        int priorita;
        printf("Inserire la nuova priorità:\n| Alta [3] | Media [2] | Bassa [1] |\n");
        while (input_intero(&priorita, 1, 3)){
            printf("Errore: digitare un intero compreso tra 0 e 10\n");
        }
        aggiorna_priorita(da_modificare, priorita);
        break;

    case 4:
        int ore_cmp;
        printf("Inserire il le ore compeletate di questa attivita.\n");
        while (input_intero(&ore_cmp, 1, (ottieni_tempo_stimato(da_modificare)/3600))){
            printf("Errore: inserire un numero di ore compreso tra 0 e il tempo stimato(%ld).\n,", (ottieni_tempo_stimato(da_modificare)/3600));
        }
        aggiorna_tempo_completato(da_modificare, ore_cmp);
        aggiorna_status(da_modificare);
        break;

    case 5:
        int ore_stm;
        printf("Inserire il nuovo tempo stimato per completare l'attività in ore.\n");
        while (input_intero(&ore_stm, ottieni_tempo_completato(da_modificare)/3600, INT_MAX)){
            printf("Errore: inserire un numero di ore maggiore o uguale al tempo completato.\n");
        }
        aggiorna_tempo_stimato(da_modificare, ore_stm);
        aggiorna_status(da_modificare);
        break;
    
    case 6:
        char data[MAX_DATA];
        time_t scadenza;
        while (1){
            printf("Inserire la nuova data di scadenza, utilizzare il formato dd/mm/yyyy hh:mm.\n");
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

        aggiorna_scadenza(da_modificare, scadenza);
        aggiorna_status(da_modificare);
        break;
    
    default:
        break;
    }

    free(elenco);  
    printf("==================================================================================================\n");
    printf("|                         Attivita modificata correttamente                                      |\n");
    printf("==================================================================================================\n");
    sleep(2);
}

attivita scegli_attivita(attivita* elenco, int quanti, int* annullata) {
    *annullata = 0;

    printf("\n==================================================================================================\n");
    printf("|       Inserisci l'indice dell'attività da selezionare [1-%d] o 0 per annullare l'operazione     |\n", quanti);
    printf("==================================================================================================\n");

    int scelta;
    while (input_intero(&scelta, 0, quanti)) {
        printf("Errore: digitare un intero compreso tra 0 e %d\n", quanti);
    }

    if (scelta == 0) {
        *annullata = 1;
        return NULL;
    }

    return elenco[scelta - 1];
}

void visualizza_attivita(attivita* elenco, int quanti){
    system("clear");
    if (quanti == 1) {
        printf("==================================================================================================\n");
        printf("|                              Attività trovata                                                  |\n");
        printf("==================================================================================================\n\n");
        stampa_attivita(elenco[0]);
    } else {
        printf("==================================================================================================\n");
        printf("|                                  %d attività trovate                                           |\n", quanti);
        printf("==================================================================================================\n\n");
        for (int i = 0; i < quanti; i++) {
            printf("                                          [%d]\n", i + 1);
            stampa_attivita(elenco[i]);
        }
    }
}
