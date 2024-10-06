CC = gcc

.c.o: 
	$(CC) -c $<

all: run_int_heap run_word_heap

run_int_heap: run_int_heap.o adt_heap.o
	$(CC) -o $@ run_int_heap.o adt_heap.o

run_word_heap: run_word_heap.o adt_heap.o
	$(CC) -o $@ run_word_heap.o adt_heap.o
clean:
	rm -f *.o
	rm -f run_int_heap
	rm -f run_word_heap
