CC=cc
CFLAGS=-O2 -std=c99 -Wall
DEBUG=false
BIN=per

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif

$(BIN): per.c
	$(CC) -o $(BIN) $(CFLAGS) $^

clean:
	rm $(BIN)
