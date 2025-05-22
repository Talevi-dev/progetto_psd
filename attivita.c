#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "utils.h"
#include "attivita.h"


// Definizione della struttura dati che rappresenta un attività
struct act{
    char nome[MAX_NOME];            // Nome dell'attivita (stringa massimo 50 caratteri)
    char corso[MAX_NOME];           // Nome del corso (stringa massimo 50 caratteri)
    char descrizione[MAX_DESC];     // Descrizine dell'attività (stringa massimo 300 caratteri)
    int priorita;                   // Priorità, assume valori: 1 (Bassa), 2 (Media), 3 (Alta)
    int status;                     // Status, assume valori: 0 (In corso), 1 (Completata), 2(In ritardo)
    time_t tempo_completato;        // Tempo, in secondi, dello studio già completato sull'attività
    time_t tempo_stimato;           // Tempo, in secondi, stimato per portare a termine l'attività
    time_t scadenza;                // Data di scadenza: in secodi trascorsi dal 1° gennaio 1970
};

/*
* SPECIFICA SINTATTICA
* - attivita crea_attivita(char*, char*, char*, int, int, time\_t, time\_t, time\_t)
*     Tipi in uso: attivita, interi, stringhe, time_t 
*     Descrizione: crea una nuova attività a partire dai parametri forniti in input. 
*                  Restituisce un puntatore alla nuova struttura allocata dinamicamente
*
* SPECIFICA SEMANTICA
* - crea_attivita(nm, cs, dsc, prt, sts, cmp, stm, scd)
*     Precondizione: i puntatori delle stinghe nm, cs e dsc devono puntare ad aree di memoria valide, 
*                    i valori prt, sts, cmp e scd devono essere valori validi per un'attivita 
*     Postcondizione: inizializza e restituisce "nuova", un' attivita allocata dinamicamente e 
*                     creata con i valori passati in input o NULL se l'allocazione dinamica della memoria fallisce
*     Side effect: stampa a schermo un messaggio di errore se l'allocazione dinamica della memoria fallisce
*/
attivita crea_attivita(char *nm, char *cs, char *dsc, int prt, int sts, time_t cmp, time_t stm, time_t scd){
    attivita nuova = (attivita)malloc(sizeof(struct act));
    if (nuova == NULL){
        printf("Errore: impossibile allocare memoria per la nuova attività.\n");
        exit(EXIT_FAILURE);
    }
    
    //Utilizzo di strncpy per assicurarsi una copia corretta delle stringhe negli appositi campi
    strncpy(nuova->nome, nm, MAX_NOME - 1);
    nuova->nome[MAX_NOME - 1] = '\0';

    strncpy(nuova->corso, cs, MAX_NOME - 1);
    nuova->corso[MAX_NOME - 1] = '\0';

    strncpy(nuova->descrizione, dsc, MAX_DESC - 1);
    nuova->descrizione[MAX_DESC - 1] = '\0';

    nuova->priorita = prt;
    nuova->status = sts;
    nuova->tempo_completato = cmp;
    nuova->tempo_stimato = stm;
    nuova->scadenza = scd;

    return nuova;
}

