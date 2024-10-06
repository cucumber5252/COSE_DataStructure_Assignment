////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	void *dataPtr;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	int		count;
	NODE	*root;
	int		(*compare)(const void *, const void *); 
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( int (*compare)(const void *, const void *));

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree, void (*callback)(void *));

/* Inserts new data into the tree
	callback은 이미 트리에 존재하는 데이터를 발견했을 때 호출하는 함수
	return	0 overflow
			1 success
			2 if duplicated key
*/
int BST_Insert( TREE *pTree, void *dataInPtr, void (*callback)(void *));

/* Deletes a node with keyPtr from the tree
	return	address of data of the node containing the key
			NULL not found
*/
void *BST_Delete( TREE *pTree, void *keyPtr);

/* Retrieve tree for the node containing the requested key (keyPtr)
	return	address of data of the node containing the key
			NULL not found
*/
void *BST_Search( TREE *pTree, void *keyPtr);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree, void (*callback)(const void *));

/* prints tree using right-to-left inorder traversal
*/
void BST_TraverseR( TREE *pTree, void (*callback)(const void *));

/* Print tree using right-to-left inorder traversal with level
*/
void printTree( TREE *pTree, void (*callback)(const void *));

/* returns number of nodes in tree
*/
int BST_Count( TREE *pTree);

