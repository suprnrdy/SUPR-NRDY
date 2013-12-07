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
    zombieSyphonServer.setName("Zombie Server");
    holdTime = DEFAULTHOLDTIMER;
    loadingTime = DEFAULTLOADTIMER;
    idleTime = DEFAULTIDLETIMER;
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
            if((ofGetSystemTime() - idleTimer) >= idleTime) {
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
                float scaleFactor = (float)localImage.width/600.0;
                localImage.resize(localImage.width/scaleFactor, localImage.height/scaleFactor);
                if(!zombifiedFBO.isAllocated()) {
                    zombifiedFBO.allocate(localImage.width, localImage.height);
                    zombifiedTexture = zombifiedFBO.getTextureReference();
                    cout << "FBO Allocated" << endl;
                }
                
                if(isPlay) {  
                    if(isZombie) {
                        if(localImage.isAllocated()) {
                            zombieFace.zombify(&localImage, &zombifiedFBO);
                            //                //cout << "Zombified" << endl;
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

            } else {
                visualState = snIDLE;
            }
            break;
        case snLOADING:
//            cout << "LOADING: " << loadingTimer << endl;
            if((ofGetSystemTime() - loadingTimer) >= loadingTime) {
                visualState = snHOLD;
                holdTimer = ofGetSystemTime();
            }
            break;
        case snHOLD:
//            cout << "HOLD" << endl;
            if((ofGetSystemTime() - holdTimer) >= holdTime) {
                visualState = snUNLOADING;
                loadingTimer = ofGetSystemTime();
            }
            break;
        case snUNLOADING:
//            cout << "UNLOADING" << endl;
            if((ofGetSystemTime() - loadingTimer) >= loadingTime) {
                visualState = snIDLE;
                idleTimer = ofGetSystemTime();
            }
            break;
    }
}

void zombified::draw() {
    ofSetColor(255, 255, 255);
    if(isPlay) {
        if(zombifiedFBO.isAllocated()) {
            //zombifiedFBO.draw(400, 200);
            zombifiedTexture.draw(400, 200, 400, 300);
            zombieSyphonServer.publishTexture(&zombifiedFBO.getTextureReference());
        }
    }
}

