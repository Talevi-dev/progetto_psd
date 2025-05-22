#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

/*
* SPECIFICA SINTATTICA
* - void pulisci_buffer(void)
*     Tipi in uso: void
*     Descrizione: svuota il buffer di input per evitare letture indesiderate
*
* SPECIFICA SEMANTICA
* - pulisci_buffer()
*     Precondizione: nessuna
*     Postcondizione: nessuna
*     Side effect: legge e scarta tutti i caratteri nel buffer di input fino a \n
*/
void pulisci_buffer(){
    while (getchar() != '\n');
}

/*
* SPECIFICA SINTATTICA
* - void pulisci_schermo(void)
*     Tipi in uso: void
*     Descrizione: pulisce il contenuto del terminale, adattandosi al sistema operativo in uso
*
* SPECIFICA SEMANTICA
* - pulisci_schermo()
*     Precondizione: sistema operativo Windows, Linux o macOS
*     Postcondizione: nessuna
*     Side effect: pulisce la schermata del terminale
*/
void pulisci_schermo(){
    // Sistemi operativi Windows
    #ifdef _WIN32
        system("cls");

    // Sistemi operativi Linux/macOS
    #else
        system("clear");
    #endif
}

/*
* SPECIFICA SINTATTICA
* - int input_stringa(char *, int)
*     Tipi in uso: stringa, intero
*     Descrizione: legge una stringa da input e controlla che sia della dimensione corretta. 
*                  Restituisce 0 se l'input è corretto e 1 se la stringa è troppo lunga
*
* SPECIFICA SEMANTICA
* - input_stringa(str, len)
*     Precondizione: il puntatore str deve essere valido e puntare ad un buffer di almeno len caratteri,
*                    len deve essere maggiore di 1
*     Postcondizione: la funzione restituisce 0 se la stringa digitata in input è < len - 1  e 
*                     1 se la stringa è di dimensioni > len - 1
*     Side effect: riempie il buffer str con la stringa digitata in input
*/  
int input_stringa(char *str, int len){

    // Pulisce la stringa passata come buffer
    memset(str, 0, len);

    // Lettura dell'input
    if (fgets(str, len, stdin)){
        long l = strlen(str);

        // Rimozione del carattere nuova linea se presente nel buffer
        if (l > 0 && str[l - 1] == '\n'){
            str[l - 1] = '\0';
        
        // Se non presente, la stringa inserita è troppo lunga
        }else if(l == len - 1){
            pulisci_buffer();
            return 1;
        }

        return 0;
    }

    // Azzera la stringa se avviene un errore in lettura della fgets
    str[0] = '\0';
    return 1;
}

/*
* SPECIFICA SINTATTICA
* - int input_intero(int *, int, int)
*     Tipi in uso: intero
*     Descrizione: legge un intero in input e controlla che sia compreso in un certo range. 
*                  Restituisce 0 se l'input è corretto e 1 se l'input è fuori dal range
*
* SPECIFICA SEMANTICA
* - input_intero(input, x, y)
*          Precondizione: il puntatore input deve essere valido, x <= y
*          Postcondizione: la funzione restituisce 0 se l'intero digitato in input è x <= input <= y, 
*                          e 1 in caso contrario
*          Side effect: salva l'intero digitato nella variabile puntata dal puntatore input
*/  
int input_intero(int *input, int x, int y){

    int r = scanf("%d", input);
    if (*input <= y && *input >=  x && r == 1){
        return 0;
    } 

    pulisci_buffer();
    return 1;
}

