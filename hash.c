#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "attivita.h"
#include "hash.h"
#include "funzioni_attivita.h"

// Definizione del nodo della lista collegata per gestire le collisioni nella tabella hash
struct node{
    attivita elemento;          // Puntatore all struttura che rappresenta l'attività
    struct node* next;          // Puntatore al nodo sucessivo della lista
};

// Definizione della struttura della tabella hash
struct hash{
    int num_elem;             // Numero di elementi presenti nella tabella 
    int dimensione;           // Dimensione della tabella hash
    struct node** tavola;     // Array di puntatori a nodi
};

// Funzione che crea una nuova tabella hash della dimensione dell'input
tabella_hash nuova_hash(int dimensione){

    tabella_hash ht = (tabella_hash)malloc(sizeof(struct hash));
    if (ht == NULL){
        printf("Errore allocazione tabella hash.\n");
        exit(EXIT_FAILURE);
    }
    
    ht->dimensione = dimensione;
    ht->num_elem = 0;

    // Inizializzo l'array di puntatori a NULL con la calloc
    ht->tavola = (struct node**)calloc(ht->dimensione, sizeof(struct node*));
    if (ht->tavola == NULL){
        printf("Errore allocazione di memoria tabella_hash\n");
        exit(EXIT_FAILURE);
    }

    return ht;
}

// Funzione che inserisce un puntatore ad una struttura che rappresenta una attivita nella tabella hash
void inserisci_hash(tabella_hash ht, attivita elem){

    int idx = hash_fun(ht, ottieni_nome(elem));

    struct node* nuovo_nodo = (struct node*)malloc(sizeof(struct node));
    if (nuovo_nodo == NULL){
        printf("Errore allocazione nodo\n");
        exit(EXIT_FAILURE);
    }

    nuovo_nodo->elemento = elem;
    nuovo_nodo->next = NULL;

    // Inserisce il nuovo nodo alla testa della lista nella posizione idx della tabella hash
    nuovo_nodo->next = ht->tavola[idx];
    ht->tavola[idx] = nuovo_nodo;

    ht->num_elem++;
}

