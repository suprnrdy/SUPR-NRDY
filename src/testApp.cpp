#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 260-xInit;
    gui = new ofxUICanvas(0,0,320,320);		//ofxUICanvas(float x, float y, float width, float height)
    gui->addWidgetDown(new ofxUILabel("SUPR NRDY", OFX_UI_FONT_LARGE));
    gui->addTextInput("DIR", "/Users/brizo/Desktop/images", length-xInit);
    gui->addLabelButton("Select Folder", false);
    gui->addWidgetDown(new ofxUIToggle("FULLSCREEN", 32, 32, false));
    ofxUISlider *mslider = (ofxUISlider*) gui->addWidgetDown(new ofxUIMinimalSlider("PARTICLES", 0, 8000, 7000.0, length-xInit, 50,  OFX_UI_FONT_MEDIUM));
    //ofxUIMinimalSlider(<#string _name#>, <#float _min#>, <#float _max#>, <#float _value#>, <#float w#>, <#float h#>)
    mslider->setLabelPrecision(0);
    gui->addWidgetDown(new ofxUIToggle("Play", true, 32, 32));
//    ofxUIToggle(<#string _name#>, <#bool _value#>, <#float w#>, <#float h#>)
    gui->addLabelButton("Load Logo", false);
    gui->addLabelButton("Reset to Logo", false);
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL, ofColor(0,160));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_FILL_HIGHLIGHT, ofColor(100, 160));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255,255));
    gui->loadSettings("GUI/guiSettings.xml");
    
    
    currentDirectory = "/Users/brizo/Desktop/images";
    ofSetVerticalSync(true);
    ofImage temp;
    int x, y;
    ofDisableArbTex();
    temp.loadImage("/Users/brizo/Desktop/images/0.jpg");
    x = temp.width/2;
    y = temp.height/2;
    imageTexture.allocate(4*x, 4*y);
    imageTexture.begin();
    ofClear(0, 0);
    ofSetColor(255, 255, 255);
    temp.draw(0, 0, x, y);
    temp.loadImage("/Users/brizo/Desktop/images/1.jpg");
    temp.draw(x, 0, x, y);
    temp.loadImage("/Users/brizo/Desktop/images/2.jpg");
    temp.draw(2*x, 0, x, y);
    temp.loadImage("logos/lexus.jpg");
    temp.draw(3*x, 0, x, y);
    imageTexture.end();
    ofEnableArbTex();
    //	particleSystem.loadTexture("/Users/brizo/Desktop/images/processing/IMG_4338.jpg", 2, 2);
    particleSystem.loadTexture(&imageTexture, 4, 4);
    particleSystem.pause();
    particleSystem.setup(&ourImages);
//    ourZomby.setup(&ourImages);
//    ourZomby.play();
    setDir = false;
}


//--------------------------------------------------------------
void testApp::update(){
//    ourZomby.update();
    particleSystem.update();
    if(setDir) {
        openDialog();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    glBegin(GL_QUADS);
    glEnable(GL_DEPTH_TEST);
    glColor3f( 0.0f, 0.1f, 0.5f );
    glVertex3f( 0.0f, 0.0f, -0.3f );
    glColor3f( 0.0f, 0.5f, 0.5f );
    glVertex3f( ofGetWidth(), 0.0f, -0.3f );
    glColor3f( 0.0f, 0.5f, 0.5f );
    glVertex3f( ofGetWidth(), ofGetHeight(), -0.3f );
    glColor3f( 0.0f, 0.5f, 0.5f );
    glVertex3f( 0.0f, ofGetHeight(), -0.3f );

    glEnd();
    
    ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString("FRAMERATE: " + ofToString(ofGetFrameRate()), 400, 10);
    particleSystem.render();
}

void testApp::exit()
{
	gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

void testApp::openDialog() {
    setDir = false;
    ofFileDialogResult openFolderResult= ofSystemLoadDialog("Select image directory", true);
    if (openFolderResult.bSuccess){
        
        ofLogVerbose("User selected a file");
        
        //We have a folder, check it and process it
        currentDirectory = openFolderResult.getPath();
        ofxUITextInput *dir = (ofxUITextInput *)gui->getWidget("DIR");
        dir->setTextString(currentDirectory);
        ourImages.setPath(currentDirectory);
    }else {
        ofLogVerbose("User hit cancel");
    }
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
	if(e.widget->getName() == "BACKGROUND VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofBackground(slider->getScaledValue());
    } else if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
    } else if(e.widget->getName() == "Select Folder") {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue() == 0) {
            setDir = true;
        }
    } else if(e.widget->getName() == "PARTICLES") {
        ofxUISlider *mslider = (ofxUISlider *) e.widget;
        cout << mslider->getScaledValue() << endl;
        particles = mslider->getScaledValue();
        particleSystem.updateParticles(particles);
    } else if(e.widget->getName() == "Play")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if(toggle->getValue()) {
            particleSystem.play();
        } else {
            particleSystem.pause();
        }
    }  else if(e.widget->getName() == "Load Logo") {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue() == 0) {
            ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png");
            
            //Check if the user opened a file
            if (openFileResult.bSuccess){
                ofFile file (openFileResult.getPath());
                string fileExtension = ofToUpper(file.getExtension());
                //We only want images
                if (fileExtension == "JPG" || fileExtension == "PNG") {
                    ofLogVerbose("User selected a file");
                    if (file.exists()){
                        //We have a file, check it and process it
                        particleSystem.loadLogo(file.path());
                    
                    }
                }
            }else {
                ofLogVerbose("User hit cancel");
            }
        }
    } else if(e.widget->getName() == "Reset to Logo") {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if(button->getValue() == 0) {
            particleSystem.resetLogo();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}