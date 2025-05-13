#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "attivita.h"
#include "funzioni_attivita.h"
#include "alberoAVL.h"

struct node{
    attivita chiave;
    alberoAVL sx_priorita;
    alberoAVL dx_priorita;
    alberoAVL sx_scadenza;
    alberoAVL dx_scadenza;
    int altezza_prt;
    int altezza_scd;
};

int ottieni_altezza_prt(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return N->altezza_prt;
}

int ottieni_altezza_scd(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return N->altezza_scd;
}

alberoAVL crea_foglia(attivita a){
    alberoAVL N = (alberoAVL)malloc(sizeof(struct node));
    if (N == NULL){
        printf("Errore allocazione memoria nodo\n");
        return NULL;
    }
    N->chiave = a;
    N->sx_priorita = NULL;
    N->dx_priorita = NULL;
    N->sx_scadenza = NULL;
    N->dx_scadenza = NULL;
    N->altezza_prt = 1;
    N->altezza_scd = 1;

    return N;
}

int max(int a, int b){ 
    return (a > b) ? a : b;
}

int ottieni_bilanciamento_prt(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return ottieni_altezza_prt(N->sx_priorita) - ottieni_altezza_prt(N->dx_priorita);
}

int ottieni_bilanciamento_scd(alberoAVL N){
    if (N == NULL){
        return 0;
    }
    return ottieni_altezza_scd(N->sx_scadenza) - ottieni_altezza_scd(N->dx_scadenza);
}

alberoAVL rotazione_dx_priorita(alberoAVL y){
    alberoAVL x = y->sx_priorita;
    alberoAVL T2 = x->dx_priorita;

    x->dx_priorita = y;
    y->sx_priorita = T2;

    y->altezza_prt = max(ottieni_altezza_prt(y->sx_priorita), ottieni_altezza_prt(y->dx_priorita)) + 1;
    x->altezza_prt = max(ottieni_altezza_prt(x->sx_priorita), ottieni_altezza_prt(x->dx_priorita)) + 1;

    return x;
}

alberoAVL rotazione_dx_scadenza(alberoAVL y){
    alberoAVL x = y->sx_scadenza;
    alberoAVL T2 = x->dx_scadenza;

    x->dx_scadenza = y;
    y->sx_scadenza = T2;

    y->altezza_scd = max(ottieni_altezza_scd(y->sx_scadenza), ottieni_altezza_scd(y->dx_scadenza)) + 1;
    x->altezza_scd = max(ottieni_altezza_scd(x->sx_scadenza), ottieni_altezza_scd(x->dx_scadenza)) + 1;

    return x;
}

alberoAVL rotazione_sx_priorita(alberoAVL x){
    alberoAVL y = x->dx_priorita;
    alberoAVL T2 = y->sx_priorita;

    y->sx_priorita = x;
    x->dx_priorita = T2;

    x->altezza_prt = max(ottieni_altezza_prt(x->sx_priorita), ottieni_altezza_prt(x->dx_priorita)) + 1;
    y->altezza_prt = max(ottieni_altezza_prt(y->sx_priorita), ottieni_altezza_prt(y->dx_priorita)) + 1;

    return y;
}

alberoAVL rotazione_sx_scadenza(alberoAVL x){
    alberoAVL y = x->dx_scadenza;
    alberoAVL T2 = y->sx_scadenza;

    y->sx_scadenza = x;
    x->dx_scadenza = T2;

    x->altezza_scd = max(ottieni_altezza_scd(x->sx_scadenza), ottieni_altezza_scd(x->dx_scadenza)) + 1;
    y->altezza_scd = max(ottieni_altezza_scd(y->sx_scadenza), ottieni_altezza_scd(y->dx_scadenza)) + 1;

    return y;
}

