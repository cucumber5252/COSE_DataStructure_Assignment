#include <stdlib.h> // malloc
#include <stdio.h>

#include "bst.h"

// internal functions (not mandatory)
static int _insert(NODE *root, NODE *newPtr, int (*compare)(const void *, const void *), void (*callback)(void *));
static NODE *_makeNode(void *dataInPtr);
static void _destroy(NODE *root, void (*callback)(void *));
static NODE *_delete(NODE *root, void *keyPtr, void **dataOutPtr, int (*compare)(const void *, const void *));
static NODE *_search(NODE *root, void *keyPtr, int (*compare)(const void *, const void *));
static void _traverse(NODE *root, void (*callback)(const void *));
static void _traverseR(NODE *root, void (*callback)(const void *));
static void _inorder_print(NODE *root, int level, void (*callback)(const void *));


// used in BST_Insert
static int _insert( NODE *root, NODE *newPtr, int (*compare)(const void *, const void *), void (*callback)(void *)){
    int cmp = compare(newPtr->dataPtr, root->dataPtr);
    if (cmp < 0) {
        if (root->left) {
            return _insert(root->left, newPtr, compare, callback);
        } else {
            root->left = newPtr;
            return 1;
        }
    } else if (cmp > 0) {
        if (root->right) {
            return _insert(root->right, newPtr, compare, callback);
        } else {
            root->right = newPtr;
            return 1;
        }
    } else {
        callback(root->dataPtr);
        return 2;
    }
}

// used in BST_Insert
static NODE *_makeNode( void *dataInPtr){    
    NODE *node = (NODE *)malloc(sizeof(NODE));
    if (node) {
        node->dataPtr = dataInPtr;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

// used in BST_Destroy
static void _destroy( NODE *root, void (*callback)(void *)) {
    if (root) {
        _destroy(root->left, callback);
        _destroy(root->right, callback);
        callback(root->dataPtr);
        free(root);
    }
}

// used in BST_Delete
// return 	pointer to root
static NODE *_delete( NODE *root, void *keyPtr, void **dataOutPtr, int (*compare)(const void *, const void *)){
    if (!root) return NULL;

    int cmp = compare(keyPtr, root->dataPtr);
    if (cmp < 0) {
        root->left = _delete(root->left, keyPtr, dataOutPtr, compare);
    } else if (cmp > 0) {
        root->right = _delete(root->right, keyPtr, dataOutPtr, compare);
    } else {
        *dataOutPtr = root->dataPtr;
        if (!root->left) {
            NODE *temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            NODE *temp = root->left;
            free(root);
            return temp;
        } else {
            //우측 subtree에서 가장 작은 노드를 찾아서 삭제할 노드의 자리에 놓는다.
            NODE *minNode = root->right;
            while (minNode->left) minNode = minNode->left;
            root->dataPtr = minNode->dataPtr;
            root->right = _delete(root->right, minNode->dataPtr, &minNode->dataPtr, compare);
        }
    }
    return root;
}

// used in BST_Search
// Retrieve node containing the requested key
// return	address of the node containing the key
//			NULL not found
static NODE *_search( NODE *root, void *keyPtr, int (*compare)(const void *, const void *)){
    if (!root) return NULL;
    int cmp = compare(keyPtr, root->dataPtr);
    if (cmp < 0) return _search(root->left, keyPtr, compare);
    if (cmp > 0) return _search(root->right, keyPtr, compare);
    return root;
}

// used in BST_Traverse
static void _traverse( NODE *root, void (*callback)(const void *)){
    if (root) {
        _traverse(root->left, callback);
        callback(root->dataPtr);
        _traverse(root->right, callback);
    }
}

// used in BST_TraverseR
static void _traverseR( NODE *root, void (*callback)(const void *)){
    if (root) {
        _traverseR(root->right, callback);
        callback(root->dataPtr);
        _traverseR(root->left, callback);
    }
}


// used in printTree
static void _inorder_print( NODE *root, int level, void (*callback)(const void *)) {
    if (root) {
        _inorder_print(root->right, level + 1, callback);
        for (int i = 0; i < level; i++) printf("     ");
        callback(root->dataPtr);
        _inorder_print(root->left, level + 1, callback);
    }
}

TREE *BST_Create(int (*compare)(const void *, const void *)) {
    TREE *tree = (TREE *)malloc(sizeof(TREE));
    if (tree) {
        tree->count = 0;
        tree->root = NULL;
        tree->compare = compare;
    }
    return tree;
}

void BST_Destroy(TREE *pTree, void (*callback)(void *)) {
    if (pTree) {
        _destroy(pTree->root, callback);
        free(pTree);
    }
}

int BST_Insert(TREE *pTree, void *dataInPtr, void (*callback)(void *)) {
    NODE *newNode = _makeNode(dataInPtr);
    if (!newNode) return 0;

    if (!pTree->root) {
        pTree->root = newNode;
    } else {
        int result = _insert(pTree->root, newNode, pTree->compare, callback);
        if (result == 2) {
            free(newNode);
            return 2;
        }
    }

    pTree->count++;
    return 1;
}

void *BST_Delete(TREE *pTree, void *keyPtr) {
    void *dataOutPtr = NULL;
    pTree->root = _delete(pTree->root, keyPtr, &dataOutPtr, pTree->compare);
    if (dataOutPtr) {
        pTree->count--;
    }
    return dataOutPtr;
}

void *BST_Search(TREE *pTree, void *keyPtr) {
    NODE *node = _search(pTree->root, keyPtr, pTree->compare);
    return node ? node->dataPtr : NULL;
}

void BST_Traverse(TREE *pTree, void (*callback)(const void *)) {
    _traverse(pTree->root, callback);
}

void BST_TraverseR(TREE *pTree, void (*callback)(const void *)) {
    _traverseR(pTree->root, callback);
}

void printTree(TREE *pTree, void (*callback)(const void *)) {
    _inorder_print(pTree->root, 0, callback);
}

int BST_Count(TREE *pTree) {
    return pTree->count;
}