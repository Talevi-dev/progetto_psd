#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attivita.h"
#include "funzioni_attivita.h"
#include "hash.h"
#include "utils.h"

struct node{
    attivita elemento;
    struct node* next;
};

struct hash{
    int num_elem;
    int dimensione;
    struct node** tavola;
};

int ottieni_num_elem(tabella_hash ht){
    return ht->num_elem;
}

tabella_hash nuova_hash(){
    tabella_hash ht = (tabella_hash)malloc(sizeof(struct hash));
    if (ht == NULL){
        printf("Errore allocazione tabella hash.\n");
        exit(EXIT_FAILURE);
    }
    
    ht->dimensione = 1;
    ht->num_elem = 0;

    ht->tavola = (struct node**)malloc(sizeof(struct node*) * ht->dimensione);
    if (ht->tavola == NULL){
        printf("Errore allocazione di memoria tabella_hash\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ht->dimensione; i++){
        ht->tavola[i] = NULL;
    }

    return ht;
}

int hash_fun(tabella_hash ht, char* nome){
    int idx;
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(nome); i++){

        // sum = sum + (ascii value of
        // char * (primeNumber ^ x))...
        // where x = 1, 2, 3....n
        sum = ((sum % ht->dimensione)
               + (((int)nome[i]) * factor) % ht->dimensione)
              % ht->dimensione;

        // factor = factor * prime
        // number....(prime
        // number) ^ x
        factor = ((factor % __INT16_MAX__)
                  * (31 % __INT16_MAX__))
                 % __INT16_MAX__;
    }

    idx = sum;
    return idx;
}

void inserisci_hash(tabella_hash ht, attivita elem){
    int idx = hash_fun(ht, ottieni_nome(elem));

    struct node* nuovo_nodo = (struct node*)malloc(sizeof(struct node));
    if (nuovo_nodo == NULL){
        printf("Errore allocazione nodo\n");
        exit(EXIT_FAILURE);
    }

    nuovo_nodo->elemento = elem;
    nuovo_nodo->next = NULL;

    nuovo_nodo->next = ht->tavola[idx];
    ht->tavola[idx] = nuovo_nodo;

    ht->num_elem++;
}

int cerca_hash(tabella_hash ht, char* nome, attivita** out) {
    int idx = hash_fun(ht, nome);
    struct node* curr = ht->tavola[idx];

    int dimensione = 5;
    int count = 0;

    *out = malloc(dimensione * sizeof(attivita));
    if (*out == NULL) {
        printf("Errore allocazione array di risultati.\n");
        return 0;
    }

    while (curr != NULL) {
        if (strcmp(ottieni_nome(curr->elemento), nome) == 0) {
            if (count >= dimensione) {
                dimensione *= 2;
                *out = realloc(*out, dimensione * sizeof(attivita));
                if (*out == NULL) {
                    printf("Errore realloc array di risultati.\n");
                    return count;
                }
            }
            (*out)[count++] = curr->elemento;
        }
        curr = curr->next;
    }

    return count;
}

