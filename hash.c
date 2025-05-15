#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attivita.h"
#include "funzioni_attivita.h"
#include "hash.h"

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
    
    ht->dimensione = 100;
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

int cerca(tabella_hash ht, char* nome, attivita** out) {
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

void cancella(tabella_hash ht, attivita da_eliminare){

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

    if (priorita > 0) {
        printf("Contenuto della tabella hash con priorità %d:\n", priorita);
    } else {
        printf("Contenuto della tabella hash:\n");
    }

    for (int i = 0; i < ht->dimensione; i++) {
        struct node* currNode = ht->tavola[i];

        if (currNode != NULL) {
            while (currNode != NULL) {
                if (priorita > 0) {
                    if (ottieni_priorita(currNode->elemento) == priorita) {
                        stampa_attivita2(currNode->elemento);
                    }
                } else {
                    stampa_attivita2(currNode->elemento);
                }
                currNode = currNode->next;
            }
        }
    }
}