/*
* SPECIFICA SINTATTICA
* - int controlla_input_data(char *)
*     Tipi in uso: stringa, intero
*     Descrizione: controlla che la stringa in input sia nel formato corretto "dd/mm/yyyy hh:mm".
*                  Restituisce 1 se il formato è corretto e 0 altrimenti
*
* SPECIFICA SEMANTICA
* - controlla_input_data(data)
*          Precondizione: data deve puntare ad una stringa valida e nel formato "gg/mm/aaaa hh:mm"
*          Postcondizione: la funzione restituisce 1 se il formato è corretto e la stringa rappresenta una data reale, 
*                          e 0 se il formato non è corretto o la stringa non rappresenta una data reale.
*          Side effect: in caso di formato non corretto stampa a schermo un messaggio di errore
*/  
int controlla_input_data(char *data){
    int giorno, mese, anno, ora, minuti;

    // Controllo sulla lunghezza della stringa
    if ((data == NULL) || (strlen(data) != MAX_DATA - 1) ){
        printf("Errore: stringa di dimensione errata\n");
        return 0;
    }

    // Controllo sul formato e lettura della stringa
    if (sscanf(data, "%2d/%2d/%4d %2d:%2d", &giorno, &mese, &anno, &ora, &minuti) != 5){
        printf("Errore: stringa in un formato non corretto\n");
        return 0;
    }

    // Controllo della validità del giorno
    if (giorno < 1 || giorno > 31){
        printf("Errore: giorno non valido\n");
        return 0;
    }

    // Controllo della validità del mese
    if (mese < 1 || mese > 12){
        printf("Erroe: mese non valido\n");
        return 0;
    }

    // Controllo della validità dell'anno 
    // Il limite superiore è il 2038 perchè è il limite del tipo time_t su sitemi a 32 bit 
    if (anno < 1970 || anno > 2038){
        printf ("Errore: anno non valido\n");
        return 0;
    }

    if (mese == 2){

        // Calcolo se l'anno è bisestile
        int bisestile = (anno % 4 == 0 && (anno % 100 != 0 || anno % 400 == 0));

        // Controllo validità del giorno per febbraio
        if ((bisestile && (giorno > 29)) || (!bisestile && (giorno > 28))){
            printf ("Errore: giorno non valido per febbraio\n");
            return 0;
        }

    // Controllo validità del giorno per mesi con 30 giorni
    }else if (mese == 4 || mese == 6 || mese == 9 || mese == 11){

        if (giorno > 30){
            printf ("Errore: giorno non valido\n");
            return 0;
        }
    }
    
    // Controllo validità orario inserito
    if (ora < 0 || ora > 23 || minuti < 0 || minuti > 59){
        printf("Errore: orario non valido\n");
        return 0;
    }

    return 1;
}

/*
* SPECIFICA SINTATTICA
* - int controllo_settimana(time_t)
*     Tipi in uso: intero, time_t
*     Descrizione: verifica se una data cade nella settimana corrente.
*                  Restituisce 1 se è cosi e 0 altrimenti
*
* SPECIFICA SEMANTICA
* - controllo\_settimana(scadenza)
*     Precondizione: scadenza deve essere un valore valido per il tipo time_t,
*                    l'orologio della macchina deve essere impostato correttamente
*     Postcondizione: la funzione restituisce 1 se la scadenza è nella settimana corrente, e 0 viceversa.
*     Side effect: nessun side effect
*/  
int controllo_settimana(time_t scadenza){

    // Calcolo di data e orario correnti
    time_t ora = time(NULL);

    // Converisione da time_t a struct tm per avere accesso al giorno corrente della settimana 
    struct tm oggi = *localtime(&ora);
    int giorno_corrente = oggi.tm_wday;

    // Calcola i giorni da sottrarre ad oggi per arrivare a lunedi
    // 0 rappresenta la domenica, da 1 a 6 i restanti giorni della settimana
    int giorni_da_togliere = (giorno_corrente == 0) ? 6 : (giorno_corrente - 1);

    // Setto la stuct tm oggi alla mezzanotte del lunedi di questa settimana
    oggi.tm_mday -= giorni_da_togliere;
    oggi.tm_hour = 0;
    oggi.tm_min = 0;
    oggi.tm_sec = 0;

    // Converisione in time_t
    time_t lunedi = mktime(&oggi); 

    // Calcolo della domenica della settimana corrente in time_t
    time_t domenica = lunedi + (7 * 24 * 3600); 

    return scadenza >= lunedi && scadenza < domenica;
}

/*
* SPECIFICA SINTATTICA
* - time_t converti_data(char *)
*     Tipi in uso: intero, time_t
*     Descrizione: converte un stringa nel formato "dd/mm/yyyy hh:mm" in un tipo time\_t.
*                  Restituisce il valore converitito
*
* SPECIFICA SEMANTICA
* - converti_data(data)
*     Precondizione: data deve puntare ad una stringa valida e nel formato "gg/mm/aaaa hh:mm"
*     Postcondizione: la funzione restituisce un valore time_t che rappresenta la data fornita
*     Side effect: nessun side effect
*/  
time_t converti_data (char *data){

    // Inizializzo un struct tm vuota per poterla sovrascrivere con la stringa in input
    struct tm tm_data = {0};
    sscanf(data, "%d/%d/%d %d:%d", &tm_data.tm_mday, &tm_data.tm_mon, &tm_data.tm_year, &tm_data.tm_hour, &tm_data.tm_min);
    
    // Sottraggo 1900 perche tm.year è espresso come anni dal 1900
    tm_data.tm_year -= 1900; 

    // Sottraggo 1 perche tm.mon va da 0 a 11
    tm_data.tm_mon -= 1;      
    
    return mktime(&tm_data);
}