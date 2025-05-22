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
    struct node** tavola;     // Array di puntatori a struct node
};

/*
* SPECIFICA SINTATTICA
* - tabella_hash nuova_hash(int)
*     Tipi in uso: tabella_hash, int
*     Descrizione: crea una nuova tabella hash della dimensione data in input
*
* SPECIFICA SEMANTICA
* - nuova_hash(dim)
*     Precondizione: dimensione deve essere un valore maggiore di 0
*     Postcondizione: restituisce un puntatore ad una tabella hash ht allocata dinamicamete, di dimensione 
*                     dim e inizializzata a NULL. Se l'allocazione fallisce il programma termina con un exit faliure 
*     Side effect: nessuno
*/
tabella_hash nuova_hash(int dim){

    tabella_hash ht = (tabella_hash)malloc(sizeof(struct hash));
    if (ht == NULL){
        printf("Errore allocazione tabella hash.\n");
        exit(EXIT_FAILURE);
    }
    
    ht->dimensione = dim;
    ht->num_elem = 0;

    // Inizializzo l'array di puntatori a NULL tramite la funzione calloc
    ht->tavola = (struct node**)calloc(ht->dimensione, sizeof(struct node*));
    if (ht->tavola == NULL){
        printf("Errore allocazione di memoria tabella_hash\n");
        exit(EXIT_FAILURE);
    }

    return ht;
}

/*
* SPECIFICA SINTATTICA
* - void inserisci_hash(tabella_hash, attivita)
*     Tipi in uso: tabella\_hash, attivita
*     Descrizione: aggiunge un elemento nella tabella hash, gestendo eventuali collisioni tramite il concatenamento
*
* SPECIFICA SEMANTICA
* - inserisci_hash(ht, elem)
*     Precondizione: ht e elem devono essere due puntatori validi e inizializzati correttamente
*     Postcondizione: restituisce ht dopo aver inserito l'attivita elem nella lista di collisione di 
*                     indice idx calcolato dalla funzione di hash, e aumentato il num_elem di ht 
*     Side effect: nessuno
*/
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

