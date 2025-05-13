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

alberoAVL nuovo_alberoAVL(){
    return NULL;
}

alberoAVL* nuova_forestaAVL(int n) {
    alberoAVL* forestaAVL = malloc(sizeof(alberoAVL) * n);

    if (forestaAVL == NULL) {
        printf("Errore allocazione array di alberi\n");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        forestaAVL[i] = nuovo_alberoAVL();
    }

    return forestaAVL;
}


alberoAVL* inserisci_alberoAVL(alberoAVL *F, attivita a){

    attivita copia = crea_attivita("", "", "", 0, 0, 0, 0, 0);
    copia_attivita(copia, a);

    F[0] = inserisci_alberoAVL_prt(F[0], a);
    F[1] = inserisci_alberoAVL_scd(F[1], copia);

    return F;
}

alberoAVL inserisci_alberoAVL_prt(alberoAVL N, attivita a){
    if (N == NULL){
        return crea_foglia(a);
    }
    
    if (confronta_attivita_priorita(a, N->chiave) < 0){
        N->sinistra = inserisci_alberoAVL_prt(N->sinistra, a);
    }else if (confronta_attivita_priorita(a, N->chiave) > 0){
        N->destra = inserisci_alberoAVL_prt(N->destra, a);
    }else{
        return N;
    }

    N->altezza = 1 + max(ottieni_altezza(N->sinistra), ottieni_altezza(N->destra));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));

    return N;
}

alberoAVL inserisci_alberoAVL_scd(alberoAVL N, attivita a){
    if (N == NULL){
        return crea_foglia(a);
    }
    
    if (confronta_attivita_scadenza(a, N->chiave) < 0){
        N->sinistra = inserisci_alberoAVL_scd(N->sinistra, a);
    }else if (confronta_attivita_scadenza(a, N->chiave) > 0){
        N->destra = inserisci_alberoAVL_scd(N->destra, a);
    }else{
        return N;
    }

    N->altezza = 1 + max(ottieni_altezza(N->sinistra), ottieni_altezza(N->destra));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));

    return N;
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

alberoAVL *cancella_alberoAVL(alberoAVL *F, attivita a){
    F[0] = cancella_alberoAVL_prt(F[0], a);
    F[1] = cancella_alberoAVL_scd(F[1], a);

    return F;
}

alberoAVL cancella_alberoAVL_prt(alberoAVL N, attivita a){
    if (N == NULL){
        return N;
    }

    if (confronta_attivita_priorita(a, N->chiave) < 0){
        N->sinistra = cancella_alberoAVL_prt(N->sinistra, a);
    }else if( confronta_attivita_priorita(a, N->chiave) > 0){
        N->destra = cancella_alberoAVL_prt(N->destra, a);
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
            N->destra = cancella_alberoAVL_prt(N->destra, temp->chiave);
        }
    }
    
    if (N == NULL){
        return N;
    }

    N->altezza = 1 + max(ottieni_altezza(N->sinistra), ottieni_altezza(N->destra));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));
    
    return N;
}

alberoAVL cancella_alberoAVL_scd(alberoAVL N, attivita a){
    if (N == NULL){
        return N;
    }

    if (confronta_attivita_scadenza(a, N->chiave) < 0){
        N->sinistra = cancella_alberoAVL_scd(N->sinistra, a);
    }else if( confronta_attivita_scadenza(a, N->chiave) > 0){
        N->destra = cancella_alberoAVL_scd(N->destra, a);
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
            N->destra = cancella_alberoAVL_scd(N->destra, temp->chiave);
        }
    }
    
    if (N == NULL){
        return N;
    }

    N->altezza = 1 + max(ottieni_altezza(N->sinistra), ottieni_altezza(N->destra));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));
    
    return N;
}

alberoAVL min_nodo(alberoAVL node){
    alberoAVL current = node;

    while (current->sinistra != NULL){
        current = current->sinistra;
    }

    return current;
}

int ottieni_altezza(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return N->altezza;
}

int ottieni_bilanciamento(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return ottieni_altezza(N->sinistra) - ottieni_altezza(N->destra);
}

int max(int a, int b){ 
    return (a > b) ? a : b;
}

void stampa_albero(alberoAVL T){
    if (T != NULL) {
        stampa_albero(T->sinistra);
        stampa_attivita(T->chiave);
        stampa_albero(T->destra);
    }
}

int main()
{
    alberoAVL *F = nuova_forestaAVL(2);


    attivita a1 = crea_attivita("a", "a", "a", 1, 0, 0, 100, 1);
    attivita a2 = crea_attivita("b", "a", "a", 2, 0, 0, 100, 2);
    attivita a3 = crea_attivita("c", "a", "a", 3, 0, 0, 100, 3);
    attivita a4 = crea_attivita("d", "a", "a", 1, 0, 0, 100, 4);
    attivita a5 = crea_attivita("e", "a", "a", 1, 0, 0, 100, 8);
    attivita a6 = crea_attivita("f", "a", "a", 1, 0, 0, 100, 7);
    attivita a7 = crea_attivita("g", "a", "a", 6, 0, 0, 100, 6);
    attivita a8 = crea_attivita("h", "a", "a", 5, 0, 0, 100, 5);

    F = inserisci_alberoAVL(F, a1);
    F = inserisci_alberoAVL(F, a2);
    F = inserisci_alberoAVL(F, a3);
    F = inserisci_alberoAVL(F, a4);
    F = inserisci_alberoAVL(F, a5);
    F = inserisci_alberoAVL(F, a6);
    F = inserisci_alberoAVL(F, a7);
    F = inserisci_alberoAVL(F, a8);

    /*F[1] = inserisci_alberoAVL_scd(F[1], a1_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a2_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a3_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a4_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a5_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a6_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a7_s);
    F[1] = inserisci_alberoAVL_scd(F[1], a8_s);*/

    printf("Albero AVL:\n");
    stampa_albero(F[0]);
    printf("Albero AVL:\n");
    stampa_albero(F[1]);

    F = cancella_alberoAVL(F, a8);
    F = cancella_alberoAVL(F, a1);

    printf("Albero dopo cancellazione:\n");
    stampa_albero(F[0]);
    printf("Albero dopo cancellazione:\n");
    stampa_albero(F[1]);

    return 0;
}