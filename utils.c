#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

int input_stringa(char *str, int len) {
    memset(str, 0, len);

    if (fgets(str, len, stdin)) {
        long l = strlen(str);

        if (l > 0 && str[l - 1] == '\n') {
            str[l - 1] = '\0';
        } else if (l == len - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return 1;
        }

        return 0;
    }

    printf("Errore nell'inserimento della stringa.\n");
    str[0] = '\0';
    return 1;
}

int input_intero(int *input, int x, int y){
    int r = scanf("%d", input);
    if (*input <= y && *input >=  x && r == 1){
        return 0;
    } 
    pulisci_buffer();
    return 1;
}

int controlla_input_data(char *data){
    int giorno, mese, anno, ora, minuti;

    if ((data == NULL) || (strlen(data) != MAX_DATA - 1) ){
        printf("Errore: stringa di dimensione errata\n");
        return 0;
    }

    if (sscanf(data, "%2d/%2d/%4d %2d:%2d", &giorno, &mese, &anno, &ora, &minuti) != 5){
        printf("Errore: stringa in un formato non corretto\n");
        return 0;
    }

    if (giorno < 1 || giorno > 31){
        printf("Errore: giorno non valido\n");
        return 0;
    }

    if (mese < 1 || mese > 12){
        printf("Erroe: mese non valido\n");
        return 0;
    }

    if (mese == 2){
        int bisestile = (anno % 4 == 0 && (anno % 100 != 0 || anno % 400 == 0));
        if ((bisestile && (giorno > 29)) || (!bisestile && (giorno > 28))){
            printf ("Errore: giorno non valido per febbraio\n");
            return 0;
        }
    }else if (mese == 4 || mese == 6 || mese == 9 || mese == 11){
        if (giorno > 30){
            printf ("Errore: giorno non valido\n");
            return 0;
        }
    }

    if (anno < 1970 || anno > 2038){
        printf ("Errore: anno non valido\n");
        return 0;
    }
    

    if (ora < 0 || ora > 23 || minuti < 0 || minuti > 59){
        printf("Errore: orario non valido\n");
        return 0;
    }

    return 1;
}

time_t converti_data (char *data){
    struct tm tm_data = {0};
    sscanf(data, "%d/%d/%d %d:%d", &tm_data.tm_mday, &tm_data.tm_mon, &tm_data.tm_year, &tm_data.tm_hour, &tm_data.tm_min);
    
    tm_data.tm_year -= 1900; 
    tm_data.tm_mon -= 1;      
    
    time_t data_t = mktime(&tm_data);

    return data_t;
}

void pulisci_buffer(){
    while (getchar() != '\n');
}
