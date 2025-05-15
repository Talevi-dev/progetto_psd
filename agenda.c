#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#include "attivita.h"
#include "utils.h"
#include "hash.h"
#include "funzioni_attivita.h"

#define FILE_DATI "dati.txt"

void elimina(tabella_hash ht, char* nome);

void modifica(tabella_hash ht, char* nome, int mod);

attivita visualizza(tabella_hash ht, char* nome, int* annullata);

int main(){
    system("clear");
    tabella_hash ht = nuova_hash();
    carica_hash(ht, FILE_DATI);
    while (1){
        system("clear");
        printf("==================================================================================================\n");
        printf("|                           HAI %d ATTIVITA INSERITE NELL'AGENDA                                 |\n", ottieni_num_elem(ht));
        printf("==================================================================================================\n");
        printf("| [1] INSERISCI ATTIVITA                                                                         |\n");
        printf("| [2] CANCELLA ATTIVITA                                                                          |\n");
        printf("| [3] MODIFICA/AGGIORNA ATTIVITA                                                                 |\n");
        printf("| [4] VISUALIZZA ATTIVITA                                                                        |\n");
        printf("| [0] CHIUDI                                                                                     |\n");
        printf("==================================================================================================\n");

        int scelta;
        while (input_intero(&scelta, 0, 4)){
            printf("Errore: digitare un intero compreso tra 0 e 4\n");
        }

        pulisci_buffer();
        system("clear");
        char nome[MAX_NOME];
        switch (scelta){
        case 1:
            attivita temp = input_attivita();
            inserisci_hash(ht, temp);
            printf("==================================================================================================\n");
            printf("|                           Attivita inserita correttamente                                      |\n");
            printf("==================================================================================================\n");
            sleep (2);
            break;
    
        case 2:
            printf("==================================================================================================\n");
            printf("!                      Inserire il nome dell' attività da eliminare                              |\n");
            printf("==================================================================================================\n");
            while (input_stringa(nome, MAX_NOME)){
                printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
            }
            elimina(ht, nome);
            break;

        case 3:
            int scelta;
            printf("==================================================================================================\n");
            printf("!                      Inserire il nome dell' attività da modificare                             |\n");
            printf("==================================================================================================\n");
            while (input_stringa(nome, MAX_NOME)){
                printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
            }

            system("clear");
            printf("==================================================================================================\n");
            printf("|                               Cosa desideri modificare/aggiornare?                             |\n");
            printf("| Corso [1] | Descr.[2] | Priorità [3] | Tempo completato [4] | Tempo stimato [5] | Scadenza [6] |\n");
            printf("==================================================================================================\n");
            while (input_intero(&scelta, 1, 6)){
                printf("Errore: digitare un intero compreso tra 1 e 6\n");
            }

            modifica(ht, nome, scelta);
            break;

        case 4:
            int priorita;
            printf("==================================================================================================\n");
            printf("|                           Scegliere come visualizzare le attivita                              |\n");
            printf("|        | Tutte [0] | Per priorità: Bassa [1] | Media [2] | Alta [3] | Per Nome [4] |           |\n");
            printf("==================================================================================================\n");
            while (input_intero(&priorita, 0, 4)){
                printf("Errore: digitare un intero compreso tra 0 e 3\n");
            }

            if (priorita == 4){
                system("clear");
                printf("==================================================================================================\n");
                printf("!                      Inserire il nome dell' attività da visualizzare                           |\n");
                printf("==================================================================================================\n");
                pulisci_buffer();
                while (input_stringa(nome, MAX_NOME)){
                    printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
                }
                visualizza(ht, nome, &priorita);
            }else{
                stampa_hash(ht, priorita);
            pulisci_buffer();
            printf("Premi INVIO per continuare\n");
            getchar();
            pulisci_buffer();
            }
            
            break;

        case 0:
            salva_hash(ht, FILE_DATI);
            distruggi_hash(ht);
            exit(EXIT_SUCCESS);
            break;

        default:
            break;
        }
    }
    return 0;
}

void elimina(tabella_hash ht, char* nome){
    int annullata;
    attivita da_eliminare = visualizza(ht, nome, &annullata);
    if (annullata){
        return;
    }
    
    cancella_hash(ht, da_eliminare);

    printf("==================================================================================================\n");
    printf("|                                   Attivita eliminata                                           |\n");
    printf("==================================================================================================\n");
    sleep(2);
}

void modifica(tabella_hash ht, char* nome, int mod){
    int annullata;
    attivita da_modificare = visualizza(ht, nome, &annullata);
    if (annullata){
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
        while (input_intero(&ore_stm, 1, INT_MAX)){
            printf("Errore: inserire un numero di ore maggiore di 0.\n");
        }
        aggiorna_tempo_stimato(da_modificare, ore_stm);
        aggiorna_status(da_modificare);
        break;
    
    case 6:
        char data[MAX_DATA];
        time_t scadenza;
        while (1){
            printf("Inserire la nuova data di scadenza, utilizzare il formato dd/mm/yyyy hh/mm.\n");
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

    printf("==================================================================================================\n");
    printf("|                         Attivita modificata correttamente                                      |\n");
    printf("==================================================================================================\n");
    sleep(2);
}

attivita visualizza(tabella_hash ht, char* nome, int* annullata){
    attivita* risultati;
    int trovati = cerca_hash(ht, nome, &risultati);

    *annullata = 0;

    if (trovati == 0) {
        system("clear");
        printf("==================================================================================================\n");
        printf("|                   Nessuna attività trovata con nome '%s'                                       \n", nome);
        printf("==================================================================================================\n");
        sleep(2);
        return NULL;
    }

    printf("==================================================================================================\n");
    printf("|                   Attività trovate con nome '%s'                                               \n", nome);
    printf("==================================================================================================\n");
    for (int i = 0; i < trovati; i++) {
        printf("                                          [%d]\n", i + 1);
        stampa_attivita_corta(risultati[i]);
    }

    int scelta;
    printf("\n==================================================================================================\n");
    printf("|       Inserisci l'indice dell'attività da selezionare (1-%d) o 0 per annullare l'operazione     |\n", trovati);
    printf("==================================================================================================\n");

    while (input_intero(&scelta, 0, trovati)) {
        printf("Errore: digitare un intero compreso tra 0 e %d\n", trovati);
    }

    if (scelta == 0) {
        *annullata = 1;
        printf("==================================================================================================\n");
        printf("|                                 Operazione annullata.                                          |\n");
        printf("==================================================================================================\n");
        sleep(2);
        free(risultati);
        return NULL;
    }

    attivita selezionata = risultati[scelta - 1];
    free(risultati);
    return selezionata;
}
