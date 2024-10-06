
////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	void		*dataPtr;
	struct node	*llink;
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*head;
	NODE	*rear;
	int		(*compare)(const void *, const void *); // used in _search function
} LIST;

////////////////////////////////////////////////////////////////////////////////
// function declarations

// Allocates dynamic memory for a list head node and returns its address to caller
// return	head node pointer
// 			NULL if overflow
LIST *createList( int (*compare)(const void *, const void *));

//  이름 리스트에 할당된 메모리를 해제 (head node, data node)
void destroyList( LIST *pList, void (*callback)(void *));

// Inserts data into list
// callback은 이미 리스트에 존재하는 데이터를 발견했을 때 호출하는 함수
//	return	0 if overflow
//			1 if successful
//			2 if duplicated key
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *));

// Removes data from list
//	return	0 not found
//			1 deleted
int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr);

// interface to search function
//	pArgu	key being sought
//	dataOutPtr	contains found data
//	return	1 successful
//			0 not found
int searchNode( LIST *pList, void *pArgu, void **dataOutPtr);

// returns number of nodes in list
int countList( LIST *pList);

// returns	1 empty
//			0 list has data
int emptyList( LIST *pList);

// traverses data from list (forward)
void traverseList( LIST *pList, void (*callback)(const void *));

// traverses data from list (backward)
void traverseListR( LIST *pList, void (*callback)(const void *));
