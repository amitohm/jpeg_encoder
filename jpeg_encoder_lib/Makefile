# Makefile for saliency_lib/source

.IGNORE:

.KEEP_STATE:

.SUFFIXES: .o .c

.c.o: 
	$(CC) $(CCFLAGS) -c $< -o $@
.s.o: 
	$(AS) $< -o $@

CROSS = 

CC = $(CROSS)gcc
AS = $(CROSS)as
AR = $(CROSS)ar
RANLIB = $(CROSS)ranlib

CCFLAGS += -g -Wall

UPDATE_OFFSETS = x

LIB = libjpegenc.a

SRCS = $(wildcard *.c) 
ASM_SRCS = $(wildcard *.s)
OBJS = $(SRCS:.c=.o) $(ASM_SRCS:.s=.o)

all: $(LIB)


$(LIB):	$(OBJS)
	$(AR) rcu ../test_app/$@ $(OBJS)
	$(RANLIB) ../test_app/$@

clean:
	rm -rf $(OBJS)


