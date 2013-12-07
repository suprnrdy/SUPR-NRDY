#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "imageEngine.h"
//#include "visualElements/ve_zombified.h"
#include "visualElements/ve_particles.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    ofxUICanvas *gui;
    imageEngine ourImages;
    string currentDirectory;
//    zombified ourZomby;
    
    int particles;
    ParticleManager particleSystem;
    ofFbo imageTexture;
    
    void openDialog();
    bool setDir;
    
};