alberoAVL inserisci_alberoAVL(alberoAVL N, attivita a){
    if (N == NULL){
        return crea_foglia(a);
    }
    
    if (confronta_attivita_priorita(a, N->chiave) < 0){
        N->sx_priorita = inserisci_alberoAVL(N->sx_priorita, a);
    }else if (confronta_attivita_priorita(a, N->chiave) > 0){
        N->dx_priorita = inserisci_alberoAVL(N->dx_priorita, a);
    }else{
        return N;
    }

    N->altezza_prt = 1 + max(ottieni_altezza_prt(N->sx_priorita), ottieni_altezza_prt(N->dx_priorita));

    N = bilancia_alberoAVL_prt(N, ottieni_bilanciamento_prt(N));

    return N;
}

alberoAVL cerca_nodo_prt(alberoAVL root, attivita a) {
    // Se l'albero è vuoto o abbiamo trovato il nodo
    if (root == NULL || confronta_attivita_priorita(root->chiave, a) == 0)
        return root;

    // Se la chiave è minore, cerca nel sotto-albero sinistro
    if (confronta_attivita_priorita(a, root->chiave) < 0)
        return cerca_nodo_prt(root->sx_priorita, a);

    // Altrimenti cerca nel sotto-albero destro
    return cerca_nodo_prt(root->dx_priorita, a);
}

alberoAVL aggiorna_alberoAVL_scadenza(alberoAVL N, alberoAVL temp){
    if (N == NULL){
        N = temp;
        return N;
    }

    if (confronta_attivita_scadenza(temp->chiave, N->chiave) < 0){
        N->sx_scadenza = aggiorna_alberoAVL_scadenza(N->sx_scadenza, temp);
    }else if (confronta_attivita_scadenza(temp->chiave, N->chiave) > 0){
        N->dx_scadenza = aggiorna_alberoAVL_scadenza(N->dx_scadenza, temp);
    }else{
        return N;
    }

    N->altezza_scd = 1 + max(ottieni_altezza_scd(N->sx_scadenza), ottieni_altezza_scd(N->dx_scadenza));

    N = bilancia_alberoAVL_scd(N, ottieni_bilanciamento_scd(N));

    return N;
}

void stampa_albero_prt(alberoAVL T){
    if (T != NULL) {
        stampa_albero_prt(T->sx_priorita);
        stampa_attivita(T->chiave);
        stampa_albero_prt(T->dx_priorita);
    }
}

void stampa_albero_scd(alberoAVL T){
    if (T != NULL) {
        stampa_albero_scd(T->sx_scadenza);
        stampa_attivita(T->chiave);
        stampa_albero_scd(T->dx_scadenza);
    }
}

alberoAVL min_nodo(alberoAVL node){
    alberoAVL current = node;

    while (current->sx_priorita != NULL){
        current = current->sx_priorita;
    }

    return current;
}


alberoAVL bilancia_alberoAVL_prt(alberoAVL N, int bil){
    if (bil > 1 && ottieni_bilanciamento_prt(N->sx_priorita) >= 0){
        return rotazione_dx_priorita(N);
    }

    if (bil > 1 && ottieni_bilanciamento_prt(N->sx_priorita) < 0){
        N->sx_priorita =  rotazione_sx_priorita(N->sx_priorita);
        return rotazione_dx_priorita(N);
    }

    if (bil < -1 && ottieni_bilanciamento_prt(N->dx_priorita) <= 0){
        return rotazione_sx_priorita(N);
    }

    if (bil < -1 && ottieni_bilanciamento_prt(N->dx_priorita) > 0){
        N->dx_priorita = rotazione_dx_priorita(N->dx_priorita);
        return rotazione_sx_priorita(N);
    }
}

