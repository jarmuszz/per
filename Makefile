CC=cc
CFLAGS=-O2 -std=c99 -Wall
DEBUG=false
BIN=per

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	CFLAGS += -lbsd
endif

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif

$(BIN): per.c
	$(CC) -o $(BIN) $(CFLAGS) $^

clean:
	rm $(BIN)
