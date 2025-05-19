#include <time.h>

#define MAX_DATA 16+1

void pulisci_buffer();

void pulisci_schermo();

int input_stringa(char *buff, int len);

int input_intero(int *input, int x, int y);

int controlla_input_data(char *data);

int controllo_settimana(time_t scadenza);

time_t converti_data (char *data);
