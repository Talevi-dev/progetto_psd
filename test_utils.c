#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

#define M 8
#define MAX_DESC 300

void file_input(char *nome_file, char* buffer);
void file_output(char *nome_file, long n);
int run_test(char *tc_id, int op);

int main(int argc, char *argv[]){
    FILE *test_suit, *result;
    char tc_id[M];
    int op, time, pass;

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

    while (fscanf(test_suit, "%s %d", tc_id, &op) == 2){
        pass = run_test(tc_id, op);

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

int run_test(char *tc_id, int op){
    char input_fname[M], output_fname[M], oracle_fname[M], in[MAX_DESC], or[MAX_DESC];
    int result;

    sprintf(input_fname, "%s_input.txt", tc_id);
    sprintf(output_fname, "%s_output.txt", tc_id);
    sprintf(oracle_fname, "%s_oracle.txt", tc_id);

    file_input(input_fname, in);
    file_input(oracle_fname, or);
    long or_long = atol(or);

    switch (op){
        case 1:
            int temp = controlla_input_data(in);
            file_output(output_fname, result);
            return temp == or_long;
            break;
    
        case 2:
            int temp = controllo_settimana(atol(in));
            file_output(output_fname, result);
            return (temp == or_long);
            break;
    
        case 3:
            time_t temp = converti_data(in);
            file_output(output_fname, temp);
            return temp == or_long;
            break;
    
        default:
            break;
    }
}

void file_input(char *nome_file, char *buffer){

    FILE *fd = fopen(nome_file, "r");
    if (fd == NULL) {
        printf("Errore in apertura del file %s.\n", nome_file);
    }

    if (fscanf(fd, "%s", buffer) != 1){
        printf("Errore di lettura del file %s.\n", nome_file);
        fclose(fd);
        exit(EXIT_FAILURE);
    }

    fclose(fd);
}

void file_output(char *nome_file, long n){  
	int i;  
	FILE *fd; 	

	fd=fopen(nome_file, "w"); 		
	if( fd==NULL ){
		printf("Errore in apertura del file %s.\n", nome_file);
    }

	if (fprintf(fd, "%ld\n", n) < 0){
        printf("Errore nella scrittura del file %s.\n", nome_file);
        fclose(fd);
        exit(EXIT_FAILURE);
    } 

    fclose(fd); 
}