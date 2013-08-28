//
//  visualElement.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/10/13.
//
//

#include "visualElement.h"

visualElement::visualElement() {
    
}

visualElement::~visualElement() {
    
}

void visualElement::setup(imageEngine *e) {
    visualState = snIDLE;
    isPlay = false;
    syphonServerStatus = false;
    imgPointer = 0;
    imageLoader = e;
}

void visualElement::update() {
    switch(visualState) {
        case snIDLE:
            break;
        case snLOADIMAGES:
            break;
        case snLOADING:
            break;
        case snHOLD:
            break;
        case snUNLOADING:
            break;
    }
    if(isPlay) {
        // Do something here
    }
}

void visualElement::draw() {
    
}

void visualElement::setBackground(string imagePath) {
    
}

void visualElement::play() {
    isPlay = true;
}

void visualElement::stop() {
    isPlay = false;
}

void visualElement::syphonOff() {
    syphonServerStatus = false;
}

void visualElement::syphonOn() {
    syphonServerStatus = true;
}
