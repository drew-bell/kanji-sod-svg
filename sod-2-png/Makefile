PROGRAM_NAME = sod2png
CC = gcc
CCFLAGS = -Wall -g
CFLAGS = -I/usr/include/libxml2 -I/usr/local/include/cairo
LDFLAGS = -lxml2 -lcairo -lsvg-cairo

all: sod2png

sod2png: main.o arg.o editing.o svg2png.o types.o
	${CC} ${CCFLAGS} ${CFLAGS} ${LDFLAGS} -o ${PROGRAM_NAME} main.o arg.o editing.o svg2png.o types.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf *.o sod2png