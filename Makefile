# Per installation script
#
# To Install:
# 	make
# 	sudo make install
#
# To Uninstall:
# 	sudo make uninstall

CC=cc
CFLAGS=-O2 -std=c99 -Wall -Wextra
DEBUG=false
BIN=per
CSRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)
DOC=per.1
MANDEST=/usr/share/man/man1
BINDEST=/usr/bin

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif

# Default
build: $(BIN)

# Installing/Uninstalling
install: build install-bin install-doc

install-doc: $(DOC)
	cp $(DOC) $(MANDEST)
	gzip $(MANDEST)/$(DOC)

install-bin: $(BIN)
	cp $(BIN) $(BINDEST)

uninstall:
ifneq (, $(wildcard $(MANDEST)/$(DOC).gz))
	rm $(MANDEST)/$(DOC).gz 
endif
ifneq (, $(wildcard $(BINDEST)/$(BIN)))
	rm $(BINDEST)/$(BIN)
endif

# Building
$(BIN): $(CSRC) $(HEADERS)
	$(CC) -o $(BIN) $(CFLAGS) $(CSRC) 

# Cleaning
clean: 
ifneq (, $(wildcard $(BIN)))
	rm $(BIN)
else
	@echo "Nothing to clear."
endif

.PHONY: install install-doc install-bin uninstall build clean
