//
//   imageSlider.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 9/11/13.
//
//

#ifndef __SUPRNRDY___imageSlider__
#define __SUPRNRDY___imageSlider__

#include "visualElement.h"
#include "ofxSyphon.h"

class imageSlider : public visualElement {
public:
    imageSlider();
    ~imageSlider();
    
    void setup(imageEngine *e);
    void update();
    void draw();
    
private:
    ofImage localImage;
    ofFbo sliderFBO;
    
    ofxSyphonServer sliderSyphon;
    
    //Motion Variables
};

#endif /* defined(__SUPRNRDY___imageSlider__) */
