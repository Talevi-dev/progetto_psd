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

// Funzione che stampa a schermo i dati dell'attività data in input
void stampa_attivita(attivita a){
    printf("==================================================================================================\n");
    printf("|Nome: %s | Corso: %s\n", ottieni_nome(a), ottieni_corso(a));
    printf("==================================================================================================\n");
    printf("|Descrizione: %s \n", ottieni_descrizione(a));
    printf("==================================================================================================\n");

    // Conversione del valore intero della priotità
    int p = ottieni_priorita(a);
    printf("|Priorità: ");
    if ((p) == 3){
        printf("Alta(%d) |", p);
    }else if ((p) == 2){
        printf("Media(%d) |", p);
    }else{
        printf("Bassa(%d) |", p);
    }

    // Calcolo della percentuale di progresso
    printf(" Percentuale di progresso: %.2f%% |", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100);

    // Conversione del valore intero dello status
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
    
    // Conversione della scadenza da time_t a stringa
    time_t scd = ottieni_scadenza(a);
    printf("| Scadenza: %s", ctime(&scd));
    printf("==================================================================================================\n");
    printf("\n");
}

// Funzione che elimina una determinata attivita dalla tabella hash in base al nome
void elimina_attivita(tabella_hash ht, char* nome){
    // Cerca tutte le attivita con il nome dato in input
    attivita* elenco;
    int ris = cerca_hash(ht, nome, &elenco);

    if (ris == 0) {
        printf("Nessuna attività trovata con nome '%s'\n", nome);
        free(elenco);
        sleep(2);
        return;
    }

    // Stampa le attivita trovate a schermo
    visualizza_attivita(elenco, ris);

    // Fa scegliere all'utente tramite input quale attivita eliminare o se annullare l'operazione
    int annullata;
    attivita da_eliminare = scegli_attivita(elenco, ris, &annullata);

    if (annullata){
        free(elenco);
        return;
    }
    
    // Cancella l'attivita selezionata dall'utente
    cancella_hash(ht, da_eliminare);
    free(elenco);

    printf("==================================================================================================\n");
    printf("|                                   Attivita eliminata                                           |\n");
    printf("==================================================================================================\n");
    sleep(2);
}

