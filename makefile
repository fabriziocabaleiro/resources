OBJ= \
	objs/main.o \
	objs/resinfo.o \
	objs/conffile.o \
	objs/resfunctions.o \
	objs/rrdtool.o \
	objs/args.o \
	objs/log.o \
	objs/mainfunc.o

WOBJ= \
	objs/resweb.o \
	objs/resinfo.o \
	objs/conffile.o \
	objs/log.o

CC     = gcc
FLAGS  = -Wall -g
CFLAGS = $(FLAGS) -c
VERSION=$(shell git tag|tail -n1)

all: resources resweb

resources: $(OBJ)
	$(CC) $(FLAGS) $^ -o $@

resweb: $(WOBJ)
	$(CC) $(FLAGS) $^ -o $@

objs/resweb.o: resweb.c resinfo.h conffile.h log.h
	$(CC) $(CFLAGS) $< -o $@

objs/main.o: main.c resinfo.h conffile.h args.h log.h mainfunc.h
	$(CC) $(CFLAGS) $< -o $@

objs/mainfunc.o: mainfunc.c rrdtool.h resinfo.h conffile.h resfunctions.h args.h log.h 
	$(CC) $(CFLAGS) -DVERSION=\"$(VERSION)\" $< -o $@

objs/resinfo.o: resinfo.c resinfo.h args.h
	$(CC) $(CFLAGS) $< -o $@

objs/resfunctions.o: resfunctions.c resfunctions.h resinfo.h 
	$(CC) $(CFLAGS) $< -o $@

objs/conffile.o: conffile.c conffile.h resinfo.h
	$(CC) $(CFLAGS) $< -o $@

objs/rrdtool.o: rrdtool.c rrdtool.h 
	$(CC) $(CFLAGS) $< -o $@

objs/args.o: args.c args.h log.h
	$(CC) $(CFLAGS) $< -o $@

objs/log.o: log.c log.h 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm objs/*.o
	rm resources resweb
