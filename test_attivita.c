#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "funzioni_attivita.h"
#include "attivita.h"

#define M 20
#define NUM_RIGHE 8

attivita file_input(char *nome_file);
void file_output(char *nome_file, attivita a);
int run_test(char *tc_id, int op, char *str);
int confronta_attivita(attivita a, attivita b);

int main(int argc, char *argv[]){
    FILE *test_suit, *result;
    char tc_id[M], str[MAX_DESC];
    int op, pass;

    if (argc != 3){
        printf("Usare: %s <test_suit> <result>\n", "test_attivita.exe");
        exit(EXIT_FAILURE);
    }

    test_suit = fopen(argv[1], "r");
    result = fopen(argv[2], "w");
    
    if ((test_suit == NULL) || (result == NULL)){
        printf("Errore apertura dei file\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(test_suit, "%s %d %s", tc_id, &op, str) == 3){
        pass = run_test(tc_id, op, str);

        fprintf(result, "%s ", tc_id);
        if (pass){
            fprintf(result, "PASS\n");
        }else{
            fprintf(result, "FAIL\n");
        } 
    }

    fclose(test_suit);
    fclose(result);

    return 0;
}

int run_test(char *tc_id, int op, char *str){
    char input_fname[M], output_fname[M], oracle_fname[M];
    int result;

    sprintf(input_fname, "%s_input.txt", tc_id);
    sprintf(output_fname, "%s_output.txt", tc_id);
    sprintf(oracle_fname, "%s_oracle.txt", tc_id);

    attivita in = file_input(input_fname);
    attivita or = file_input(oracle_fname);

    switch (op){
        case 1:
            aggiorna_nome(in, str);
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;
    
        case 2:
            aggiorna_corso(in, str);
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;
    
        case 3:
            aggiorna_descrizione(in, str);
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;

       case 4:
            aggiorna_priorita(in, atoi(str));
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;

        case 5:
            aggiorna_tempo_completato(in, atol(str));
            aggiorna_status(in);
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;

        case 6:
            aggiorna_tempo_stimato(in, atol(str));
            aggiorna_status(in);
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;
        
        case 7:
            aggiorna_scadenza(in, atol(str));
            result = confronta_attivita(in, or);
            file_output(output_fname, in);
            break;
    
        default:
            break;
    }

    free(in);
    free(or);

    return result;
}

attivita file_input(char *nome_file){
    char buffer[NUM_RIGHE][MAX_DESC];

    FILE *fd = fopen(nome_file, "r");
    if (fd == NULL) {
        printf("Errore in apertura del file %s.\n", nome_file);
    }

    int i = 0;
    while (i < NUM_RIGHE && fgets(buffer[i], MAX_DESC, fd) != NULL) {
        buffer[i][strcspn(buffer[i], "\n")] = '\0';
        i++;
    }

    fclose(fd);

    return crea_attivita(buffer[0], buffer[1], buffer[2], atoi(buffer[3]), atoi(buffer[4]), 
                        atol(buffer[5]), atol(buffer[6]), atol(buffer[7]));
}

void file_output(char *nome_file, attivita a){  
	int i;  
	FILE *fd; 	

	fd=fopen(nome_file, "w"); 		
	if( fd==NULL ){
		printf("Errore in apertura del file %s.\n", nome_file);
    }

	fprintf(fd, "%s\n", ottieni_nome(a));
    fprintf(fd, "%s\n", ottieni_corso(a));  
    fprintf(fd, "%s\n", ottieni_descrizione(a));       
    fprintf(fd, "%d\n", ottieni_priorita(a));  
    fprintf(fd, "%d\n", ottieni_status(a));  
    fprintf(fd, "%ld\n", ottieni_tempo_completato(a));  
    fprintf(fd, "%ld\n", ottieni_tempo_stimato(a));  
    fprintf(fd, "%ld\n", ottieni_scadenza(a));  

    fclose(fd); 
}

int confronta_attivita(attivita a, attivita b){
    return (!strcmp(ottieni_nome(a), ottieni_nome(b)) && 
            !strcmp(ottieni_corso(a), ottieni_corso(b)) && 
            !strcmp(ottieni_descrizione(a), ottieni_descrizione(b)) &&
            (ottieni_priorita(a) == ottieni_priorita(b)) &&
            (ottieni_status(a) == ottieni_status(b)) &&
            (ottieni_tempo_completato(a) == ottieni_tempo_completato(b)) &&
            (ottieni_tempo_stimato(a) == ottieni_tempo_stimato(b)) &&
            (ottieni_scadenza(a) == ottieni_scadenza(b)));
}