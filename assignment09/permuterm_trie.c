#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
TRIE *trieCreateNode(void);
void trieDestroy(TRIE *root);
int trieInsert(TRIE *root, char *str, int dic_index);
int trieSearch(TRIE *root, char *str);
void trieList(TRIE *root, char *dic[]);
static int trieList_main(TRIE *root, char *dic[], int count);
void triePrefixList(TRIE *root, char *str, char *dic[]);
int make_permuterms(char *str, char *permuterms[]);
void clear_permuterms(char *permuterms[], int size);
void trieSearchWildcard(TRIE *root, char *str, char *dic[]);

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void) {
    TRIE *newNode = (TRIE *)malloc(sizeof(TRIE));
    if (!newNode) return NULL;

    newNode->index = -1;
    for (int i = 0; i < MAX_DEGREE; i++) {
        newNode->subtrees[i] = NULL;
    }

    return newNode;
}

/* Deletes all data in trie and recycles memory
*/
/* Deletes all data in trie and recycles memory
*/
void trieDestroy(TRIE *root) {
    if (!root) return;


    for (int i = 0; i < MAX_DEGREE; i++) {
        if (root->subtrees[i] != NULL) {
            trieDestroy(root->subtrees[i]);
        }
    }

    free(root);
}


/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert(TRIE *root, char *str, int dic_index) {
    if (!root || !str) return 0;

    TRIE *current = root;
    TRIE *newNode = NULL;
    char *ptr = str;

    while (*ptr) {
        char ch = *ptr;
        if (isupper(ch)) ch = tolower(ch);
        if (!islower(ch) && ch != EOW) return 0; 

        int index = getIndex(ch);
        if (current->subtrees[index] == NULL) {
            newNode = trieCreateNode();
            if (!newNode) {
                trieDestroy(root); 
                return 0;
            }
            current->subtrees[index] = newNode;
        }
        current = current->subtrees[index];
        ptr++;
    }

    if (current->index != -1) return 0; // 중복 삽입 방지
    current->index = dic_index;

    return 1;
}




/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch(TRIE *root, char *str) {
    if (!root || !str) return -1;

    TRIE *current = root;
    char *ptr = str;

    while (*ptr) {
        if (isupper(*ptr)) *ptr = tolower(*ptr);
        if (!islower(*ptr) && *ptr != EOW) return -1;

        int index = getIndex(*ptr);

        if (current->subtrees[index] == NULL) return -1;
        current = current->subtrees[index];
        ptr++;
    }

    int index = getIndex(EOW);
    if (current->subtrees[index] != NULL) {
        current = current->subtrees[index];
    } else {
        return -1;
    }

    return current->index;
}



/* prints all entries in trie using preorder traversal
*/

static int trieList_main(TRIE *root, char *dic[], int count) {
    if (!root) return count;

    if (root->index != -1) {
        printf("[%i]%s\n", count +1, dic[root->index]);
        count++;
    }

    for (int i = 0; i < MAX_DEGREE; i++) {
        count = trieList_main(root->subtrees[i], dic, count);
    }

    return count;
}

/* prints all entries starting with str (as prefix) in trie
	ex) "ab" -> "abandoned", "abandoning", "abandonment", "abased", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]){
    if (!root || !str) return;

    TRIE *current = root;
    char *ptr = str;
    
    while (*ptr) {
        int index = getIndex(*ptr);
        if (current->subtrees[index] == NULL) return;
        current = current->subtrees[index];
        ptr++;
    }

    trieList(current, dic);
}

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms(char *str, char *permuterms[]) {
    int len = strlen(str);
    char buffer[len + 2];
    strcpy(buffer, str);
    buffer[len] = EOW;
    buffer[len + 1] = '\0';
    int count = 0;

    for (int i = 0; i <= len; i++) {
        permuterms[count] = (char *)malloc(len + 2);
        if (permuterms[count] == NULL) {
            clear_permuterms(permuterms, count);
            return 0;
        }
        strcpy(permuterms[count], buffer + i);
        strncat(permuterms[count], buffer, i);
        count++;
    }

    return count;
}



/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size){
    for (int i = 0; i < size; i++) {
        free(permuterms[i]);
        permuterms[i] = NULL;
    }
}

/* wildcard search
    ex) "*a*b" -> "abandoning", "abasement", "abatement", ...
*/
void trieSearchWildcard(TRIE *root, char *str, char *dic[]) {
    int len = strlen(str);
    char *buffer = (char *)malloc((len + 2) * sizeof(char));
    char *tempStr = (char *)malloc((len + 2) * sizeof(char));

    for (int i = 0; i < len; i++) {
        tempStr[i] = tolower(str[i]);
    }
    
    tempStr[len] = '\0';

    if (tempStr[0] == '*' && tempStr[len - 1] == '*') {
        strncpy(buffer, tempStr + 1, len - 2);
        buffer[len - 2] = '\0';
        triePrefixList(root, buffer, dic);
    } else if (tempStr[0] == '*' && tempStr[len - 1] != '*') {
        strcpy(buffer, tempStr + 1);
        int buflen = strlen(buffer);
        buffer[buflen] = EOW;
        buffer[buflen + 1] = '\0';
        triePrefixList(root, buffer, dic);
    } else if (tempStr[0] != '*' && tempStr[len - 1] == '*') {
        tempStr[len-1] = '\0';
        buffer[0] = EOW;
        strcpy(buffer + 1, tempStr);
        triePrefixList(root, buffer, dic);
    } else {
        char *asterisk = strchr(tempStr, '*');
        if (asterisk != NULL) {
            int prefixLen = asterisk - tempStr;
            int postfixLen = len - prefixLen - 1;
            char prefix[prefixLen + 1];
            char postfix[postfixLen + 1];

            strncpy(prefix, tempStr, prefixLen);
            prefix[prefixLen] = '\0';
            strncpy(postfix, asterisk + 1, postfixLen);
            postfix[postfixLen] = '\0';

            if (strlen(prefix) > 0 && strlen(postfix) > 0) {
                strcpy(buffer, postfix);
                strcat(buffer, "$");
                strcat(buffer, prefix);
                triePrefixList(root, buffer, dic);
            } else if (strlen(prefix) > 0) {
                strcpy(buffer, prefix);
                triePrefixList(root, buffer, dic);
            } else if (strlen(postfix) > 0) {
                strcpy(buffer, postfix);
                strcat(buffer, "$");
                triePrefixList(root, buffer, dic);
            }
        } else {
            triePrefixList(root, tempStr, dic);
        }
    }

    free(buffer);
    free(tempStr);
}


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *permute_trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p; // # of permuterms
	int num_words = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	while (fscanf( fp, "%s", str) != EOF)
	{	
		num_p = make_permuterms( str, permuterms);
		
		for (int i = 0; i < num_p; i++)
			trieInsert( permute_trie, permuterms[i], num_words);
		
		clear_permuterms( permuterms, num_p);
		
		dic[num_words++] = strdup( str);
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search term
		if (strchr( str, '*')) 
		{
			trieSearchWildcard( permute_trie, str, dic);
		}
		// keyword search
		else 
		{
			ret = trieSearch( permute_trie, str);
			
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		printf( "\nQuery: ");
	}

    for (int i = 0; i < num_words; i++) {
        free(dic[i]);
    }

    trieDestroy(permute_trie);

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
void trieList( TRIE *root, char *dic[])
{
	if (root == NULL) return;

	trieList_main(root, dic, 0);
}

