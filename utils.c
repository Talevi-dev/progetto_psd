#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

// Funzione per pulire il buffer ed evitare letture indesiderate
void pulisci_buffer(){
    while (getchar() != '\n');
}

/* 
* Funzione che legge una stringa in input e controlla che non sia troppo lunga
* Restituisce 0 se l'input è corretto e 1 se la stringa è troppo lunga
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
* Funzione che legge un intero in input e controlla che sia compreso tra x e y
* Restituisce 0 se l'input è corretto e 1 se l'input è fuori dal range
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
* Funzione che controlla che la stringa in input sia nel formato corretto 
* dd/mm/yyyy hh:mm
* Restituisce 1 se il formato è corretto e 0 altrimenti
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
* Funzione che verifica se una data cade nella settimana corrente
* Restituisce 1 se è cosi e zero altrimenti
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
* Funzioine che converte un stringa nel formato dd/mm/yyyy hh:mm in un time_t
* Restituisce il valore converitito
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