alberoAVL bilancia_alberoAVL_scd(alberoAVL N, int bil){
    if (bil > 1 && ottieni_bilanciamento_scd(N->sx_scadenza) >= 0){
        return rotazione_dx_scadenza(N);
    }

    if (bil > 1 && ottieni_bilanciamento_scd(N->sx_scadenza) < 0){
        N->sx_scadenza =  rotazione_sx_scadenza(N->sx_scadenza);
        return rotazione_dx_scadenza(N);
    }

    if (bil < -1 && ottieni_bilanciamento_scd(N->dx_scadenza) <= 0){
        return rotazione_sx_scadenza(N);
    }

    if (bil < -1 && ottieni_bilanciamento_scd(N->dx_scadenza) > 0){
        N->dx_scadenza = rotazione_dx_scadenza(N->dx_scadenza);
        return rotazione_sx_scadenza(N);
    }
}

/*alberoAVL cancella_alberoAVL(alberoAVL N, attivita a){
    if (N == NULL){
        return N;
    }

    if ( confronta_attivita_priorita(a, N->chiave) < 0){
        N->sx_priorita = cancella_alberoAVL(N->sx_priorita, a);
    }else if( confronta_attivita_priorita(a, N->chiave) > 0){
        N->dx_priorita = cancella_alberoAVL(N->dx_priorita, a);
    }else{

        free(N->chiave);
        if( (N->sx_priorita == NULL) || (N->dx_priorita == NULL) ){
            alberoAVL temp = N->sx_priorita ? N->sx_priorita : N->dx_priorita;

            if (temp == NULL){
                temp = N;
                N = NULL;
            }else{
                N = temp; 
            }   

            free(temp);
        }else{
            alberoAVL temp = min_nodo(N->dx_priorita);
            copia_attivita(N->chiave, temp->chiave);
            N->dx_priorita = cancella_alberoAVL(N->dx_priorita, temp->chiave);
        }
    }
    
    if (N == NULL){
        return N;
    }

    N->altezza_prt = 1 + max(ottieni_altezza_prt(N->sx_priorita), ottieni_altezza_prt(N->dx_priorita));

    N = bilancia_alberoAVL(N, ottieni_bilanciamento(N));
    
    return N;
}*/

int main()
{
    alberoAVL T = NULL;
    alberoAVL temp;


    attivita a1 = crea_attivita("a", "a", "a", 1, 0, 0, 100, 7);
    attivita a2 = crea_attivita("b", "a", "a", 2, 0, 0, 100, 8);
    attivita a3 = crea_attivita("c", "a", "a", 3, 0, 0, 100, 3); 
    attivita a4 = crea_attivita("d", "a", "a", 4, 0, 0, 100, 5);
    attivita a5 = crea_attivita("e", "a", "a", 8, 0, 0, 100, 4);
    attivita a6 = crea_attivita("f", "a", "a", 7, 0, 0, 100, 6);
    attivita a7 = crea_attivita("g", "a", "a", 6, 0, 0, 100, 1); 
    attivita a8 = crea_attivita("h", "a", "a", 5, 0, 0, 100, 2);

    T = inserisci_alberoAVL(T, a1);
    temp = cerca_nodo_prt(T, a1);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a2);
    temp = cerca_nodo_prt(T, a2);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a3);
    temp = cerca_nodo_prt(T, a3);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a4);
    temp = cerca_nodo_prt(T, a4);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a5);
    temp = cerca_nodo_prt(T, a5);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a6);
    temp = cerca_nodo_prt(T, a6);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a7);
    temp = cerca_nodo_prt(T, a7);
    T = aggiorna_alberoAVL_scadenza(T, temp);
    T = inserisci_alberoAVL(T, a8);
    temp = cerca_nodo_prt(T, a8);
    T = aggiorna_alberoAVL_scadenza(T, temp);

    printf("ALBERO PRIORITA:\n");
    stampa_albero_prt(T);
    printf("ALBERO SCADENZA:\n");
    stampa_albero_scd(T);

    //T = cancella_alberoAVL(T, a8);
    //T = cancella_alberoAVL(T, a1);

    //printf("Albero dopo cancellazione:\n");
    //stampa_albero(T);

    return 0;
}