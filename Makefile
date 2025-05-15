EX=./examples
INC=./include
SRC=./src
PPM_BIN=ppm


all: bargp.o ex

ex: ppm

ppm: $(EX)/ppm.c
	cc bargp.o -I$(INC) $(EX)/ppm.c -o $(PPM_BIN)

bargp.o: $(INC)/bargp.h $(SRC)/bargp.c
	cc -c $(SRC)/bargp.c

clean:
	rm -rf *.o $(PPM_BIN)
