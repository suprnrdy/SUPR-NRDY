//
//  ve_zombified.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/16/13.
//
//

#include "ve_zombified.h"

zombified::zombified() {
    isZombie = true;
}

zombified::~zombified() {
//    delete zombifiedFBO;
}

void zombified::setup(imageEngine *e) {
    visualState = snIDLE;
    isPlay = true;
    syphonServerStatus = false;
    imgPointer = 0;
    imageLoader = e;
    
    // Initialize our timers
    idleTimer = ofGetSystemTime();
    loadingTimer = 0;
    holdTimer = 0;
}

void zombified::update() {
    switch(visualState) {
        case snIDLE:
            if((ofGetSystemTime() - idleTimer) >= zombyIDLETIMER) {
                visualState = snLOADIMAGES;
//                cout << "IDLE" << endl;
            }
            break;
        case snLOADIMAGES:
//            cout << "LOADIMAGES" << endl;
            if(imageLoader->newImages()){
                cout << localImage.loadImage(imageLoader->getImagePath()) << endl;
                visualState = snLOADING;
                loadingTimer = ofGetSystemTime();
                if(!zombifiedFBO.isAllocated()) {
                    zombifiedFBO.allocate(localImage.width, localImage.height);
                    cout << "FBO Allocated" << endl;
                }
            } else {
                visualState = snIDLE;
            }
            break;
        case snLOADING:
//            cout << "LOADING: " << loadingTimer << endl;
            if((ofGetSystemTime() - loadingTimer) >= zombyLOADTIMER) {
                visualState = snHOLD;
                holdTimer = ofGetSystemTime();
            }
            break;
        case snHOLD:
//            cout << "HOLD" << endl;
            if((ofGetSystemTime() - holdTimer) >= zombyHOLDTIMER) {
                visualState = snUNLOADING;
                loadingTimer = ofGetSystemTime();
            }
            break;
        case snUNLOADING:
//            cout << "UNLOADING" << endl;
            if((ofGetSystemTime() - loadingTimer) >= zombyLOADTIMER) {
                visualState = snIDLE;
                idleTimer = ofGetSystemTime();
            }
            break;
    }
    if(isPlay) {
        if(isZombie) {
            if(localImage.isAllocated()) {
                zombieFace.zombify(&localImage, &zombifiedFBO);
                //cout << "Zombified" << endl;
            }
            else {
                zombifiedFBO.begin();
                ofClear(0, 0);
                zombifiedFBO.end();
                //cout << "clearing" << endl;
            }
        }
        //isZombie = false;
    }
}

void zombified::draw() {
    ofSetColor(255, 255, 255);
    if(isPlay) {
        if(zombifiedFBO.isAllocated()) {
            zombifiedFBO.draw(400, 200);
        }
    }
}

