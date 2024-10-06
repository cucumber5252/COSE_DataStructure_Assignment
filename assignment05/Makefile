CC = gcc

.c.o: 
	$(CC) -c $<

all: word_count5

word_count5: word_count5.o adt_dlist.o
	$(CC) -o $@ word_count5.o adt_dlist.o
	
clean:
	rm -f *.o
	rm -f word_count5
