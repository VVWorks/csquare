CFLAGS = -Wall

INC = ./include/
SRC = ./src/
BIN = ./bin/
OBJ = ./bin/obj/
ARGS = ""

build: build_translator build_csquare

reader:
	gcc $(CFLAGS) -o $(OBJ)reader.o $(SRC)csqr_reader.c -c

build_translator: reader
	gcc $(CFLAGS) -o $(BIN)translator $(SRC)translator.c $(OBJ)reader.o

build_csquare: reader
	gcc $(CFLAGS) -o $(BIN)csquare $(SRC)csquare.c $(OBJ)reader.o

.PHONY: clean run_translator run_csquare

run_translator: build_translator
	$(BIN)translator $(ARGS)

run_csquare: build_csquare
	$(BIN)csquare $(ARGS)

clean:
	rm -f /bin/csquare
	rm -f /bin/translator