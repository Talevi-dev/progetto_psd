typedef struct act *attivita;

#define MAX_NOME 50+1
#define MAX_DESC 300+1 

attivita crea_attivita(char *n, char *c, char *d, int p, int x, time_t scd);

attivita input_attivita();
