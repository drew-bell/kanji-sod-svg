PROGRAM_NAME = sod2png
CC = gcc
CCFLAGS = -Wall -g
CFLAGS = -I/usr/include/libxml2 -I/usr/local/include/cairo
LDFLAGS = -lxml2 -lcairo -lsvg-cairo
srcdir = .
all: sod2png

sod2png: $(srcdir)/main.o \
	$(srcdir)/arg.o \
	$(srcdir)/editing.o \
	$(srcdir)/svg2png.o \
	$(srcdir)/types.o
	${CC} ${CCFLAGS} ${CFLAGS} ${LDFLAGS} --std=c99 -ggdb -o ${PROGRAM_NAME} main.o arg.o editing.o svg2png.o types.o

.c.o:
	$(CC) $(CFLAGS) --std=c99 -ggdb -c $< -o $@

clean: 
	rm -rf *.o sod2png

