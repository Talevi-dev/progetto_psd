#include <stdio.h>
#include <time.h>

int main (){
    time_t tempo = 0;

    tempo = tempo + (400*3600);

    printf("La data e ora convertite in data_t: %ld\n%ld\n%s\n", (tempo/3600), tempo, ctime(&tempo));

    return 0;
}