/*
* SPECIFICA SINTATTICA
* - attivita input_attivita()
*     Tipi in uso: attività 
*     Descrizione: prende da tastiera i dati necessari per la creazione di un attività, controllandone la validità.
*                  Restituisce un puntatore all'attività creata
*
* SPECIFICA SEMANTICA
* - input_attivita(nm, cs, dsc, prt, sts, cmp, stm, scd)
*     Precondizione: nessuna  
*     Postcondizione: restituisce un puntatore ad una nuova attivita allocata dinamicamente e 
*                     creata con i valori passati in input tramite la funzione crea_attivita
*     Side effect: stampa a schremo messaggi per aiutare l'utente nell'input dei parametri e 
*                  messaggi di errore nel caso in cui l'input non sia valido
*/
attivita input_attivita(){
    char nome[MAX_NOME], corso[MAX_NOME], descrizione[MAX_DESC], data[MAX_DATA];
    int priorita, ore;

    // Time_t indica il tempo trascorso in secondi dal 1 gennaio 1970 
    time_t stima = 0;
    time_t scadenza;

    printf("==================================================================================================\n");
    printf("|                            Inserire il nome dell' attività                                     |\n");
    printf("==================================================================================================\n");

    // Input del nome e controllo sulla strigna inserita
    while (input_stringa(nome, MAX_NOME)){
        printf("Attenzione, il nome dell' attività è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    pulisci_schermo();;
    printf("==================================================================================================\n");
    printf("|                               Inserire il nome del corso                                       |\n");
    printf("==================================================================================================\n");

    // Input del corso e controllo sulla strigna inserita
    while (input_stringa(corso, MAX_NOME)){
        printf("Attenzione, il nome del corso è troppo lungo, riprovare.(max 50 caratteri)\n");
    }

    pulisci_schermo();;
    printf("==================================================================================================\n");
    printf("|                           Inserire la descrizione dell' attività                               |\n");
    printf("==================================================================================================\n");

    // Input della descrizione e controllo sulla strigna inserita
    while (input_stringa(descrizione, MAX_DESC)){
        printf("Attenzione, la descrizione è troppo lunga, riprovare.(max 300 caratteri)\n");
    }
    
    pulisci_schermo();;
    printf("==================================================================================================\n");
    printf("|                Inserire la priorità:  | Bassa [1] | Media [2] | Alta [3] |                     |\n");
    printf("==================================================================================================\n");

    // Input della priorita e controllo sull'intero inserito
    while (input_intero(&priorita, 1, 3)){
        printf("Errore: digitare un intero compreso tra 1 e 3\n");
    }

    pulisci_schermo();;
    printf("==================================================================================================\n");
    printf("|                    Inserire il tempo stimato per completare l'attività in ore                  |\n");
    printf("==================================================================================================\n");

    // Input delle ore stimate e controllo sull'intero inserito
    while (input_intero(&ore, 1, INT_MAX)){
        printf("Errore: inserire un numero di ore maggiore di 0.\n");
    }

    // Conversione da ore a secondi per rendere il dato congruo al tipo time_t 
    stima = ore * 3600;

    pulisci_buffer();
    pulisci_schermo();;
    printf("==================================================================================================\n");
    printf("|             Inserire la data di scadenza, utilizzare il formato dd/mm/yyyy hh:mm               |\n");
    printf("==================================================================================================\n");

    while (1){

        input_stringa(data, MAX_DATA);

        // Controllo della correttezza del formato della data
        if (controlla_input_data(data)){

            // Coversione della data inserita da stringa a time_t
            scadenza = converti_data(data);

            // Calcolo di data e orario correnti
            time_t ora_attuale = time(NULL);

            // Confronto tra scadenza e ora corrente per verificare che la data non sia già passata
            if (scadenza > ora_attuale){
                break;
            }
            printf("Errore: la data inserita è gia passata.\n");
        } 
    }

    pulisci_schermo();;
    
    // Creazione e restituzione dell'attivitaìà
    return crea_attivita(nome, corso, descrizione, priorita, 0, 0, stima, scadenza); 
}

/*
* SPECIFICA SINTATTICA
* - char* ottieni_nome(attivita)
*     Tipi in uso: attivita, stringa
*     Descrizione: restituisce una stringa contenente il nome dell'attività data in input 
*
* SPECIFICA SEMANTICA
* - ottieni_nome(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un puntatore alla stringa contente il nome dell'attivita a in input o 
*                     NULL se l'attivita non è correttamente inizializzata
*     Side effect: nessuno
*/
char *ottieni_nome(attivita a){
    if (a == NULL){
        return NULL;
    }
    return a -> nome;
}

/*
* SPECIFICA SINTATTICA
* - char* ottieni_corso(attivita)
*     Tipi in uso: attivita, stringa
*     Descrizione: restituisce una stringa contenente il corso dell'attività data in input
*
* SPECIFICA SEMANTICA
* - ottieni_corso(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un puntatore alla stringa contente il corso dell'attivita a in input o 
*                     NULL se l'attivita non è correttamente inizializzata
*     Side effect: nessuno
*/
char *ottieni_corso(attivita a){
    if (a == NULL){
        return NULL;
    }
    return a -> corso;
}

/*
* SPECIFICA SINTATTICA
* - char* ottieni_descrizione(attivita)
*     Tipi in uso: attivita, stringa
*     Descrizione: restituisce una stringa contenente la descrizione dell'attività data in input
*
* SPECIFICA SEMANTICA
* - ottieni_descrizione(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un puntatore alla stringa contente la descrizione dell'attivita a in input o 
*                     NULL se l'attivita non è correttamente inizializzata
*     Side effect: nessuno
*/
char *ottieni_descrizione(attivita a){
    if (a == NULL){
        return NULL;
    }
    return a -> descrizione;
}

/*
* SPECIFICA SINTATTICA
* - int ottieni_priorità(attivita)
*     Tipi in uso: attivita, intero
*     Descrizione: restituisce la priorità dell'attività data in input
*
* SPECIFICA SEMANTICA
* - ottieni_priorita(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un valore intero che rappresenta la priorità dell'attivita a o 
*                     -1 se l'attivita non è corettamente inizializzata
*     Side effect: nessuno
*/
int ottieni_priorita(attivita a){
    if (a == NULL){
        return -1;
    }
    return a -> priorita;
}

/*
* SPECIFICA SINTATTICA
* - int ottieni_status(attivita)
*     Tipi in uso: attivita, intero
*     Descrizione: restituisce lo status dell'attività data in input
*
* SPECIFICA SEMANTICA
* - ottieni_status(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un valore intero che rappresenta lo status dell'attivita a o 
*                     -1 se l'attivita non è corettamente inizializzata
*     Side effect: nessuno
*/
int ottieni_status(attivita a){
    if (a == NULL){
        return -1;
    }
    return a -> status;
}

/*
* SPECIFICA SINTATTICA
* - time_t ottieni_tempo_completato(attivita)
*     Tipi in uso: attivita, time_t
*     Descrizione: restituisce il tempo completato dell'attivita data in input
*
* SPECIFICA SEMANTICA
* - ottieni_tempo_completato(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un valore time_t che rappresenta il tempo di studio eseguito per l'attivita a 
*                     espressa in secondi dal 1° gennaio 1970 o -1 se l'attivita non è corettamente inizializzata
*     Side effect: nessuno
*/
time_t ottieni_tempo_completato(attivita a){
    if (a == NULL){
        return -1;
    }
    return a -> tempo_completato;
}

/*
* SPECIFICA SINTATTICA
* - time_t ottieni_tempo_stimato(attivita)
*     Tipi in uso: attivita, time_t
*     Descrizione: restituisce il tempo stimato dell'attivita data in input
*
* SPECIFICA SEMANTICA
* - ottieni_tempo_stimato(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un valore time_t che rappresenta il tempo stimato per completare l'attivita a 
*                     espresso in secondi dal 1° gennaio 1970 o -1 se l'attivita non è corettamente inizializzata
*     Side effect: nessuno
*/
time_t ottieni_tempo_stimato(attivita a){
    if (a == NULL){
        return -1;
    }
    return a -> tempo_stimato;
}

/*
* SPECIFICA SINTATTICA
* - time_t ottieni_scadenza(attivita)
*     Tipi in uso: attivita, time_t
*     Descrizione: restituisce la scadenza dell'attivita data in input
*
* SPECIFICA SEMANTICA
* - ottieni_scadenza(a)
*     Precondizione: il puntatore a deve puntare ad una struttura correttamente allocata in memoria ed inizializzata
*     Postcondizione: restituisce un valore time_t che rappresenta la scadenza dell'attivita a 
*                     espressa in secondi dal 1° gennaio 1970 o -1 se l'attivita non è corettamente inizializzata
*     Side effect: nessuno
*/
time_t ottieni_scadenza(attivita a){
    if (a == NULL){
        return -1;
    }
    return a -> scadenza;
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_corso(attivita, char*)
*     Tipi in uso: attivita, stringa
*     Descrizione: modifica il nome del corso dell' attività in input sostituendolo con la stringa in input
*
* SPECIFICA SEMANTICA
* - aggiorna_corso(a, nuovo)
*     Precondizione: i puntatori a e nuovo devono puntare ad aree di memoria correttamente allcate ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo corso dell'attivita a sostituendolo con la stringa "nuovo" se 
*                  sono valide le precondizioni, non fa niente in caso contrario
*/
void aggiorna_corso(attivita a, char *nuovo){
    if (a != NULL && nuovo != NULL){
        strncpy(a->corso, nuovo, MAX_NOME - 1);
        a->corso[MAX_NOME - 1] = '\0';
    }
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_descrizione(attivita, char*)
*     Tipi in uso: attivita, stringa
*     Descrizione: modifica la descrizione dell' attività in input sostituendola con la stringa in input
*
* SPECIFICA SEMANTICA
* - aggiorna_descrizione(a, nuovo)
*     Precondizione: i puntatori a e nuovo devono puntare ad aree di memoria correttamente allcate ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo descrizione dell'attivita a sostituendolo con la stringa "nuovo" se 
*                  sono valide le precondizioni, non fa niente in caso contrario
*/
void aggiorna_descrizione(attivita a, char *nuovo){
    if (a != NULL && nuovo != NULL){
        strncpy(a->descrizione, nuovo, MAX_DESC - 1);
        a->descrizione[MAX_DESC - 1] = '\0';
    }
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_priorità(attivita, int)
*     Tipi in uso: attivita, intero
*     Descrizione: modifica la priorità dell' attività in input sostituendola con l'intero in input
*
* SPECIFICA SEMANTICA
* - aggiorna_priorita(a, p)
*     Precondizione: il puntatore a deve puntare ad un area di memoria correttamente allcata ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo priorità dell'attivita a sostituendolo con l'intero p se 
*                  sono valide le precondizioni, non fa niente in caso contrario
*/
void aggiorna_priorita(attivita a, int p){
    if (a != NULL){
        a -> priorita = p;
    }
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_status(attivita)
*     Tipi in uso: attivita
*     Descrizione: aggiorna automaticamente lo status di un'attività data in input in base alla scadenza, 
*                  al tempo stimato e al tempo completato
*
* SPECIFICA SEMANTICA
* - aggiorna_status(a)
*     Precondizione: il puntatore a deve puntare ad un area di memoria correttamente allcata ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo status dell'attivita a ponendolo a 1 se tempo_completato >= tempo_stimato, 
*                  a 2 se tempo_completato <= tempo_stimato e la scadenza è passata e a 0 altrimenti. 
*                  Se non sono valide le precondizioni, non fa niente
*/
void aggiorna_status(attivita a){
    if (a != NULL){
        // Calcolo di data e orario correnti
        time_t ora_attuale = time(NULL);
    
        // Controllo se l'attivita è stata portat a tremine
        if ((a -> tempo_completato) >= (a -> tempo_stimato)){
            a -> status = 1;

        // Controllo se l'attività è in ritardo
        }else if(((a -> scadenza) < ora_attuale)){
            a -> status = 2;

        // L'attivita è in corso
        }else{
            a -> status = 0;
        }
    }
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_tempo_completato(attivita, int o)
*     Tipi in uso: attivita, int
*     Descrizione: modifica il tempo completato dell' attività in input sostituendola 
*                  con l'intero in input moltiplicato per 3600 (converisione da ore in secondi)
*
* SPECIFICA SEMANTICA
* - aggiorna_tempo_completato(a, o)
*     Precondizione: il puntatore a deve puntare ad un area di memoria correttamente allcata ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo tempo_completato dell'attivita a sostituendolo con l'intero o (moltiplicato per 3600)
*                      se sono valide le precondizioni, non fa niente in caso contrario
*/
void aggiorna_tempo_completato(attivita a, int o){
    if (a != NULL){
        a -> tempo_completato += (o * 3600);
    }   
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_tempo_stimato(attivita, int o)
*     Tipi in uso: attivita, int
*     Descrizione: modifica il tempo completato dell' attivita in input sostituendola 
*                  con l'intero in input moltiplicato per 3600 (converisione da ore in secondi)
*
* SPECIFICA SEMANTICA
* - aggiorna_tempo_stimato(a, o)
*     Precondizione: il puntatore a deve puntare ad un area di memoria correttamente allcata ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo tempo_stimato dell'attivita a sostituendolo con l'intero o (moltiplicato per 3600) 
*                  se sono valide le precondizioni, non fa niente in caso contrario
*/
void aggiorna_tempo_stimato(attivita a, int o){
    if (a != NULL){
        (a -> tempo_stimato) = (o * 3600);
    }
}

/*
* SPECIFICA SINTATTICA
* - void aggiorna_scadenza(attivita, time_t)
*     Tipi in uso: attivita, time_t
*     Descrizione: modifica la scadenza dell' attività in input sostituendola con il valore time_t in input
*
* SPECIFICA SEMANTICA
* - aggiorna_scadenza(a, nuova)
*     Precondizione: il puntatore a deve puntare ad un area di memoria correttamente allcata ed inizializzata
*     Postcondizione: nessuna
*     Side effect: modifica il campo scadenza dell'attivita a sostituendolo con il valore time_t "nuova" 
*                  se sono valide le precondizioni, non fa niente in caso contrario
*/
void aggiorna_scadenza(attivita a, time_t nuova){
    if (a != NULL){
        a -> scadenza = nuova;
    }
}