// Funzione che rimuove la attivita data in input dalla tabella hash
void cancella_hash(tabella_hash ht, attivita da_eliminare){

    int idx = hash_fun(ht, ottieni_nome(da_eliminare));

    struct node* curr = ht->tavola[idx];
    struct node* prev = NULL;

    // Scorro la lista dei nodi per trovare l'attività da eliminare
    while (curr != NULL) {
        if (curr->elemento == da_eliminare){

            // Se il nodo che cerco è il primo unico presente della lista
            if (prev == NULL){

                //Rimuovo dalla testa
                ht->tavola[idx] = curr->next;
            }else{

                //Conetto il nodo precedente con il sucessivo
                prev->next = curr->next;
            }   

            // Libero la memoria e decremento il numero di elementi nella tabella
            free(curr->elemento);  
            free(curr);
            ht->num_elem--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

// Funzione che stampa tutte le attivita presenti nella tabella o solo quelle con la priorita data in input
void stampa_hash(tabella_hash ht, int priorita){

    // Controllo sulla tabella in input
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
        while (curr != NULL) {
            if (priorita == 0 || ottieni_priorita(curr->elemento) == priorita) {
                stampa_attivita(curr->elemento);
            }
            curr = curr->next;
        }
    }
}

// Funzione per liberare tutta la memoria allocata per la tabella hash
void distruggi_hash(tabella_hash ht){
    // Controllo sulla tabella in input
    if (ht == NULL){
        return;
    }

    for (int i = 0; i < ht->dimensione; i++){
        struct node* curr = ht->tavola[i];

        // Scorro la lista di nodi e dealloco la memoria conservando un puntatore al nodo sucessivo
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

// Funzione che salva il contenuto della tabella hash su di un file dato in input
void salva_hash(tabella_hash ht, const char *nome_file){
    FILE *fd = fopen(nome_file, "w");
    if (fd == NULL) {
        printf("Errore apertura file %s\n", nome_file);
        return;
    }

    for (int i = 0; i < ht->dimensione; i++){
        struct node* curr = ht->tavola[i];

        // Salvo su file i dati dell'attivita nel nodo corrente
        while (curr != NULL){
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

// Funzione che inserisce le attivita salvate in un file dato in input nella tabella hash data in input
void carica_hash(tabella_hash ht, const char *nome_file){
    FILE *fd = fopen(nome_file, "r");
    if (fd == NULL) {
        printf("Errore apertura file %s\n", nome_file);
        return;
    }

    char buffer[8][MAX_DESC];

    while (1) {

        int i = 0;
        while(i < 8) {
            if (fgets(buffer[i], MAX_DESC, fd) == NULL) {
                break;
            }

            // Rimuove il carattere di nuova riga
            buffer[i][strcspn(buffer[i], "\n")] = '\0';
            i++;
        }

        // Controlla che siano stati letti tutti i dati necessari per creare un'attività
        if (i < 8){
            break; 
        }

        attivita a = crea_attivita(buffer[0], buffer[1], buffer[2], atoi(buffer[3]), 
                    atoi(buffer[4]), atol(buffer[5]), atol(buffer[6]), atol(buffer[7]));

        // Aggiorno lo status delle attività in base alla data corrente
        aggiorna_status(a);
        inserisci_hash(ht, a);
    }

    fclose(fd);
}

/*
* Funzione che genera un report delle settimanta corrente indicando 
* le attivita in corso, completate o in ritardo
*/ 
void report_settimanale_hash(tabella_hash ht){
    
    // Controllo sulla tabella in input
    if (ht == NULL) {
        printf("Erroe: Tabella hash vuota.\n");
        return;
    }

    // Contatori per lo status dell'attività
    int in_corso = 0;
    int in_ritardo = 0;
    int completate = 0;

    system("clear");
    printf("==================================================================================================\n");
    printf("|                                   REPORT SETTIMANALE                                           |\n");
    printf("==================================================================================================\n");

    for (int i = 0; i < ht->dimensione; i++){
        struct node* curr = ht->tavola[i];
            while (curr != NULL){

                // Se l'attivita cade nella settimanta corrente
                if (controllo_settimana(ottieni_scadenza(curr->elemento))){
                    stampa_attivita(curr->elemento);

                    // Incrementa uno dei contatori in base allo status dell'attività
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
    printf("==================================================================================================\n");
    printf("|  Questa settimana hai:  %d attività completate | %d attività in corso | %d attivita in ritardo    |\n", completate, in_corso, in_ritardo);
    printf("==================================================================================================\n");
}

/*
* Funzione che cerca tutte le attività con un certo nome e le inserisce nell'array di attività dato in input
* Restituisce il numero di attivita trovate
*/
int cerca_hash(tabella_hash ht, char* nome, attivita** risultati) {
    int idx = hash_fun(ht, nome);
    struct node* curr = ht->tavola[idx];

    // Dimensione dell'array dei risutati
    int dimensione = 5;
    int count = 0;

    *risultati = malloc(dimensione * sizeof(attivita));
    if (*risultati == NULL) {
        printf("Errore allocazione array di risultati.\n");
        return 0;
    }

    while (curr != NULL){

        // Se l'attivita ha il nome cercato
        if (strcmp(ottieni_nome(curr->elemento), nome) == 0){

            // Se la dimensione dell'array di risultati non è sufficiente viene raddoppiata
            if (count >= dimensione){
                dimensione *= 2;
                *risultati = realloc(*risultati, dimensione * sizeof(attivita));
                if (*risultati == NULL) {
                    printf("Errore realloc array di risultati.\n");
                    return count;
                }
            }

            // Aggiugi l'attivita all'array di risultati
            (*risultati)[count++] = curr->elemento;
        }
        curr = curr->next;
    }

    return count;
}

// Funzione che trasforma una stringa in un indice della tabella hash
int hash_fun(tabella_hash ht, char* nome){

    // Primo numero usato per mescolare i caratteri
    int factor = 31;
    int sum = 0; 
    for (int i = 0; i < strlen(nome); i++){

        // Somma pesata dei caratteri com modulo la dimensione della tabella hash   
        sum = ((sum % ht->dimensione)
               + (((int)nome[i]) * factor) % ht->dimensione)
              % ht->dimensione;

        // Aggiorna il peso
        factor = ((factor % __INT16_MAX__)
                  * (31 % __INT16_MAX__))
                 % __INT16_MAX__;
    }

    return sum;
}

// Funzione che restituisce il numero attivita presenti nella tabella
int ottieni_num_elem(tabella_hash ht){
    return ht->num_elem;
}