typedef struct node *alberoAVL;

alberoAVL nuovo_alberoAVL();

alberoAVL* nuova_forestaAVL(int n);

alberoAVL inserisci_alberoAVL_prt(alberoAVL N, attivita a);

alberoAVL inserisci_alberoAVL_scd(alberoAVL N, attivita a);

alberoAVL crea_foglia(attivita a);

alberoAVL bilancia_alberoAVL(alberoAVL N, int bil);

alberoAVL rotazione_destra(alberoAVL y);

alberoAVL rotazione_sinistra(alberoAVL x);

alberoAVL cancella_alberoAVL_prt(alberoAVL N, attivita a);

alberoAVL cancella_alberoAVL_scd(alberoAVL N, attivita a);

alberoAVL min_nodo(alberoAVL node);

int ottieni_altezza(alberoAVL N);

int ottieni_bilanciamento(alberoAVL N);

int max(int a, int b);

void stampa_albero(alberoAVL T);