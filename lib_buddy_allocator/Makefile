
#This next line defines arguments that are passed to all compilation steps
CCFLAGS = -g -Wall

binaryBuddy: binaryBuddy.o binaryBuddy.h
	gcc $(CCFLAGS) -o binaryBuddy binaryBuddy.o

binaryBuddy.o: binaryBuddy.c binaryBuddy.h
	gcc $(CCFLAGS) -c binaryBuddy.c

clean:
	rm -f binaryBuddy binaryBuddy.o
