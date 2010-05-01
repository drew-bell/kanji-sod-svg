PROGRAM_NAME = sod2png
CC = gcc
CCFLAGS = -Wall -g
CFLAGS = -I/usr/include/libxml2 -I/usr/local/include/cairo
LDFLAGS = -lxml2 -lcairo -lsvg-cairo
srcdir = . src/
all: sod2png

sod2png: $(srcdir)/main.o \
	$(srcdir)/arg.o \
	$(srcdir)/editing.o \
	$(srcdir)/svg2png.o \
	$(srcdir)/types.o
	${CC} ${CCFLAGS} ${CFLAGS} ${LDFLAGS} --std=c99 -o ${PROGRAM_NAME} src/main.o src/arg.o src/editing.o src/svg2png.o src/types.o

.c.o:
	$(CC) $(CFLAGS) --std=c99 -c $< -o $@

clean: 
	rm -rf src/*.o src/sod2png

