#include <stdio.h>
#include <string.h> // strdup
#include <stdlib.h>
#include "adt_heap.h"

// User structure type definition
// 단어 구조체
typedef struct {
	char	*word;		// 단어
	int		freq;		// 빈도
} tWord;

////////////////////////////////////////////////////////////////////////////////
// 단어 구조체를 위한 메모리를 할당하고 word, freq 초기화
// return	할당된 단어 구조체에 대한 pointer
//			NULL if overflow
tWord *createWord( char *word, int freq)
{
	tWord *newWord = malloc( sizeof( tWord));
	
	if (newWord == NULL) return NULL;
	
	newWord->word = strdup( word);
	newWord->freq = freq;
	
	return newWord;
}

////////////////////////////////////////////////////////////////////////////////
// 단어 구조체에 할당된 메모리를 해제
void destroyWord( void *pWord)
{
	free( ((tWord *)pWord)->word);
	free( pWord);
}

////////////////////////////////////////////////////////////////////////////////
// compares two words in word structures
// 정렬 기준 : 단어
int compare_by_word( const void *n1, const void *n2)
{
	tWord *p1 = (tWord *)n1;
	tWord *p2 = (tWord *)n2;
	
	return strcmp( p1->word, p2->word);
}

////////////////////////////////////////////////////////////////////////////////
// prints contents of word structure
void print_word(const void *dataPtr)
{
	printf( "%s\t%d\n", ((tWord *)dataPtr)->word, ((tWord *)dataPtr)->freq);
}

////////////////////////////////////////////////////////////////////////////////
// prints word of word structure
void print_word_only(const void *dataPtr)
{
	printf( "%s\n", ((tWord *)dataPtr)->word);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	HEAP *heap;
	void *dataPtr;
	
	char word[100];
	int freq;
	tWord *pWord;
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
		
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		fprintf( stderr, "file open error: %s\n", argv[1]);
		return 2;
	}
	
	heap = heap_Create(compare_by_word); // initial capacity = 10
	
	printf("Insert:");
	
	while(fscanf(fp, "%s\t%d", word, &freq) != EOF)
	{
		printf(" %s", word); // 입력 단어
		pWord = createWord(word, freq);
		
		// insert function call
		if (heap_Insert(heap, pWord) == 0) {
			destroyWord(pWord);
			break;
		}
 	}
	printf("\n");
	fclose(fp);
	
	printf("Heap:  ");
	heap_Print(heap, print_word_only);
	
	printf( "Delete: ");
	
	for (int i = 0; i < 10 && !heap_Empty(heap); i++)
	{
		// delete function call
		heap_Delete(heap, &dataPtr);

		print_word(dataPtr);

		destroyWord(dataPtr);
 	}
	printf("\n");
	
	heap_Destroy(heap, destroyWord);
	
	return 0;
}
