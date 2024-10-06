#include <stdlib.h> // malloc

#include "adt_dlist.h"

// internal insert function
// inserts data into list
// for addNode function
// return	1 if successful
// 			0 if memory overflow
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr){
	NODE *newNode = (NODE *)malloc(sizeof(NODE));
	if(!newNode){
		return 0;
	}
	newNode->dataPtr = dataInPtr;
	if (pPre == NULL){ //empty or beginning
		newNode->rlink = pList->head;
		newNode->llink = NULL;
		
		if(pList->head == NULL){ //empty
			pList->rear = newNode;
		} else {//beginning
			pList->head->llink = newNode;
		}
		pList->head = newNode;
	} else { //middle or end
		newNode->rlink = pPre->rlink;
		newNode->llink = pPre;

		if(pPre->rlink == NULL){ //end
			pList->rear = newNode;
		} else { //middle
			newNode->rlink->llink = newNode;
		}
		pPre->rlink = newNode;
	}

	pList->count++;
	return 1;
};

// internal delete function
// deletes data from list and saves the (deleted) data to dataOutPtr
// for removeNode function
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr){
	*dataOutPtr = pLoc->dataPtr;

	if (pPre == NULL) { //empty or 1개만 남는 경우
		pList->head = pLoc->rlink;
		if (pList->head == NULL) { //empty
			pList->rear = NULL;
		} else { //1개만 남는 경우
			pLoc->rlink->llink = NULL;
		}
	} else { //middle or end
		pPre->rlink = pLoc->rlink;
		if (pPre->rlink == NULL) { //end
			pList->rear = pPre;
		} else { //middle
			pLoc->rlink->llink = pPre;
		}
	}

	free(pLoc);
	pList->count--;

}




// internal search function
// searches list and passes back address of node containing target and its logical predecessor
// for addNode, removeNode, searchNode functions
// return 1 found
// 0 not found
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu) {
    *pPre = NULL;
    *pLoc = pList->head;
    while (*pLoc != NULL && pList->compare(pArgu, (*pLoc)->dataPtr) > 0) {
        *pPre = *pLoc;
        *pLoc = (*pLoc)->rlink;
    }
    return (*pLoc != NULL && pList->compare(pArgu, (*pLoc)->dataPtr) == 0);
}


// Allocates dynamic memory for a list head node and returns its address to caller
// return	head node pointer
// 			NULL if overflow
LIST *createList(int (*compare)(const void *, const void *)){
	LIST *list = (LIST *)malloc(sizeof(LIST));
    if (list) {
        list->count = 0;
        list->head = NULL;
        list->rear = NULL;
        list->compare = compare;
    }
    return list;
}

//  단어 리스트에 할당된 메모리를 해제 (head node, data node, word data)
void destroyList( LIST *pList, void (*callback)(void *)){
	NODE *deleteNodePtr;
	while(pList->head != NULL){
		deleteNodePtr = pList->head;
		pList->head = deleteNodePtr->rlink;
		callback(deleteNodePtr->dataPtr);
		free(deleteNodePtr);
	}
	free(pList);
}

// Inserts data into list
// return	0 if overflow
//			1 if successful
//			2 if duplicated key (이미 저장된 단어는 빈도 증가)
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *)){
	NODE *pPre, *pLoc;
	int found = _search(pList, &pPre, &pLoc, dataInPtr);
	int checkOverflow;

	if (found == 1) { //찾음
		callback(pLoc->dataPtr);
		return 2;
	} else { //못 찾음
		checkOverflow = _insert(pList, pPre, dataInPtr);
		if (checkOverflow == 1) {
			return 1;
		} else {
			return 0;
		}
	}
}

// Removes data from list
//	return	0 not found
//			1 deleted
int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr) {
	NODE *pPre, *pLoc;
	int found = _search(pList, &pPre, &pLoc, keyPtr);
	if (found == 1){
		_delete(pList, pPre, pLoc, dataOutPtr);
		return 1;
	}
	return 0;
}

// interface to search function
//	pArgu	key being sought
//	dataOutPtr	contains found data
//	return	1 successful
//			0 not found
int searchNode( LIST *pList, void *pArgu, void **dataOutPtr){
	NODE *pPre, *pLoc;
	int found = _search(pList, &pPre, &pLoc, pArgu);

	if (found == 1) { //찾음
		*dataOutPtr = pLoc->dataPtr;
		return 1;
	} else { //못 찾음
		*dataOutPtr = NULL;
		return 0;
	}

}

// returns number of nodes in list
int countList( LIST *pList){
	return pList->count;
}

// returns	1 empty
//			0 list has data
int emptyList( LIST *pList){
	if (pList->count == 0){
		return 0;
	} else {
		return 1;
	}
}


// traverses data from list (forward)
void traverseList( LIST *pList, void (*callback)(const void *)){
	NODE *pNode = pList->head;
	while(pNode != NULL){
		callback(pNode->dataPtr);
		pNode = pNode->rlink;
	}
}

// traverses data from list (backward)
void traverseListR( LIST *pList, void (*callback)(const void *)){
	NODE *pNode = pList->rear;
	while(pNode != NULL){
		callback(pNode->dataPtr);
		pNode = pNode->llink;
	}
}