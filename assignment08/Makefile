CC = gcc

.c.o: 
	$(CC) -c $<

all: word_count7

word_count7: word_count7.o avlt.o
	$(CC) -o $@ word_count7.o avlt.o
	
clean:
	rm -f *.o
	rm -f word_count7
