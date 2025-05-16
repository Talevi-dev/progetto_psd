#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"
#include "attivita.h"
#include "hash.h"
#include "funzioni_attivita.h"

#define FILE_DATI "dati.txt"

int main(){
    // Iniziallizza la tabella hash
    tabella_hash ht = nuova_hash(MAX_HASH);

    // Carica i dati salvati nel file dati.txt
    carica_hash(ht, FILE_DATI);

    system("clear");
    printf("==================================================================================================\n");
    printf("|                                        BENVENUTO                                               |\n");
    printf("==================================================================================================\n");
    sleep(2);

    // Loop del menù interattivo
    while (1){
        system("clear");
        printf("==================================================================================================\n");
        printf("|                           HAI %d ATTIVITA INSERITE NELL'AGENDA                                 |\n", ottieni_num_elem(ht));
        printf("==================================================================================================\n");
        printf("| [1] INSERISCI ATTIVITA                                                                         |\n");
        printf("| [2] CANCELLA ATTIVITA                                                                          |\n");
        printf("| [3] MODIFICA/AGGIORNA ATTIVITA                                                                 |\n");
        printf("| [4] VISUALIZZA ATTIVITA                                                                        |\n");
        printf("| [5] REPORT SETTIMANALE                                                                         |\n");
        printf("| [0] CHIUDI                                                                                     |\n");
        printf("==================================================================================================\n");

        int scelta;

        // Controllo sull'input dell'utente
        while (input_intero(&scelta, 0, 5)){
            printf("Errore: digitare un intero compreso tra 0 e 4\n");
        }

        pulisci_buffer();
        system("clear");

        char nome[MAX_NOME];

        // Switch per selezionare l'operazione
        switch (scelta){

        // Inserimento nuova attività
        case 1:
            attivita temp = input_attivita();
            inserisci_hash(ht, temp);
            printf("==================================================================================================\n");
            printf("|                           Attivita inserita correttamente                                      |\n");
            printf("==================================================================================================\n");
            sleep (2);
            break;
    
        // Eliminazione attività per nome
        case 2:
            printf("==================================================================================================\n");
            printf("|                      Inserire il nome dell' attività da eliminare                              |\n");
            printf("==================================================================================================\n");
            while (input_stringa(nome, MAX_NOME)){
                printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
            }
            elimina_attivita(ht, nome);
            break;

        // Modifia attività per nome
        case 3:
            int scelta;
            printf("==================================================================================================\n");
            printf("|                      Inserire il nome dell' attività da modificare                             |\n");
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

            modifica_attivita(ht, nome, scelta);
            break;

        // Visualizzaione attivita 
        case 4:
            int priorita;
            printf("==================================================================================================\n");
            printf("|                           Scegliere come visualizzare le attivita                              |\n");
            printf("|        | Tutte [0] | Per priorità: Bassa [1] | Media [2] | Alta [3] | Per Nome [4] |           |\n");
            printf("==================================================================================================\n");
            while (input_intero(&priorita, 0, 4)){
                printf("Errore: digitare un intero compreso tra 0 e 3\n");
            }

            // Visualizzazione per nome
            if (priorita == 4){
                system("clear");
                printf("==================================================================================================\n");
                printf("|                      Inserire il nome dell' attività da visualizzare                           |\n");
                printf("==================================================================================================\n");
                pulisci_buffer();
                while (input_stringa(nome, MAX_NOME)){
                    printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
                }

                // Ricerca per nome
                attivita* elenco;
                int quanti = cerca_hash(ht, nome, &elenco);
                visualizza_attivita(elenco, quanti);
                free(elenco);
            }else{
                // Visualizzazione completa o per priorità
                stampa_hash(ht, priorita);
            }

            
            printf("Premi INVIO per continuare\n");
            pulisci_buffer();
            getchar();
            
            break;

        // Report Settimanale
        case 5:
            report_settimanale_hash(ht);

            printf("Premi INVIO per continuare\n");
            pulisci_buffer();
            getchar();
            break;
        
        // Uscita dal programma
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
