//
//  ve_zombified.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/16/13.
//
//

#ifndef __SUPRNRDY__ve_zombified__
#define __SUPRNRDY__ve_zombified__

#include "visualElement.h"
#include "faceReplace.h"
#include "ofxSyphon.h"

class zombified : public visualElement {
public:
    zombified();
    ~zombified();
    
    void setup(imageEngine *e);
    void update();
    void draw();
    
private:
    ofImage localImage;
    ofFbo zombifiedFBO;
    ofTexture zombifiedTexture;
    
    bool isZombie;
    faceReplace zombieFace;
    ofxSyphonServer zombieSyphonServer;
    
};

#endif /* defined(__SUPRNRDY__ve_zombified__) */
