#
.SUFFIXES: .c .o .h

CC = gcc
CFLAGS = -std=c11 -Wall -pedantic 

TARGETS := em-parse em-dump #em-stats
OBJS := em5-parser.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGETS) $(OBJS)  #do not delete on error
.PHONY: all clean


all: $(TARGETS)

em-parse: em-parse.o $(OBJS)
	$(CC) $(CFLAGS)  $^ -o $@

em-dump: em-dump.o $(OBJS)
	$(CC) $(CFLAGS)  $^ -o $@


clean:
	-rm -f *.o
	-rm -f $(TARGETS)
