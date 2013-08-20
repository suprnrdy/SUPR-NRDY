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
    isPlay = false;
    syphonServerStatus = false;
    imgPointer = 0;
    imageLoader = e;
    localImage.loadImage(e->getImagePath());
    zombifiedFBO.allocate(localImage.width, localImage.height);
    cout << "FBO Allocated" << endl;
}

void zombified::update() {
    if(isPlay) {
        if(isZombie) {
            zombieFace.zombify(&localImage, &zombifiedFBO);
        }
        isZombie = false;
    }
}

void zombified::draw() {
    ofSetColor(255, 255, 255);
    if(isPlay) {
        if(localImage.isAllocated()) {
            //localImage.draw(0, 0);
        }
        if(zombifiedFBO.isAllocated()) {
            zombifiedFBO.draw(100, 100);
        }
    }
}

