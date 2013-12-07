//
//  faceReplace.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/16/13.
//
//

#ifndef __SUPRNRDY__faceReplace__
#define __SUPRNRDY__faceReplace__

//#include "ofMain.h"
#include "ofxCvHaarFinder.h"

class faceReplace {
public:
    faceReplace();
    ~faceReplace();
    
    int zombify(ofImage *original, ofFbo *zombified);
    
    
private:
    
    void loadLUT(string path);
    void applyLUT(ofPixelsRef pix);
    
    ofFbo *zombified;
    
    ofImage mouthImg, lefteyeImg, righteyeImg, lutImg, mustacheImg;
    ofxCvHaarFinder finder;
    ofxCvHaarFinder mouthFinder;
    ofxCvHaarFinder leftEyeFinder;
    ofxCvHaarFinder rightEyeFinder;
    ofxCvGrayscaleImage gray;
    bool LUTloaded;
    ofVec3f lut[32][32][32];
    ofPoint lutPos;
};

#endif /* defined(__SUPRNRDY__faceReplace__) */
