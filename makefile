# gtknag
# Copyright (c) 2022 Jovan Lanik

# Makefile

NAME := gtknag

PREFIX ?= /usr/local
INSTALL ?= install

LIBS := gtk+-wayland-3.0 gtk-layer-shell-0
CFLAGS += -std=c11 -Iinclude $(shell pkg-config --cflags $(LIBS))
LDLIBS += $(shell pkg-config --libs $(LIBS))

SRC = $(wildcard *.c) 
OBJ = $(SRC:%.c=%.o)

TRASH = $(OBJ) $(NAME)

.PHONY: all clean install uninstall

all: $(NAME)

clean:
	@rm $(TRASH) | true

install:
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) $(NAME) $(DESTDIR)$(PREFIX)/bin/$(NAME)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(NAME)

$(NAME): $(OBJ)
	$(LINK.c) $(LDLIBS) $^ -o $@
