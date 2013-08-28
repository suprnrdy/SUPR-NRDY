//
//  visualElement.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/10/13.
//
//

#ifndef __SUPRNRDY__visualElement__
#define __SUPRNRDY__visualElement__

#include "imageEngine.h"

// OUR sy STATE
#define snIDLE          0
#define snLOADIMAGES    1
#define snLOADING       2
#define snHOLD          3
#define snUNLOADING     4

#define DEFAULTLOADTIMER  200
#define DEFAULTIDLETIMER  2000
#define DEFAULTHOLDTIMER  2000

class visualElement {
public:
    visualElement();
    ~visualElement();
    
    void setup(imageEngine *e);
    void update();
    void draw();
    
    // PUBLIC FUNCTIONS, User Actions
    void setBackground(string imagePath);
    void play();
    void stop();
    void syphonOn();
    void syphonOff();

protected:
    ofImage bkgImage;
    imageEngine *imageLoader;
    // STATE VARIABLES
    int visualState;
    int imgPointer;
    bool isPlay;
    bool syphonServerStatus;
    long long holdTimer;
    long long loadingTimer;
    long long idleTimer;
    int alpha;
};

#endif /* defined(__SUPRNRDY__visualElement__) */
