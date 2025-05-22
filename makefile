agenda.exe: agenda.o hash.o attivita.o utils.o funzioni_attivita.o
	gcc agenda.o hash.o attivita.o utils.o funzioni_attivita.o -o agenda.exe 

agenda.o: agenda.c hash.h attivita.h funzioni_attivita.h utils.h
	gcc -c agenda.c

funzioni_attivita.o: funzioni_attivita.c funzioni_attivita.h attivita.h utils.h
	gcc -c funzioni_attivita.c

hash.o: hash.c attivita.h funzioni_attivita.h utils.h
	gcc -c hash.c

attivita.o: attivita.c attivita.h utils.h
	gcc -c attivita.c

utils.o: utils.c utils.h
	gcc -c utils.c

clean:
	rm -f *.o *.exe