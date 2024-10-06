#define BALANCING

#include <stdlib.h> // malloc
#include <stdio.h>

#include "avlt.h"

#define max(x, y)	(((x) > (y)) ? (x) : (y))

// internal functions (not mandatory)
static NODE *_insert( NODE *root, NODE *newPtr, int (*compare)(const void *, const void *), void (*callback)(void *), int *duplicated);
static NODE *_makeNode( void *dataInPtr);
static void _destroy( NODE *root, void (*callback)(void *));
static NODE *_delete( NODE *root, void *keyPtr, void **dataOutPtr, int (*compare)(const void *, const void *));
static NODE *_search( NODE *root, void *keyPtr, int (*compare)(const void *, const void *));
static void _traverse( NODE *root, void (*callback)(const void *));
static void _traverseR( NODE *root, void (*callback)(const void *));
static void _inorder_print( NODE *root, int level, void (*callback)(const void *));
static int getHeight( NODE *root);
static NODE *rotateRight( NODE *root);
static NODE *rotateLeft( NODE *root);


TREE *AVLT_Create( int (*compare)(const void *, const void *)) {
    TREE *tree = (TREE *)malloc(sizeof(TREE));
    if (tree) {
        tree->root = NULL;
        tree->count = 0;
        tree->compare = compare;
    }
    return tree;
}

void AVLT_Destroy( TREE *pTree, void (*callback)(void *)) {
    if (pTree) {
        _destroy(pTree->root, callback);
        free(pTree);
    }
}

int AVLT_Insert( TREE *pTree, void *dataInPtr, void (*callback)(void *)) {
    int duplicated = 0;
    NODE *newNode = _makeNode(dataInPtr);
    if (!newNode)
        return 0;

    pTree->root = _insert(pTree->root, newNode, pTree->compare, callback, &duplicated);
    if (duplicated) {
        free(newNode);
        return 2;
    }

    pTree->count++;
    return 1;
}

void *AVLT_Delete( TREE *pTree, void *keyPtr) {
    void *dataOutPtr = NULL;
    pTree->root = _delete(pTree->root, keyPtr, &dataOutPtr, pTree->compare);
    if (dataOutPtr)
        pTree->count--;
    return dataOutPtr;
}

void *AVLT_Search( TREE *pTree, void *keyPtr) {
    NODE *node = _search(pTree->root, keyPtr, pTree->compare);
    return node ? node->dataPtr : NULL;
}

void AVLT_Traverse( TREE *pTree, void (*callback)(const void *)) {
    _traverse(pTree->root, callback);
}

void AVLT_TraverseR( TREE *pTree, void (*callback)(const void *)) {
    _traverseR(pTree->root, callback);
}

void printTree( TREE *pTree, void (*callback)(const void *)) {
    _inorder_print(pTree->root, 0, callback);
}

int AVLT_Count( TREE *pTree) {
    return pTree->count;
}

int AVLT_Height( TREE *pTree) {
    return getHeight(pTree->root);
}



