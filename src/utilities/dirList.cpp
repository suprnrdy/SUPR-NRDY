//
//  dirList.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/10/13.
//
//

#include "dirList.h"

dirList::dirList() {
    head = NULL;
    nodeCount = 0;
}

dirList::~dirList() {
    if(head == NULL) return;
	NODE *cur = head;
	while(cur) {
		Node *ptr = cur;
		cur = cur->next;
		delete ptr;
	}
    nodeCount = 0;
}

void dirList::push(string n) {
    if(head == NULL) {
		head = new NODE;
		head->path = n;
		head->next = NULL;
        nodeCount++;
		return;
	}
	NODE *cur = head;
	while(cur) {
		if(cur->next == NULL) {
			NODE *ptr = new NODE;
			ptr->path = n;
			ptr->next = NULL;
			cur->next = ptr;
            nodeCount++;
			return;
		}
		cur = cur->next;
	}
}

string dirList::pop() {
    if(head == NULL) {
		cout << "empty estack!" << endl;
        nodeCount = 0;
		return NULL;
	}
	NODE *tmp = head;
	string value = head->path;
	if(head->next) {
		head = head->next;
        delete tmp;
        nodeCount--;
	}
	// pop the last element (head)
	else {
		delete tmp;
		head = NULL;
        nodeCount = 0;
	}
	cout << "pop: " << value << endl;
	return value;
}

int dirList::size() {
    int temp = 0;
    Node *pointer;
    for (pointer = head; pointer != NULL; pointer = pointer->next) {
        temp++;
    }
    return temp;
    //return nodeCount;
}

int dirList::cleanList() {
    Node *pointer, *prevP;
    int deleteCount = 0;
    
    /* For 1st node, indicate there is no previous. */
    prevP = NULL;
    
    for (pointer = head; pointer != NULL; prevP = pointer, pointer = pointer->next) {
        if (!ofFile::doesFileExist(pointer->path, false)) {  /* Does NOT exist, remove. */
            ofLog(OF_LOG_NOTICE, "Deleting invalid path");
            if (prevP == NULL) {
                /* Fix beginning pointer. */
                head = pointer->next;
            } else {
                /*
                 * Fix previous node's next to
                 * skip over the removed node.
                 */
                prevP->next = pointer->next;
            }
            deleteCount++;
            delete pointer;  /* Deallocate the node. */
            pointer = head;
            nodeCount--;
        }
    }
    return deleteCount;
}

bool dirList::nodeExists(string n) {
    Node *pointer;
    
    /*
     * Visit each node.
     */
    for (pointer = head; pointer != NULL; pointer = pointer->next) {
        if (pointer->path == n) {  /* Found it. */
            return true;   /* Done searching. */
        }
    }
    return false;  /* Not in the list. */
}