void cancella_hash(tabella_hash ht, attivita da_eliminare){

    int idx = hash_fun(ht, ottieni_nome(da_eliminare));
    struct node* curr = ht->tavola[idx];
    struct node* prev = NULL;

    while (curr != NULL) {
        if (curr->elemento == da_eliminare) {
            if (prev == NULL) {
                ht->tavola[idx] = curr->next;
            } else {
                prev->next = curr->next;
            }   

            free(curr->elemento);  
            free(curr);
            ht->num_elem--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void distruggi_hash(tabella_hash ht){
    if (ht == NULL){
        return;
    }

    for (int i = 0; i < ht->dimensione; i++) {
        struct node* curr = ht->tavola[i];
        while (curr != NULL){
            struct node* temp = curr;
            curr = curr->next;

            free(temp->elemento);
            free(temp);
        }
    }

    free(ht->tavola);
    free(ht);
}

void stampa_hash(tabella_hash ht, int priorita){
    if (ht == NULL || ottieni_num_elem(ht) == 0) {
        printf("Erroe: Tabella hash vuota.\n");
        return;
    }

    system("clear");
    printf("==================================================================================================\n");
    if (priorita > 0) {
        printf("|                           Attivita con priorità %d:                                             |\n", priorita);
    } else {
        printf("|                             Tutte le attività:                                                  |\n");
    }
    printf("==================================================================================================\n");

    for (int i = 0; i < ht->dimensione; i++) {
        struct node* curr = ht->tavola[i];

        if (curr != NULL) {
            while (curr != NULL) {
                if (priorita > 0) {
                    if (ottieni_priorita(curr->elemento) == priorita) {
                        stampa_attivita(curr->elemento);
                    }
                } else {
                    stampa_attivita(curr->elemento);
                }
                curr = curr->next;
            }
        }
    }
}

void report_settimanale_hash(tabella_hash ht){
    if (ht == NULL || ottieni_num_elem(ht) == 0) {
        printf("Erroe: Tabella hash vuota.\n");
        return;
    }

    int in_corso = 0;
    int in_ritardo = 0;
    int completate = 0;

    time_t ora = time(&ora);
    struct tm *oggi = localtime(&ora);

    system("clear");
    printf("==================================================================================================\n");
    printf("|                                   REPORT SETTIMANALE                                           |\n");
    printf("==================================================================================================\n");

    for (int i = 0; i < ht->dimensione; i++){
        struct node* curr = ht->tavola[i];

        if (curr != NULL){
            while (curr != NULL){
                if (controllo_settimana(ottieni_scadenza(curr->elemento))){
                    stampa_attivita(curr->elemento);
                    if (ottieni_status(curr->elemento) == 0){
                        in_corso++;
                    }else if (ottieni_status(curr->elemento) == 1){
                        completate++;
                    }else{
                        in_ritardo++;
                    }    
                }
                curr = curr->next;
            }
        }
    }
    printf("==================================================================================================\n");
    printf("|  Questa settimana hai:  %d attività completate | %d attività in corso | %d attivita in ritardo    |\n", completate, in_corso, in_ritardo);
    printf("==================================================================================================\n");
}

void salva_hash(tabella_hash ht, const char *nome_file){
    FILE *fd = fopen(nome_file, "w");
    if (fd == NULL) {
        printf("Errore apertura file %s\n", nome_file);
        return;
    }

    for (int i = 0; i < ht->dimensione; i++) {
        struct node* curr = ht->tavola[i];
        while (curr != NULL) {
            fprintf(fd, "%s\n", ottieni_nome(curr->elemento));
            fprintf(fd, "%s\n", ottieni_corso(curr->elemento));  
            fprintf(fd, "%s\n", ottieni_descrizione(curr->elemento));       
            fprintf(fd, "%d\n", ottieni_priorita(curr->elemento));  
            fprintf(fd, "%d\n", ottieni_status(curr->elemento));  
            fprintf(fd, "%ld\n", ottieni_tempo_completato(curr->elemento));  
            fprintf(fd, "%ld\n", ottieni_tempo_stimato(curr->elemento));  
            fprintf(fd, "%ld\n", ottieni_scadenza(curr->elemento));  
            curr = curr->next;
        }
    }

    fclose(fd);
}

void carica_hash(tabella_hash ht, const char *nome_file){
    FILE *fd = fopen(nome_file, "r");
    if (fd == NULL) {
        printf("Errore apertura file %s\n", nome_file);
        return;
    }

    char buffer[8][MAX_DESC];

    while (1) {
        int i = 0;
        for (; i < 8; i++) {
            if (fgets(buffer[i], MAX_DESC, fd) == NULL) {
                break;
            }
            buffer[i][strcspn(buffer[i], "\n")] = '\0';
        }

        if (i < 8) {
            break; 
        }

        attivita a = crea_attivita(
            buffer[0], buffer[1], buffer[2],
            atoi(buffer[3]), atoi(buffer[4]),
            atol(buffer[5]), atol(buffer[6]), atol(buffer[7])
        );

        aggiorna_status(a);
        inserisci_hash(ht, a);
    }

    fclose(fd);
}