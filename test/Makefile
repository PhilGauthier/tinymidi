CC = gcc
CFLAGS = -Wall -pedantic -g -pg
PROJDIR = `dirname $(CURDIR)`
LIBDIR = "$(PROJDIR)/.libs"
LFLAGS = -L$(LIBDIR) -ltinymidi
INCLUDE = "$(PROJDIR)/include"
TESTS := $(wildcard *.c)


all: $(TESTS)
	echo $(LIBDIR)
	@$(foreach file, $^, \
			echo $(CC) $(CFLAGS) -I $(INCLUDE) $(file) $(LFLAGS) -o `basename $(file) .c`)

clean: $(TESTS)
	@$(foreach file, $^, \
		rm -v $(CURDIR)/`basename $(file) .c`)
