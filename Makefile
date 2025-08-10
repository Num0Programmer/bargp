EX=./examples
INC=./include
SRC=./src
PPM_BIN=ppm
INSTALL_PREFIX=/usr/local


all: libbargp.o ex

install: libbargp.o
	cp $(INC)/bargp.h $(INSTALL_PREFIX)/include
	cp libbargp.o $(INSTALL_PREFIX)/lib

ex: ppm

ppm: $(EX)/ppm.c
	cc libbargp.o -I$(INC) $(EX)/ppm.c -o $(PPM_BIN)

libbargp.o: $(INC)/bargp.h $(SRC)/bargp.c
	cc -c $(SRC)/bargp.c -o libbargp.o

clean:
	rm -rf *.o $(PPM_BIN)
