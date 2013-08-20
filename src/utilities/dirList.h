//
//  dirList.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/10/13.
//
//

#ifndef __SUPRNRDY__dirList__
#define __SUPRNRDY__dirList__

#include "ofMain.h"

class dirList {
    
public:
    void setup();
    void update();
    //void draw();
    
    dirList();
    ~dirList();

    void push(string n);
    string pop();
    bool nodeExists(string n);
    int size();
    int cleanList();
    
private:
    typedef struct Node {
		Node *next;
		string path;
	} NODE;
	NODE* head;
    int nodeCount;
};


#endif /* defined(__SUPRNRDY__dirList__) */