// used in AVLT_Insert
// return 	pointer to root
static NODE *_insert( NODE *root, NODE *newPtr, int (*compare)(const void *, const void *), void (*callback)(void *), int *duplicated) {
    if (!root)
        return newPtr;

    int cmp = compare(newPtr->dataPtr, root->dataPtr);
    if (cmp < 0) {
        root->left = _insert(root->left, newPtr, compare, callback, duplicated);
    } else if (cmp > 0) {
        root->right = _insert(root->right, newPtr, compare, callback, duplicated);
    } else {
        if (callback)
            callback(root->dataPtr);
        *duplicated = 1;
        return root;
    }

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getHeight(root->left) - getHeight(root->right);

    if (balance > 1 && compare(newPtr->dataPtr, root->left->dataPtr) < 0)
        return rotateRight(root);
    if (balance < -1 && compare(newPtr->dataPtr, root->right->dataPtr) > 0)
        return rotateLeft(root);
    if (balance > 1 && compare(newPtr->dataPtr, root->left->dataPtr) > 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && compare(newPtr->dataPtr, root->right->dataPtr) < 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}


// used in AVLT_Insert
static NODE *_makeNode( void *dataInPtr) {
    NODE *node = (NODE *)malloc(sizeof(NODE));
    if (node) {
        node->dataPtr = dataInPtr;
        node->left = node->right = NULL;
        node->height = 1;
    }
    return node;
}

// used in AVLT_Destroy
static void _destroy( NODE *root, void (*callback)(void *)){
    if (root) {
        _destroy(root->left, callback);
        _destroy(root->right, callback);
        callback(root->dataPtr);
        free(root);
    }
}

// used in AVLT_Delete
// return 	pointer to root
static NODE *_delete(NODE *root, void *keyPtr, void **dataOutPtr, int (*compare)(const void *, const void *)) {
    if (!root)
        return NULL;

    int cmp = compare(keyPtr, root->dataPtr);
    if (cmp < 0) {
        root->left = _delete(root->left, keyPtr, dataOutPtr, compare);
    } else if (cmp > 0) {
        root->right = _delete(root->right, keyPtr, dataOutPtr, compare);
    } else {
        *dataOutPtr = root->dataPtr;

        if (!root->left || !root->right) {
            NODE *temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            NODE *temp = root->right;
            while (temp->left)
                temp = temp->left;

            // Swap data
            void *tempData = root->dataPtr;
            root->dataPtr = temp->dataPtr;
            temp->dataPtr = tempData;

            root->right = _delete(root->right, temp->dataPtr, dataOutPtr, compare);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getHeight(root->left) - getHeight(root->right);

    if (balance > 1 && getHeight(root->left->left) - getHeight(root->left->right) >= 0)
        return rotateRight(root);
    if (balance > 1 && getHeight(root->left->left) - getHeight(root->left->right) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && getHeight(root->right->left) - getHeight(root->right->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && getHeight(root->right->left) - getHeight(root->right->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}



// used in AVLT_Search
// Retrieve node containing the requested key
// return	address of the node containing the key
//			NULL not found
static NODE *_search( NODE *root, void *keyPtr, int (*compare)(const void *, const void *)){
    if (!root)
        return NULL;

    int cmp = compare(keyPtr, root->dataPtr);
    if (cmp < 0)
        return _search(root->left, keyPtr, compare);
    if (cmp > 0)
        return _search(root->right, keyPtr, compare);
    return root;
}


// used in AVLT_Traverse
static void _traverse( NODE *root, void (*callback)(const void *)) {
    if (root) {
        _traverse(root->left, callback);
        callback(root->dataPtr);
        _traverse(root->right, callback);
    }
}

// used in AVLT_TraverseR
static void _traverseR( NODE *root, void (*callback)(const void *)) {
    if (root) {
        _traverseR(root->right, callback);
        callback(root->dataPtr);
        _traverseR(root->left, callback);
    }
}

// used in printTree
static void _inorder_print( NODE *root, int level, void (*callback)(const void *)){
    if (root) {
        _inorder_print(root->right, level + 1, callback);
        for (int i = 0; i < level; i++)
            printf("\t");
        callback(root->dataPtr);
        _inorder_print(root->left, level + 1, callback);
    }
}

// internal function
// return	height of the (sub)tree from the node (root)
static int getHeight( NODE *root) {
    return root ? root->height : 0;
}



// internal function
// Exchanges pointers to rotate the tree to the right
// updates heights of the nodes
// return	new root
static NODE *rotateRight(NODE *root) {
    NODE *newRoot = root->left;
    NODE *temp = newRoot->right;

    newRoot->right = root;
    root->left = temp;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

// internal function
// Exchanges pointers to rotate the tree to the left
// updates heights of the nodes
// return	new root
static NODE *rotateLeft(NODE *root) {
    NODE *newRoot = root->right;
    NODE *temp = newRoot->left;

    newRoot->left = root;
    root->right = temp;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}
