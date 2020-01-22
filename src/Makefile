# Makefile

CC = gcc
CC_FLAGS = -Wall -pedantic

LIB_FILES = ../Lib/nralloc.c ../Lib/nrarith.c ../Lib/nrio.c
LIB_OBJ = ../Lib/nralloc.o ../Lib/nrarith.o ../Lib/nrio.o

all: indexation extractor

indexation: indexation.o $(LIB_OBJ)
	$(CC) $(CC_FLAGS) indexation.o $(LIB_OBJ) -o indexation.out

extractor: feature_extractor_test.o distance.o feature_extractor.o convolution.o $(LIB_OBJ)
	$(CC) $(CC_FLAGS) feature_extractor_test.o distance.o feature_extractor.o convolution.o $(LIB_OBJ) -o extractor.out -lm

feature_extractor_test.o: feature_extractor_test.c
	$(CC) -o feature_extractor_test.o -c feature_extractor_test.c

conv: conv_main.c convolution.o
	$(CC) -w convolution.o $(LIB_OBJ) conv_main.c -lm -o convolution.out

indexation.o: indexation.c indexation.h ../Lib/def.h ../Lib/nralloc.h ../Lib/nrarith.h ../Lib/nrio.h
	$(CC) -c indexation.c -o indexation.o

distance.o: distance.c distance.h
	$(CC) -c distance.c -o distance.o -lm

../Lib/%.o: ../Lib/%.c ../Lib/%.h -w -lm
	@g++ -c $< -o $@

convolution.o: convolution.c convolution.h ../Lib/def.h ../Lib/nralloc.h ../Lib/nrarith.h ../Lib/nrio.h
	$(CC) -c convolution.c -o convolution.o

occi: occi.cpp
	g++-4.8 -o occi.out occi.cpp -I /opt/oracle/instantclient_12_2/sdk/include -locci -lnnz12 -lclntsh

soci: soci.cpp
	g++ -o soci.out soci.cpp -lsoci_core -lsoci_oracle -ldl -lclntsh -lnnz12

clean: clean_object
	rm -rfv mk.tmp
	rm -rfv *.out

clean_object:
	rm -rfv *.o

clean_lib:
	rm -rfv ../Lib/*.o
