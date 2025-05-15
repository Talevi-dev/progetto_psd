#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

tabella_hash nuova_hash(){
    tabella_hash ht = (tabella_hash)malloc(sizeof(struct hash));
    if (ht == NULL) {
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
    
    for (int i = 0; i < ht->dimensione; i++) {
        ht->tavola[i] = NULL;
    }

    return ht;
}


int hash_fun(tabella_hash ht, char* key){
    int idx;
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++) {

        // sum = sum + (ascii value of
        // char * (primeNumber ^ x))...
        // where x = 1, 2, 3....n
        sum = ((sum % ht->dimensione)
               + (((int)key[i]) * factor) % ht->dimensione)
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

void insert(tabella_hash ht, attivita elem)
{
    int idx = hash_fun(ht, ottieni_nome(elem));
    struct node* curr = ht->tavola[idx];

    while (curr != NULL) {
        if (strcmp(ottieni_nome(curr->elemento), ottieni_nome(elem))){
            return;
        }
        curr = curr->next;
    }

    struct node* nuovo_nodo = (struct node*)malloc(sizeof(struct node));
    if (nuovo_nodo == NULL){
        printf("Errore allocazione di memoria per nodo hash\n");
        exit(EXIT_FAILURE);
    }
    
    nuovo_nodo->elemento = elem;
    nuovo_nodo->next = NULL;
     

    if (ht->tavola[idx] == NULL) {
        ht->tavola[idx] = nuovo_nodo;
    } else {
        nuovo_nodo->next = ht->tavola[idx];
        ht->tavola[idx] = nuovo_nodo;
    }
}

void cancella(tabella_hash ht, char* key){

    int idx = hash_fun(ht, key);

    struct node* prev = NULL;
    struct node* curr = ht->tavola[idx];

    while (curr != NULL) {
        if (strcmp(ottieni_nome(curr->elemento), key) == 0){

            if (curr == ht->tavola[idx]) {
                ht->tavola[idx] = curr->next;
            }

            else {
                prev->next = curr->next;
            }
            free(curr->elemento);
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    return;
}

attivita cerca(tabella_hash ht, char* key){
    int idx = hash_fun(ht, key);
    struct node* curr = ht->tavola[idx];

    while (curr != NULL) {
        if (strcmp(ottieni_nome(curr->elemento), key) == 0){
            return curr->elemento;
        }
        curr = curr->next;
    }

    return NULL;
}

void distruggi_hash(tabella_hash ht) {
    if (ht == NULL) return;

    for (int i = 0; i < ht->dimensione; i++) {
        struct node* curr = ht->tavola[i];
        while (curr != NULL) {
            struct node* temp = curr;
            curr = curr->next;

            free(temp->elemento);
            free(temp);
        }
    }

    free(ht->tavola);

    free(ht);
}

void stampa_hash(tabella_hash ht, int p){
    if (ht == NULL) {
        printf("Tabella hash non inizializzata.\n");
        return;
    }

    if (p >= 0) {
        printf("Contenuto della tabella hash con priorità %d:\n", p);
    } else {
        printf("Contenuto della tabella hash:\n");
    }

    for (int i = 0; i < ht->dimensione; i++) {
        struct node* curr = ht->tavola[i];

        if (curr != NULL) {
            while (curr != NULL) {
                if (p > 0) {
                    if (ottieni_priorita(curr->elemento) == p) {
                        stampa_attivita2(curr->elemento);
                    }
                } else {
                    stampa_attivita2(curr->elemento);
                }
                curr = curr->next;
            }
        }
    }
}