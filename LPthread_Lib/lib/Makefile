APP = lpthreads.c
OBJ =  lpthreads.o
BIB_E = liblpthreads.a
BIB_D = liblpthreads.so
CC = gcc
LD = ld
AR = ar
RANLIB = ranlib
NM = nm


all: $(BIB_E) clean

$(BIB_E) : $(OBJ)
	$(AR) rv $(BIB_E) $(OBJ)
	$(RANLIB) $(BIB_E)
	$(NM) -s $(BIB_E)

$(OBJ) :
	$(CC) -c $(APP)

clean : 
	rm $(OBJ)