CC=gcc
CFLAGS=-std=c17 -pedantic -Wall -Wvla -Werror  -Wno-unused-variable -Wno-unused-but-set-variable -D_DEFAULT_SOURCE
ZOMBIE=zombie/zombie
CONTROLLER=controller/controller
LABO=labo/labo
ALL=$(ZOMBIE) $(CONTROLLER) $(LABO) 

all: zombie controller labo

zombie: $(ZOMBIE).o utils.o
	$(CC) $(CFLAGS) -o $(ZOMBIE) $(ZOMBIE).o utils.o 

$(ZOMBIE).o: $(ZOMBIE).c utils.h
	$(CC) $(CFLAGS) -c $(ZOMBIE).c -o $(ZOMBIE).o

controller: $(CONTROLLER).o utils.o
	$(CC) $(CFLAGS) -o $(CONTROLLER) $(CONTROLLER).o utils.o

$(CONTROLLER).o: $(CONTROLLER).c utils.h
	$(CC) $(CFLAGS) -c $(CONTROLLER).c -o $(CONTROLLER).o

labo: $(LABO).o utils.o
	$(CC) $(CFLAGS) -o $(LABO) $(LABO).o utils.o

$(LABO).o: $(LABO).c utils.h
	$(CC) $(CFLAGS) -c $(LABO).c -o $(LABO).o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean: 
	rm -f *.o $(ZOMBIE).o $(CONTROLLER).o $(LABO).o $(ALL)

clear:
	clear
