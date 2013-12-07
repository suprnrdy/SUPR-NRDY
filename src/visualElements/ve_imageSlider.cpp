//
//   imageSlider.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 9/11/13.
//
//

#include "ve_imageSlider.h"


imageSlider::imageSlider() {
    sliderSyphon.setName("imageSlide Server");
    holdTime = DEFAULTHOLDTIMER;
    loadingTime = DEFAULTLOADTIMER;
    idleTime = DEFAULTIDLETIMER;
}

imageSlider::~imageSlider() {
    //    delete zombifiedFBO;
}

void imageSlider::setup(imageEngine *e) {
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

void imageSlider::update() {
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
                
                if(isPlay) {
                    
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

void imageSlider::draw() {
    ofSetColor(255, 255, 255);
    if(isPlay) {
        // Do Something
    }
}

