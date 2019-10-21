APP = lpthreads.c
APPPL = packageList.c
APPSC = scheduler.c
OBJ =  lpthreads.o
OBJPL =  packageList.o
OBJSC =  scheduler.o
BIB_E = liblpthreads.a
CC = gcc
LD = ld
AR = ar
RANLIB = ranlib
NM = nm


all: $(BIB_E) clean

$(BIB_E) : $(OBJ)
	$(AR) rv $(BIB_E) $(OBJ) $(OBJSC) $(OBJPL)
	$(RANLIB) $(BIB_E)
	$(NM) -s $(BIB_E)

$(OBJ) : 
	$(CC) -c $(APP) -lm
	$(CC) -c $(APPSC)
	$(CC) -c $(APPPL)

clean : 
	rm $(OBJ) $(OBJSC) $(OBJPL)