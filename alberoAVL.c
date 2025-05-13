#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "attivita.h"
#include "funzioni_attivita.h"
#include "alberoAVL.h"

struct node{
    attivita chiave;
    alberoAVL sinistra;
    alberoAVL destra;
    int altezza;
};

int ottieni_altezza(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return N->altezza;
}

alberoAVL crea_foglia(attivita a){
    alberoAVL N = (alberoAVL)malloc(sizeof(struct node));
    if (N == NULL){
        printf("Errore allocazione memoria nodo\n");
        return NULL;
    }
    N->chiave = a;
    N->sinistra = NULL;
    N->destra = NULL;
    N->altezza = 1;

    return N;
}

int max(int a, int b){ 
    return (a > b) ? a : b;
}

int ottieni_bilanciamento(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return ottieni_altezza(N->sinistra) - ottieni_altezza(N->destra);
}

alberoAVL rotazione_destra(alberoAVL y){
    alberoAVL x = y->sinistra;
    alberoAVL T2 = x->destra;

    x->destra = y;
    y->sinistra = T2;

    y->altezza = max(ottieni_altezza(y->sinistra), ottieni_altezza(y->destra)) + 1;
    x->altezza = max(ottieni_altezza(x->sinistra), ottieni_altezza(x->destra)) + 1;

    return x;
}

alberoAVL rotazione_sinistra(alberoAVL x){
    alberoAVL y = x->destra;
    alberoAVL T2 = y->sinistra;

    y->sinistra = x;
    x->destra = T2;

    x->altezza = max(ottieni_altezza(x->sinistra), ottieni_altezza(x->destra)) + 1;
    y->altezza = max(ottieni_altezza(y->sinistra), ottieni_altezza(y->sinistra)) + 1;

    return y;
}

alberoAVL inserisci_alberoAVL(alberoAVL N, attivita a){
    if (N == NULL){
        return crea_foglia(a);
    }

    if (ottieni_priorita(a) < ottieni_priorita(N->chiave)){
        N->sinistra = inserisci_alberoAVL(N->sinistra, a);
    }else if (ottieni_priorita(a) > ottieni_priorita(N->chiave)){
        N->destra = inserisci_alberoAVL(N->destra, a);
    }else{
        return N;
    }

    N->altezza = 1 + max(ottieni_altezza(N->sinistra), ottieni_altezza(N->destra));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));

    return N;
}

void stampa_albero(alberoAVL T){
    if (T != NULL) {
        stampa_albero(T->sinistra);
        stampa_attivita(T->chiave);
        stampa_albero(T->destra);
    }
}

alberoAVL min_nodo(alberoAVL node){
    alberoAVL current = node;

    while (current->sinistra != NULL){
        current = current->sinistra;
    }

    return current;
}


alberoAVL bilancia_alberoAVL(alberoAVL N, int bil){
    if (bil > 1 && ottieni_bilanciamento(N->sinistra) >= 0){
        return rotazione_destra(N);
    }

    if (bil > 1 && ottieni_bilanciamento(N->sinistra) < 0){
        N->sinistra =  rotazione_sinistra(N->sinistra);
        return rotazione_destra(N);
    }

    if (bil < -1 && ottieni_bilanciamento(N->destra) <= 0){
        return rotazione_sinistra(N);
    }

    if (bil < -1 && ottieni_bilanciamento(N->destra) > 0){
        N->destra = rotazione_destra(N->destra);
        return rotazione_sinistra(N);
    }
}

alberoAVL cancella_alberoAVL(alberoAVL N, attivita a){
    if (N == NULL){
        return N;
    }

    if ( ottieni_priorita(a) < ottieni_priorita(N->chiave)){
        N->sinistra = cancella_alberoAVL(N->sinistra, a);
    }else if( ottieni_priorita(a) > ottieni_priorita(N->chiave)){
        N->destra = cancella_alberoAVL(N->destra, a);
    }else{

        free(N->chiave);
        if( (N->sinistra == NULL) || (N->destra == NULL) ){
            alberoAVL temp = N->sinistra ? N->sinistra : N->destra;

            if (temp == NULL){
                temp = N;
                N = NULL;
            }else{
                N = temp; 
            }   

            free(temp);
        }else{
            alberoAVL temp = min_nodo(N->destra);
            copia_attivita(N->chiave, temp->chiave);
            N->destra = cancella_alberoAVL(N->destra, temp->chiave);
        }
    }
    
    if (N == NULL){
        return N;
    }

    N->altezza = 1 + max(ottieni_altezza(N->sinistra), ottieni_altezza(N->destra));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));
    
    return N;
}

int main()
{
    alberoAVL T = NULL;


    attivita a1 = crea_attivita("a", "a", "a", 1, 0, 0, 100, 1);
    attivita a2 = crea_attivita("b", "a", "a", 2, 0, 0, 100, 2);
    attivita a3 = crea_attivita("c", "a", "a", 3, 0, 0, 100, 3); 
    attivita a4 = crea_attivita("d", "a", "a", 4, 0, 0, 100, 4);
    attivita a5 = crea_attivita("e", "a", "a", 8, 0, 0, 100, 8);
    attivita a6 = crea_attivita("f", "a", "a", 7, 0, 0, 100, 7);
    attivita a7 = crea_attivita("g", "a", "a", 6, 0, 0, 100, 6); 
    attivita a8 = crea_attivita("h", "a", "a", 5, 0, 0, 100, 5);

    T = inserisci_alberoAVL(T, a1);
    T = inserisci_alberoAVL(T, a2);
    T = inserisci_alberoAVL(T, a3);
    T = inserisci_alberoAVL(T, a4);
    T = inserisci_alberoAVL(T, a5);
    T = inserisci_alberoAVL(T, a6);
    T = inserisci_alberoAVL(T, a7);
    T = inserisci_alberoAVL(T, a8);

    printf("Albero AVL:\n");
    stampa_albero(T);

    T = cancella_alberoAVL(T, a8);
    T = cancella_alberoAVL(T, a1);

    printf("Albero dopo cancellazione:\n");
    stampa_albero(T);

    return 0;
}