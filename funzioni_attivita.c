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

/*
*SPECIFICA SINTATTICA
* - void stampa_attivita(attivita)
*     Tipi in uso: attivita
*     Descrizione: stampa a schermo tutti i dettagli dell'attivita in input in un formato leggibile
*
*SPECIFICA SEMANTICA
* - stampa_attivita(a)
*     Precondizioni: a è un puntatore valido e inizializzato correttamente
*     Postcondizioni: nessuna
*     Side effect: stampa a schermo i campi nome, corso e descrizione come normali stringhe, 
*                  converte il valore numerico di priorta e status in una stringa [Alta, Media, Bassa] 
*                  e [In corso, Completata, In riardo]. Stampa tempo_completato e tempo_stimato 
*                  moltiplicati per 3600 (conversione da secondi in ore) e una percentuale di completamento 
*                  ottenuta dal loro rapporto. Infine stampa la scadenza tramite la funzione ctime della libreria <time.h>
*/
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

/*
*SPECIFICA SINTATTICA
* - void stampa_attivita_file(attivita, FILE*)
*         Tipi in uso: attivita, puntatore a file 
*         Descrizione: stampa sul file passato in input tutti i dettagli dell'attivita in input in un formato leggibile
*
*SPECIFICA SEMANTICA
* - stampa_attivita_file(a, fd)
*     Precondizioni: a è un puntatore valido e inizializzato correttamente, 
*                    fd punta ad un file aperto correttamente in scrittura
*     Postcondizioni: nessuna
*     Side effect: stampa sul file puntato da fd, corso e descrizione come normali stringhe, 
*                  converte il valore numerico di priorta e status in una stringa [Alta, Media, Bassa] 
*                  e [In corso, Completata, In riardo]. Stampa tempo_completato e tempo_stimato 
*                  moltiplicati per 3600 (conversione da secondi in ore) e una percentuale di completamento 
*                  ottenuta dal loro rapporto. Infine stampa la scadenza tramite la funzione ctime della libreria <time.h>
*/
void stampa_attivita_file(attivita a, FILE *fd){
    fprintf(fd, "==================================================================================================\n");
    fprintf(fd, "|Nome: %s | Corso: %s\n", ottieni_nome(a), ottieni_corso(a));
    fprintf(fd, "==================================================================================================\n");
    fprintf(fd, "|Descrizione: %s \n", ottieni_descrizione(a));
    fprintf(fd, "==================================================================================================\n");

    // Conversione del valore intero della priotità
    int p = ottieni_priorita(a);
    fprintf(fd, "|Priorità: ");
    if ((p) == 3){
        fprintf(fd, "Alta(%d) |", p);
    }else if ((p) == 2){
        fprintf(fd, "Media(%d) |", p);
    }else{
        fprintf(fd, "Bassa(%d) |", p);
    }

    // Calcolo della percentuale di progresso
    fprintf(fd, " Percentuale di progresso: %.2f%% |", ((double)ottieni_tempo_completato(a) / ottieni_tempo_stimato(a)) * 100);

    // Conversione del valore intero dello status
    int s = ottieni_status(a);
    fprintf(fd, " Status: ");
    if (s == 0){
        fprintf(fd, "In corso \n");
    }else if (s == 1){
        fprintf(fd, "Completata \n");
    }else{
        fprintf(fd, "In ritardo \n");
    }
    fprintf(fd, "==================================================================================================\n");
    fprintf(fd, "| Tempo completato: %ld ", ((ottieni_tempo_completato(a))/3600));
    fprintf(fd, "| Tempo stimato: %ld ", ((ottieni_tempo_stimato(a))/3600));
    
    // Conversione della scadenza da time_t a stringa
    time_t scd = ottieni_scadenza(a);
    fprintf(fd, "| Scadenza: %s", ctime(&scd));
    fprintf(fd, "==================================================================================================\n");
    fprintf(fd, "\n");
}

