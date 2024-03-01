CFLAGS = -Wall

SRC = ./src/
BIN = ./bin/
OBJ = ./bin/obj/
DATA_STRUCT_SRC = ./c_libs/source/
ARGS = ""

.PHONY: clean run_translator run_csquare data_structs
.ONESHELL: data_structs

build: build_translator build_csquare

reader:
	gcc $(CFLAGS) -o $(OBJ)reader.o $(SRC)csqr_reader.c -c

utils:
	gcc $(CFLAGS) -o $(OBJ)utils.o $(SRC)csqr_utils.c -c

data_struct:
	gcc $(CFLAGS) -o $(OBJ)avl.o $(DATA_STRUCT_SRC)avl.c -c
	gcc $(CFLAGS) -o $(OBJ)binary_heap.o $(DATA_STRUCT_SRC)binary_heap.c -c
	gcc $(CFLAGS) -o $(OBJ)queue.o $(DATA_STRUCT_SRC)queue.c -c
	gcc $(CFLAGS) -o $(OBJ)stack.o $(DATA_STRUCT_SRC)stack.c -c
	gcc $(CFLAGS) -o $(OBJ)utils.o $(DATA_STRUCT_SRC)utils.c -c
	gcc $(CFLAGS) -o $(OBJ)vector.o $(DATA_STRUCT_SRC)vector.c -c

build_translator: reader
	gcc $(CFLAGS) -o $(BIN)translator $(SRC)translator.c $(OBJ)reader.o

build_csquare: reader utils
	gcc $(CFLAGS) -o $(BIN)csquare $(SRC)csquare.c $(OBJ)reader.o $(OBJ)utils.o

run_translator: build_translator
	$(BIN)translator $(ARGS)

run_csquare: build_csquare
	$(BIN)csquare $(ARGS)

clean:
	rm -f $(BIN)csquare
	rm -f $(BIN)translator
	rm -f $(OBJ)*