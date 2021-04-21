CC=cc
CFLAGS=-O2 -std=c99 -Wall -Wextra
DEBUG=false
BIN=per
CSRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif

$(BIN): $(CSRC) $(HEADERS)
	$(CC) -o $(BIN) $(CFLAGS) $(CSRC) 

clean: 
ifneq (, $(wildcard ./per))
	rm $(BIN)
else
	@echo "Nothing to clear."
endif
