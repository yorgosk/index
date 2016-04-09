CC=gcc
CFLAGS=-Wall
OBJECTS=AirportManagement.o Evretirio.o TSDDA.o TSEvr.o ./BST/ch8_BSTpointerR.o ./AVL/ch8_AVLpointer.o

index: $(OBJECTS)
	$(CC) $(CFLAGS) -o index $(OBJECTS)

AirportManagement.o: AirportManagement.c
	$(CC) $(CFLAGS) -c AirportManagement.c
Evretirio.o: Evretirio.c
	$(CC) $(CFLAGS) -c Evretirio.c
TSDDA.o: TSDDA.c
	$(CC) $(CFLAGS) -c TSDDA.c
TSEvr.o: TSEvr.c
	$(CC) $(CFLAGS) -c TSEvr.c

ch8_BSTpointerR.o: ./BST/ch8_BSTpointerR.c
	$(CC) $(CFLAGS) -c ./BST/ch8_BSTpointerR.c

ch8_AVLpointer.o: ./AVL/ch8_AVLpointer.c
	$(CC) $(CFLAGS) -c ./AVL/ch8_AVLpointer.c

.PHONY: clean

clean:
	rm -f index AirportManagement.o Evretirio.o TSDDA.o TSEvr.o ./BST/ch8_BSTpointerR.o ./AVL/ch8_AVLpointer.o
