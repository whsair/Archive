SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)
ALL := $(BINS) showbytes32 showbytes64 sizeof32 sizeof64
CC := gcc

CFLAGS = -Wall -Og -g
.PHONY:all

all: $(ALL)

% : %.c
	$(CC) $(CFLAGS) -o $@ $<

showbytes32: showbytes.c
	$(CC) $(CFLAGS) -m32 -o $@ $<

showbytes64: showbytes.c
	$(CC) $(CFLAGS) -m64 -o $@ $<

sizeof32: sizeof.c
	$(CC) $(CFLAGS) -m32 -o $@ $<

sizeof64: sizeof.c
	$(CC) $(CFLAGS) -m64 -o $@ $<

clean:
	rm -f $(ALL) *~ *.o
