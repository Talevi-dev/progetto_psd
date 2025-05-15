#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "funzioni_attivita.h"
#include "attivita.h"
#include "hash.h"

#define M 20
#define NUM_RIGHE 8

int run_test(char *tc_id, int op, char *str);
int confronta_attivita(attivita a, attivita b);

int main(int argc, char *argv[]){
    FILE *test_suit, *result;
    char tc_id[M], str[MAX_DESC];
    int op, pass;

    if (argc != 3){
        printf("Usare: %s <test_suit> <result>\n", "test_hash.exe");
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

    tabella_hash in = nuova_hash();
    carica_hash(in, input_fname);

    switch (op){
        case 1:
            attivita a = crea_attivita(str, "", "", 1, 0, 0, 360000, 360000);
            inserisci_hash(in, a);
            salva_hash(in, output_fname);
            result = confronta_file(input_fname, oracle_fname);
            free(a);
            break;
    
        case 2:
            attivita a = crea_attivita(str, "", "", 1, 0, 0, 360000, 360000);
            cancella_hash(in, a);
            salva_hash(in, output_fname);
            result = confronta_file(input_fname, oracle_fname);
            free(a);
            break;
    
        case 3:
            attivita a = crea_attivita(str, "", "", 1, 0, 0, 360000, 360000);
            inserisci_hash(in, a);
            cancella_hash(in, a);
            salva_hash(in, output_fname);
            result = confronta_file(input_fname, oracle_fname);
            free(a);
            break;
    
        default:
            break;
    }

    distruggi_hash(in);

    return result;
}

int confronta_file(const char *file1, const char *file2) {
    FILE *fd1 = fopen(file1, "r");
    FILE *fd2 = fopen(file2, "r");

    if (fd1 == NULL || fd2 == NULL){
        printf("Errore apertura file.\n");
        if (fd1) fclose(fd1);
        if (fd2) fclose(fd2);
        return 0;
    }

    int c1, c2;

    do{
        c1 = fgetc(fd1);
        c2 = fgetc(fd2);

        if (c1 != c2) {
            fclose(fd1);
            fclose(fd2);
            return 0;
        }

    } while (c1 != EOF && c2 != EOF);

    int uguali = (c1 == EOF && c2 == EOF);

    fclose(fd1);
    fclose(fd2);
    return uguali;
}