// Funzione per modificare una delle proprietà dell'attività di nome dato in input 
void modifica_attivita(tabella_hash ht, char* nome, int mod){
    // Cerca tutte le attivita con il nome dato in input
    attivita* elenco;
    int ris = cerca_hash(ht, nome, &elenco);

    if (ris == 0) {
        printf("Nessuna attività trovata con nome '%s'\n", nome);
        free(elenco);
        sleep(2);
        return;
    }

    // Stampa le attivita trovate a schermo
    visualizza_attivita(elenco, ris);

    // Fa scegliere all'utente tramite input quale attivita eliminare o se annullare l'operazione
    int annullata;
    attivita da_modificare = scegli_attivita(elenco, ris, &annullata);

    if (annullata) {
        free(elenco);
        return;
    }

    pulisci_buffer();
    // Switch per selezionarea il campo dell'attivita da modificare
    switch (mod){

    // Modifca del nome del corso
    case 1:
        char nuovo_crs[MAX_NOME];
        system("clear");
        printf("==================================================================================================\n");
        printf("|                          Inserire il nuovo nome del corso                                      |\n");
        printf("==================================================================================================\n");
        while (input_stringa(nuovo_crs, MAX_NOME)){
            printf("Attenzione, il nome del corso è troppo lungo, riprovare.(max 50 caratteri)\n");
        }
        aggiorna_corso(da_modificare, nuovo_crs);
        break;
    
    // Modifica della descrizione
    case 2:
        char nuovo_dsc[MAX_DESC];
        system("clear");
        printf("==================================================================================================\n");
        printf("|                           Inserire la nuova descrizone                                         |\n");
        printf("==================================================================================================\n");
        while (input_stringa(nuovo_dsc, MAX_DESC)){
            printf("Attenzione, la descrizione è troppo lunga, riprovare.(max 50 caratteri)\n");
        }
        aggiorna_descrizione(da_modificare, nuovo_dsc);
        break;

    // Modifica della priorità
    case 3:
        int priorita;
        system("clear");
        printf("==================================================================================================\n");
        printf("|             Inserire la nuova priorità: | Bassa [1] | Media [2] | Alta [3] |                   |\n");
        printf("==================================================================================================\n");
        while (input_intero(&priorita, 1, 3)){
            printf("Errore: digitare un intero compreso tra 0 e 10\n");
        }
        aggiorna_priorita(da_modificare, priorita);
        break;

    // Modifica delle ore completate
    case 4:
        int ore_cmp;
        system("clear");
        printf("==================================================================================================\n");
        printf("|                     Inserire il le ore compeletate di questa attivita                          |\n");
        printf("==================================================================================================\n");

        // Controllo che l'input dell ore completate non superi le ore stimate 
        // per evitare la generazione di percentuali prive di senso nella stampa
        while (input_intero(&ore_cmp, 1, (ottieni_tempo_stimato(da_modificare)/3600))){
            printf("Errore: inserire un numero di ore compreso tra 0 e il tempo stimato(%ld).\n,", 
                    (ottieni_tempo_stimato(da_modificare)/3600));
        }

        aggiorna_tempo_completato(da_modificare, ore_cmp);
        aggiorna_status(da_modificare);
        break;

    // Modifica delle ore stimate
    case 5:
        int ore_stm;
        system("clear");
        printf("==================================================================================================\n");
        printf("|               Inserire il nuovo tempo stimato per completare l'attività in ore                 |\n");
        printf("==================================================================================================\n");

        // Controllo che l'input dell ore stimate non sia inferiore alle ore completate 
        // per evitare la generazione di percentuali prive di senso nella stampa
        while (input_intero(&ore_stm, ottieni_tempo_completato(da_modificare)/3600, INT_MAX)){
            printf("Errore: inserire un numero di ore maggiore o uguale al tempo completato(%ld).\n", 
                    ottieni_tempo_completato(da_modificare)/3600);
        }

        aggiorna_tempo_stimato(da_modificare, ore_stm);
        aggiorna_status(da_modificare);
        break;
    
    // Modifica della scadenza
    case 6:
        char data[MAX_DATA];
        time_t scadenza;

        while (1){
            system("clear");
            printf("==================================================================================================\n");
            printf("|             Inserire la nuova data di scadenza, utilizzare il formato dd/mm/yyyy hh:mm         |\n");
            printf("==================================================================================================\n");
            input_stringa(data, MAX_DATA);

            // Controllo della correttezza del formato della data
            if (controlla_input_data(data)){

                // Coversione della data inserita da stringa a time_t
                time_t ora_attuale = time(NULL);

                // Calcolo di data e orario correnti
                scadenza = converti_data(data);

                // Confronto tra scadenza e ora corrente per verificare che la data non sia già passata
                if (scadenza > ora_attuale){
                    break;
                }
                printf("Errore: la data inserita è gia passata.\n");
            } 
            sleep(1);
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

// Funzione che stampa a schermo un array di attività
void visualizza_attivita(attivita* elenco, int ris){
    system("clear");

    if (ris == 1){
        printf("==================================================================================================\n");
        printf("|                              Attività trovata                                                  |\n");
        printf("==================================================================================================\n\n");
        stampa_attivita(elenco[0]);
    }else{
        printf("==================================================================================================\n");
        printf("|                                  %d attività trovate                                           |\n", ris);
        printf("==================================================================================================\n\n");
        for (int i = 0; i < ris; i++) {
            printf("                                          [%d]\n", i + 1);
            stampa_attivita(elenco[i]);
        }
    }
}

/*
* Funzione che permette all'utente di selezionare un'attività tra quelle presenti nella lista data in input
* Restituisce l'attività selezionata e modifica il valore dell'intero annullata
*/
attivita scegli_attivita(attivita* elenco, int ris, int* annullata){
    *annullata = 0;

    printf("\n==================================================================================================\n");
    printf("|       Inserisci l'indice dell'attività da selezionare [1-%d] o 0 per annullare l'operazione     |\n", ris);
    printf("==================================================================================================\n");

    int scelta;
    while (input_intero(&scelta, 0, ris)) {
        printf("Errore: digitare un intero compreso tra 0 e %d\n", ris);
    }

    if (scelta == 0){
        *annullata = 1;
        return NULL;
    }

    return elenco[scelta - 1];
}
