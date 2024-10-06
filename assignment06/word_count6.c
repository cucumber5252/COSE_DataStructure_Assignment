#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#include "bst.h"

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define TREE_PRINT		4
#define SEARCH			5
#define DELETE			6
#define COUNT			7

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
tWord *createWord( char *word){
    tWord *newWord = (tWord *)malloc(sizeof(tWord));
    if (!newWord) return NULL;
    newWord->word = strdup(word);
    newWord->freq = 1;
    return newWord;
}


// 단어 구조체에 할당된 메모리를 해제
// for destroyList function
void destroyWord( void *pNode){
    tWord *wordNode = (tWord *)pNode;
    if (wordNode->word) free(wordNode->word); 
    free(wordNode);
}

////////////////////////////////////////////////////////////////////////////////
// gets user's input
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'P':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'T':
			return TREE_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

// compares two words in word structures
// for BST_Create function
// 정렬 기준 : 단어
int compare_by_word( const void *n1, const void *n2)
{
	tWord *p1 = (tWord *)n1;
	tWord *p2 = (tWord *)n2;
	
	return strcmp( p1->word, p2->word);
}

// prints contents of word structure
// for BST_Traverse and BST_wTraverseR functions
void print_word(const void *dataPtr)
{
	printf( "%s\t%d\n", ((tWord *)dataPtr)->word, ((tWord *)dataPtr)->freq);
}

// prints word of word structure
// for printTree function
void print_word_only(const void *dataPtr)
{
	printf( "%s\n", ((tWord *)dataPtr)->word);
}

void increase_freq(void *dataPtr)
{
	((tWord *)dataPtr)->freq++;
}

// gets user's input
void input_word(char *word)
{
	fprintf( stderr, "Input a word to find: ");
	fscanf( stdin, "%s", word);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	
	char word[100];
	tWord *pWord;
	int ret;
	FILE *fp;
	
	if (argc != 2) {
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty tree
	tree = BST_Create(compare_by_word);
	if (!tree)
	{
		printf( "Cannot create a tree\n");
		return 100;
	}
	
	while(fscanf( fp, "%s", word) != EOF)
	{
		pWord = createWord( word);
		
		ret = BST_Insert( tree, pWord, increase_freq);
		
		if (ret == 0 || ret == 2) // failure or duplicated
		{
			destroyWord( pWord);
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, T)ree print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		void *ptr;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				BST_Destroy( tree, destroyWord);
				return 0;
			
			case FORWARD_PRINT:
				BST_Traverse( tree, print_word);
				break;
			
			case BACKWARD_PRINT:
				BST_TraverseR( tree, print_word);
				break;
			
			case TREE_PRINT:
				printTree( tree, print_word_only);
				break;
				
			case SEARCH:
				input_word(word);
				
				pWord = createWord( word);

				if ((ptr = BST_Search( tree, pWord)) != NULL) print_word( ptr);
				else fprintf( stdout, "%s not found\n", word);
				
				destroyWord( pWord);
				break;
				
			case DELETE:
				input_word(word);
				
				pWord = createWord( word);

				if ((ptr = BST_Delete( tree, pWord)) != NULL)
				{
					fprintf( stdout, "(%s, %d) deleted\n", ((tWord *)ptr)->word, ((tWord *)ptr)->freq);
					destroyWord( ptr);
				}
				else fprintf( stdout, "%s not found\n", word);
				
				destroyWord( pWord);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", BST_Count(tree));
				break;
		}
		
		if (action) fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, T)ree print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}


