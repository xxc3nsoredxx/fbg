CC = gcc
CFLAGS = -c -Wall -Wextra -std=c89 --pedantic-errors
AR = ar
AFLAGS = rcs
BIN = bin
SRC = src
OBJS = $(BIN)/main.o
LIBOBJS = $(BIN)/fbg.o
LIB = $(BIN)/libfbg.a
LFLAGS = -Lbin -lfbg

all: $(BIN)/test

$(BIN)/test: $(OBJS) $(LIB)
	$(CC) $(OBJS) $(LFLAGS) -o $(BIN)/test

$(BIN)/main.o: $(SRC)/main.c $(SRC)/fbg/screen.h
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(BIN)/main.o

$(BIN)/libfbg.a: $(LIBOBJS)
	$(AR) $(AFLAGS) $(BIN)/libfbg.a $(LIBOBJS)

$(BIN)/fbg.o: $(SRC)/fbg/fbg.c $(SRC)/fbg/screen.h
	$(CC) $(CFLAGS) $(SRC)/fbg/fbg.c -o $(BIN)/fbg.o

clean:
	rm $(BIN)/*