/*
* SPECIFICA SINTATTICA
* - void cancella_hash(tabella_hash, attivita)
*     Tipi in uso: tabella_hash, attivita
*     Descrizione: rimuove dalla tabella hash l'attività data in input, 
*                  cercandola nella lista corrispondente alla chiave calcolata tramite funzione di hash
*
* SPECIFICA SEMANTICA
* - cancella_hash(ht, da_eliminare)
*     Precondizione: ht e da_eliminare devono essere due puntatori validi e inizializzati correttamente
*     Postcondizione: restituisce ht dopo aver eliminato l'attivita da_eliminare e ne decrementa il suo num_elem 
*     Side effect: nessuno
*/
void cancella_hash(tabella_hash ht, attivita da_eliminare){

    int idx = hash_fun(ht, ottieni_nome(da_eliminare));

    struct node* curr = ht->tavola[idx];
    struct node* prev = NULL;

    // Scorro la lista di collisione dei nodi per trovare l'attività da eliminare
    while (curr != NULL) {
        if (curr->elemento == da_eliminare){

            // Se il nodo che cerco è il primo della lista
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

/*
* SPECIFICA SINTATTICA
* - void stampa_hash(tabella_hash, attivita)
*     Tipi in uso: tabella_hash, attivita
*     Descrizione: percorre la tabella hash nella sua interezza e stampa ogni attivita 
*                  con priorita uguale a quella data in input. Non applica filtri se la priorità è 0
*
* SPECIFICA SEMANTICA
* - stampa_hash(ht, mod)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, mod è un intero compreso tra 0 e 3
*     Postcondizione: nessuna 
*     Side effect: stampa a schermo tutte le attivita se mod = 0, 
*                  tutte le attivita con priorita = mod se mod != 0 e un messaggio di errore se ht è NULL o vuota 
*/
void stampa_hash(tabella_hash ht, int mod){

    // Controllo sulla tabella in input
    if (ht == NULL || ottieni_num_elem(ht) == 0) {
        printf("Erroe: Tabella hash vuota.\n");
        return;
    }

    pulisci_schermo();;
    printf("==================================================================================================\n");
    if (mod > 0) {
        printf("|                           Attivita con priorità %d:                                             |\n", mod);
    } else {
        printf("|                             Tutte le attività:                                                  |\n");
    }
    printf("==================================================================================================\n");

    // Ciclo per scorre la tabella
    for (int i = 0; i < ht->dimensione; i++) {
        struct node* curr = ht->tavola[i];
        while (curr != NULL) {

            // Stampa se la moda è 0 o concide con la priorità
            if (mod == 0 || ottieni_priorita(curr->elemento) == mod) {
                stampa_attivita(curr->elemento);
            }
            curr = curr->next;
        }
    }
}

/*
* SPECIFICA SINTATTICA
* void distruggi_hash(tabella_hash)
*     Tipi in uso: tabella_hash
*     Descrizione: libera tutta la memoria allocata dinamicamente per la tabella hash e le sue attivita
*
* SPECIFICA SEMANTICA
* - distruggi_hash(ht)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente
*     Postcondizione: nessuna 
*     Side effect: libera la memoria allocata per ogni attivita e ogni nodo della tabella hash e 
*                  infine dealloca la tabella stessa. Non fa niente se la tabella è già NULL 
*/
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

/*
* SPECIFICA SINTATTICA
* - void salva_hash(tabella_hash, const char*)
*     Tipi in uso: tabella_hash, stringa
*     Descrizione: salva il contenuto della tabella hash su di un file dato in input
*
* SPECIFICA SEMANTICA
* - salva_hash(ht, nome_file)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, nome_file è una stringa che 
                     contiene un nome di file valido
*     Postcondizione: nessuna 
*     Side effect: salva tutti gli elementi della ht nel file nome_file, in cui ogni 8 righe 
*                  rappresentano un'attivita divisa per campi. Stampa un messaggio di errore se il file 
*                  non viene aperto correttamente
*/
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

/*
* SPECIFICA SINTATTICA
* - void carica_hash(tabella_hash, const char*)
*     Tipi in uso: tabella_hash, stringa
*     Descrizione: ricostruisce le attivita salvate in un file dato in input, 
*                  ne aggiorna lo status e le inserisce nella tabella hash data in input
*
* SPECIFICA SEMANTICA
* - carica_hash(ht, nome_file)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, nome_file è una stringa che 
*                    contiene un nome di file valido e in cui ogni 8 righe rappresentano un'attivita divisa per campi
*     Postcondizione: nessuna 
*     Side effect: tutte le attività lette da nome_file vengono create, aggiornate e inserite nella tabella hash ht. 
*                  Stampa un messaggio di errore se il file non viene aperto correttamente
*/
void carica_hash(tabella_hash ht, const char *nome_file){
    FILE *fd = fopen(nome_file, "r");
    if (fd == NULL) {
        printf("Errore apertura file %s\n", nome_file);
        return;
    }

    // Dimensione della matrice data dal numero di capi di un attivita
    // per la dimensione del campo più grande
    char buffer[8][MAX_DESC];

    while (1){

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
* SPECIFICA SINTATTICA
* - void report_settimanale_hash(tabella_hash, const char*)
*     Tipi in uso: tabella\_hash, stringa
*     Descrizione: genera un report delle settimanta corrente indicando le attivita in corso, 
*                  completate e in ritardo e lo salva sul file dato in input
*
* SPECIFICA SEMANTICA
* - report_settimanale_hash(ht, nome_file)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, nome_file è una stringa che 
*                    contiene un nome di file valido
*     Postcondizione: nessuna 
*     Side effect: crea il file nome_file contenente tutte la attivita della settimana presenti in ht, 
*                  il numero di attivita completate, in corso e in ritardo. 
*                  Stampa a schermo una conferma della creazione del file
*/
void report_settimanale_hash(tabella_hash ht, const char *nome_file){
    
    // Controllo sulla tabella in input
    if (ht == NULL) {
        printf("Erroe: Tabella hash vuota.\n");
        return;
    }

    FILE *fd = fopen(nome_file, "w");
    if (fd == NULL) {
        printf("Errore apertura file per il report settimanale.\n");
        return;
    }

    // Contatori per lo status delle attività
    int in_corso = 0;
    int in_ritardo = 0;
    int completate = 0;

    pulisci_schermo();;
    fprintf(fd, "==================================================================================================\n");
    fprintf(fd, "|                                   REPORT SETTIMANALE                                           |\n");
    fprintf(fd, "==================================================================================================\n");

    for (int i = 0; i < ht->dimensione; i++){
        struct node* curr = ht->tavola[i];
            while (curr != NULL){

                // Se l'attivita cade nella settimanta corrente
                if (controllo_settimana(ottieni_scadenza(curr->elemento))){

                    stampa_attivita_file(curr->elemento, fd);

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
    fprintf(fd, "==================================================================================================\n");
    fprintf(fd, "|  Questa settimana hai:  %d attività completate | %d attività in corso | %d attivita in ritardo    |\n", completate, in_corso, in_ritardo);
    fprintf(fd, "==================================================================================================\n");

    fclose(fd);
    printf("==================================================================================================\n");
    printf("|                      Record settimanale salvato su \"report_settimanale.txt\"                    |\n");
    printf("==================================================================================================\n");
}

/*
* SPECIFICA SINTATTICA
* - void puliza_hash(tabella_hash, int)
*     Tipi in uso: tabella_hash, intero
*     Descrizione: scorre l'intera tabella hash e rimuove o tutte le attivita completate,
*                  o tutte le attività in ritardo o entrambe le categorie
*
* SPECIFICA SEMANTICA
* - pulizia_hash(ht, mod)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, mod è un intero compreso tra 1 e 3
*     Postcondizione: nessuna 
*     Side effect: rimuova dalla tabella hash ht tutte le attivita completate se mod = 1, 
*                  tutte le attivita in ritardo se mod = 2 e entrambi i tipi di attivita se mod = 3. 
*                  Stampa a schermo il numero di attivita eliminate
*/
void pulizia_hash(tabella_hash ht, int mod){
    
    // Controllo sulla tabella in input
    if (ht == NULL) {
        printf("Erroe: Tabella hash vuota.\n");
        return;
    }

    // Contatore attivita eliminate
    int conto = 0;

    for (int i = 0; i < ht->dimensione; i++){
        struct node* curr = ht->tavola[i];

            while (curr != NULL){
                struct node* temp = curr->next;

                // Se l'attività è completata o in ritardo e la modalità di cancellazione conbaciano
                if ((ottieni_status(curr->elemento) == 1 && (mod == 1 || mod == 3)) || 
                    (ottieni_status(curr->elemento) == 2 && (mod == 2 || mod == 3))){
                    
                    cancella_hash(ht, curr->elemento);
                    conto++;   
                }
                
                curr = temp;
            }
    }

    printf("==================================================================================================\n");
    printf("|                                 %d Attività eliminate                                           |\n", conto);
    printf("==================================================================================================\n");
}

/*
* SPECIFICA SINTATTICA
* - int cerca_hash(tabella_hash, char*, attivita**)
*     Tipi in uso: tabella_hash, intero, stringa, array di attivita
*     Descrizione: cerca tutte le attività nella tabella hash che hanno nome uguale alla stringa passta in input, 
*                           e le inserisce nell'array di attività dato in input. Restituisce il numero di attivita trovate
*
* SPECIFICA SEMANTICA
* - cerca_hash(ht, nome, risultati)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, nome è una stringa non nulla
*     Postcondizione: restituisce un intero count uguale al numero di attivita trovate e 
*                     inserite nell'array out ()>= 0)
*     Side effect: alloca la memoria per l'array risulatati vi inserisce i puntatori delle attivita 
*                  con nome uguale alla stringa in input 
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
                    return 0;
                }
            }

            // Aggiugi l'attivita all'array di risultati
            (*risultati)[count++] = curr->elemento;
        }
        curr = curr->next;
    }

    return count;
}

/*
* SPECIFICA SINTATTICA
* - int hash_fun(tabella_hash, char*)
*     Tipi in uso: tabella_hash, intero, stringa
*     Descrizione: calcola una funzione di hash basata su una somma pesata dei caratteri nella stringa nome, 
*                  utilizzando un fattore moltiplicativo variabile e un modulo con la dimensione della tabella
*
* SPECIFICA SEMANTICA
* - hash_fun(ht, nome)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente, 
*                    nome è una stringa non nulla, ht->dimensione > 0
*     Postcondizione: restituisce un intero sum compreso tra 0 e ht->dimensione-1, 
*                    ottenuto dalla somma pesata dei caratteri di nome con modulo ht->dimensione
*     Side effect: nessuno
*/
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

/*
* SPECIFICA SINTATTICA
* - int ottieni_num_elem(tabella_hash)
*     Tipi in uso: tabella_hash, intero
*     Descrizione: restituisce il numero di elementi presenti nella tabella hash
*
* SPECIFICA SEMANTICA
* - ottieni_num_elem(ht)
*     Precondizione: ht è un puntatore valido e inizializzato correttamente
*     Postcondizione: restituisce ht->num_elem se la tabella non è NULL e -1 altrimenti
*     Side effect: nessuno
*/
int ottieni_num_elem(tabella_hash ht){
    if (ht == NULL){
        return -1;
    }
    return ht->num_elem;
}