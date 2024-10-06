CC = gcc

.c.o: 
	$(CC) -c $<

all: word_count6

word_count6: word_count6.o bst.o
	$(CC) -o $@ word_count6.o bst.o
	
clean:
	rm -f *.o
	rm -f word_count6
