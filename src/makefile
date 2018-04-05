###################################################
#
# Makefile for FW
# Creator [Xcode -> Makefile Ver: Feb 14 2007 09:18:41]
# Created: [Fri Jul 25 10:28:37 2014]
#
###################################################

CC=g++
CF=-I/cvmfs/opt.usherbrooke.ca/CentOS6/boost64/1.61.0/include/ -O3

all :: FW moveO

FW: main.o Group.o Consumer.o Param.o Detritus.o Producer.o Microbes.o
	$(CC) -L/usr/local/lib -o FW main.o Group.o Consumer.o Param.o Detritus.o Producer.o Microbes.o

clean  : mrproper
		rm \
		./main.o\
		./Consumer.o\
		./Group.o\
		./Param.o\
		./Detritus.o\
		./Producer.o\
		./Microbes.o\
		FW

mrproper: clean
	rm FW

moveO : FW
	mkdir -p obj
	mv *.o obj

main.o: main.cpp Group.h Consumer.h Communities.h Param.h Detritus.o Producer.o Microbes.o
	$(CC) $(CF) -c -o main.o main.cpp

Group.o: Group.cpp Group.h
	$(CC) $(CF) -c -o Group.o Group.cpp

Consumer.o: Consumer.cpp Consumer.h Group.h
	$(CC) $(CF) -c -o Consumer.o Consumer.cpp

Detritus.o: Detritus.cpp Detritus.h Group.h
	$(CC) $(CF) -c -o Detritus.o Detritus.cpp

Producer.o: Producer.cpp Producer.h Group.h
	$(CC) $(CF) -c -o Producer.o Producer.cpp

Microbes.o: Microbes.cpp Microbes.h Group.h
	$(CC) $(CF) -c -o Microbes.o Microbes.cpp
