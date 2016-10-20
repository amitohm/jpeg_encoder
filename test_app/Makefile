# Makefile for Saliency using libsaliency.a

.IGNORE:

.KEEP_STATE:

.SUFFIXES: .o .c

.c.o: 
	$(CC) $(CCFLAGS) -c $< -o $@

CC = gcc

CCFLAGS += -g -Wall

LIB_JPEGENC = libjpegenc.a
BIN = jpegenc

SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c=.o)

SUBDIR_JPEGENC = "../jpeg_encoder_lib"

all: $(LIB_JPEGENC) $(BIN)

$(BIN): $(OBJS) $(LIB_JPEGENC)
	$(CC) $(CCFLAGS) -o $(BIN) $(OBJS) -I . -L . $(LIB_JPEGENC)

$(LIB_JPEGENC): 
	@cd $(SUBDIR_JPEGENC) ; make
	
clean:
	rm -rf $(OBJS) $(BIN) $(LIB_JPEGENC) $(BIN).gdb
	cd $(SUBDIR_JPEGENC); make clean


