//
//  imageEngine.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/9/13.
//
//

#ifndef __SUPRNRDY__imageEngine__
#define __SUPRNRDY__imageEngine__

//#include "ofMain.h"
#include "utilities/folderWatchThread.h"
#include "utilities/dirList.h"

class imageEngine {
    
public:
    void setup();
    void update();
    //void draw();
    
    imageEngine();
    ~imageEngine();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void onFolderUpdated(ofDirectory &e);
    string getImagePath();
    bool newImages();
    
    
private:
    void exit();
    dirList imageQueue;
    dirList dirListing;
        
    folderWatchThread watchThisFolder;
};


#endif /* defined(__SUPRNRDY__imageEngine__) */
