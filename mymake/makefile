OBJ = main.o resinfo.o conffile.o resfunctions.o rrdtool.o args.o log.o mainfunc.o
WOBJ = resweb.o resinfo.o conffile.o log.o
CC  = gcc
FLAGS = -Wall -g
CFLAGS = $(FLAGS) -c

all: resources resweb

resources: $(OBJ)
	$(CC) $(FLAGS) $^ -o $@

resweb: $(WOBJ)
	$(CC) $(FLAGS) $^ -o $@

resweb.o: ../resweb.c ../resinfo.h ../conffile.h ../log.h
	$(CC) $(CFLAGS) $< -o $@

main.o: ../main.c ../resinfo.h ../conffile.h ../args.h ../log.h ../mainfunc.h
	$(CC) $(CFLAGS) $< -o $@

mainfunc.o: ../mainfunc.c ../rrdtool.h ../resinfo.h ../conffile.h ../resfunctions.h ../args.h ../log.h 
	$(CC) $(CFLAGS) $< -o $@

resinfo.o: ../resinfo.c ../resinfo.h 
	$(CC) $(CFLAGS) $< -o $@

resfunctions.o: ../resfunctions.c ../resfunctions.h ../resinfo.h 
	$(CC) $(CFLAGS) $< -o $@

conffile.o: ../conffile.c ../conffile.h ../resinfo.h
	$(CC) $(CFLAGS) $< -o $@

rrdtool.o: ../rrdtool.c ../rrdtool.h 
	$(CC) $(CFLAGS) $< -o $@

args.o: ../args.c ../args.h 
	$(CC) $(CFLAGS) $< -o $@

log.o: ../log.c ../log.h 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
