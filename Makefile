CC ?= gcc
LDLIBS = -lusb-1.0

prefix ?= /usr/local
exec_prefix = $(prefix)

.PHONY: all
all: annecolor
annecolor: annecolor.o
annecolor.o: annecolor.c

.PHONY: clean
clean:
	rm -f annecolor annecolor.o

.PHONY: install
install:
	install annecolor $(exec_prefix)/bin

.PHONY: uninstall
uninstall:
	rm -f $(exec_prefix)/bin/annecolor