/*
*SPECIFICA SINTATTICA
* - void elimina_attivita(tabella_hash, char*)
*     Tipi in uso: tabella_hash, stringa 
*     Descrizione: consente all'utente di eliminare un'attività dalla tabella hash ht in base al nome fornito. 
*                  Se vengono trovate più attività con quel nome, l'utente può scegliere quale eliminare. 
*                  La funzione gestisce anche la visualizzazione dell'elenco delle attività trovate e 
*                  la conferma dell'eliminazione
*
*SPECIFICA SEMANTICA
* - elimina_attivita(ht, nome)
*     Precondizioni: ht è un puntatore valido e inizializzato correttamente, 
*                    nome è un puntatore ad una stringa non nulla
*     Postcondizioni: nessuna
*     Side effect: stampa a schermo le attivita tutte le attività trovate con nome ugale alla stringa in input, 
*                  permette all'utente di selezionare una di queste o di annullare l'operazione. 
*                  Se l'operazione è stata annullata la funzione termina, altrimenti modifica ht rimuovendo 
*                  l'attivita scelta tramite la funzione cancella_hash. Stampa un messaggio per notificare l'eliminazione
*/
void elimina_attivita(tabella_hash ht, char* nome){
    // Cerca tutte le attivita con il nome dato in input
    attivita* elenco;
    int ris = cerca_hash(ht, nome, &elenco);

    // Stampa le attivita trovate a schermo
    visualizza_attivita(elenco, ris);
    
    // Termine della funzione se non sono state trovate attivita
    if (ris == 0) {
        free(elenco);
        sleep(2);
        return;
    }

    // L'utente sceglie tramite input quale attivita eliminare o se annullare l'operazione
    attivita da_eliminare = scegli_attivita(elenco, ris);

    // L'utente ha annullato l'operazione
    if (da_eliminare == NULL){
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

/*
*SPECIFICA SINTATTICA
* - void modifica_attivita(tabella_hash, char*, int)
*     Tipi in uso: tabella_hash, stringa, intero
*     Descrizione: consente all'utente di modificare un dei campi dell'attività, identificata dal nome in input. 
*                  L'operazione viene eseguita solo dopo che l’utente seleziona l’attività tra tutte quelle con lo stesso nome
*
*SPECIFICA SEMANTICA
* - modifica_attivita(ht, nome, mod)
*     Precondizioni:ht è un puntatore valido e inizializzato correttamente, nome è un puntatore ad una stringa non nulla, 
*                      mod è un intero compreso tra 1 e 6
*     Postcondizioni: nessuna
*     Side effect: l'attività selezionata viene modificata a seconda del valore di mod che governa i casi di uno switch: 
*                  mod = 1 modifica del nome del corso, mod = 2 modifica della descrizione, mod = 3 modifica della priorità, 
*                  mod = 4 modifica del tempo completato, mod = 5 modifica del tempo stimato, mod = 6 modifica della scadenza. 
*                  Vengono stampati a schermo messaggi per guidare l'input dell'utente e per confermare il termine dell'operazione
*/
void modifica_attivita(tabella_hash ht, char* nome, int mod){
    // Cerca tutte le attivita con il nome dato in input
    attivita* elenco;
    int ris = cerca_hash(ht, nome, &elenco);

    // Stampa le attivita trovate a schermo
    visualizza_attivita(elenco, ris);

    // Termine della funzione se non sono state trovate attivita
    if (ris == 0) {
        free(elenco);
        sleep(2);
        return;
    }

    // Fa scegliere all'utente tramite input quale attivita eliminare o se annullare l'operazione
    attivita da_modificare = scegli_attivita(elenco, ris);

    // L'utente ha annullato l'operazione
    if (da_modificare == NULL) {
        free(elenco);
        return;
    }

    pulisci_buffer();

    // Switch per selezionarea il campo dell'attivita da modificare
    switch (mod){

    // Modifca del nome del corso
    case 1:
        char nuovo_crs[MAX_NOME];
        pulisci_schermo();;
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
        pulisci_schermo();;
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
        pulisci_schermo();;
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
        pulisci_schermo();;
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
        pulisci_schermo();;
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
            pulisci_schermo();;
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
            sleep(2);
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

/*
*SPECIFICA SINTATTICA
* - void visualizza_attivita(attivita*, int)
*     Tipi in uso: array di attivita, intero
*     Descrizione: stampa a schermo le attivita presenti nell'array passato in input
*
*SPECIFICA SEMANTICA
* - visualizza\_attivita(elenco, ris)
*     Precondizioni: elenco è un array valido e inizializzato correttamente contenente almeno ris elementi, ris è >= 0 
*     Postcondizioni: nessua
*     Side effect: se ris = 1, la funzione stampa direttamente a schermo l'unica attivita presente in elenco, 
*                  altrimenti stampa tutte le attivita presenti in elenco numerandole per semplificare una sucessiva selezione
*/
void visualizza_attivita(attivita* elenco, int ris){
    pulisci_schermo();;

    // Caso in cui c'è una singola attivita nell'array
    if (ris == 1){
        printf("==================================================================================================\n");
        printf("|                              Attività trovata                                                  |\n");
        printf("==================================================================================================\n\n");
        stampa_attivita(elenco[0]);

    // Caso in cui ci sono 0 o più di una attività nell'array
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
*SPECIFICA SINTATTICA
* - attivita visualizza_attivita(attivita*, int)
*     Tipi in uso: array di attivita, attivita, intero
*     Descrizione: Permette all'utente di selezionare un'attività tra quelle presenti nella lista data in input. 
*                  Restituisce l'attività selezionata o NULL se l'operazione è annullata
*
*SPECIFICA SEMANTICA
* - scegli_attivita(elenco, ris, annullata)
*     Precondizioni: elenco è un array valido e inizializzato correttamente contenente almeno ris elementi, ris è > 0
*     Postcondizioni: restituisce l'attivita elenco[scelta - 1] se l'utente digita un intero compreso tra 1 e ris. 
*                     Restituisce NULL se l'utente digita 0
*     Side effect: stampa a schermo messaggi per guidare l'utente nell'input e messaggi di errore 
*                  se l'input è fuori dal range tra 0 e ris
*/
attivita scegli_attivita(attivita* elenco, int ris){

    printf("\n==================================================================================================\n");
    printf("|       Inserisci l'indice dell'attività da selezionare [1-%d] o 0 per annullare l'operazione     |\n", ris);
    printf("==================================================================================================\n");

    // Controllo sulla validità dell'input
    int scelta;
    while (input_intero(&scelta, 0, ris)) {
        printf("Errore: digitare un intero compreso tra 0 e %d\n", ris);
    }

    // Caso in cui l'utente ha annullato l'operazione
    if (scelta == 0){
        return NULL;
    }

    return elenco[scelta - 1];
}
