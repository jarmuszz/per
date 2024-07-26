# Per installation script
#
# To Install:
# 	make
# 	sudo make install
#
# To Uninstall:
# 	sudo make uninstall

VERSION=1.0.4

CC=cc
CFLAGS=-O2 -std=c99 -Wall -Wextra
DEBUG=false
BIN=per
CSRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)
DOC=per.1
DESTDIR=
MANDEST=$(DESTDIR)/usr/share/man/man1
BINDEST=$(DESTDIR)/usr/bin

ifeq ($(DEBUG), true)
	CFLAGS += -g
endif

.PHONY: install install-doc install-bin uninstall build clean tarball

# Default
build: $(BIN)

# Tarball
TARBALL=$(BIN)-$(VERSION).tar.gz
TARBALL_BUILD_PARENT=/tmp/per
TARBALL_BUILD=$(TARBALL_BUILD_PARENT)/$(BIN)-$(VERSION)
tarball: clean
# Fixes awkward GNU Tar behavior
ifneq ($(shell tar --version 2>&1 | grep gnu),)
	TARFLAGS=--exclude=$(TARBALL)
endif
	mkdir -p $(TARBALL_BUILD)
	cp * $(TARBALL_BUILD)/
	cd $(TARBALL_BUILD_PARENT); tar -czvf $(TARBALL) $(TARFLAGS) $(shell basename $(TARBALL_BUILD))
	mv $(TARBALL_BUILD_PARENT)/$(TARBALL) .
	rm -r $(TARBALL_BUILD_PARENT)

# Installing/Uninstalling
install: build install-bin install-doc

install-doc: $(DOC)
	mkdir -p $(MANDEST)
	cp $(DOC) $(MANDEST)
	cd $(MANDEST); gzip $(DOC)

install-bin: $(BIN)
	mkdir -p $(BINDEST)
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
