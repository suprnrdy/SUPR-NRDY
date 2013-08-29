#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    gui = new ofxUICanvas(0,0,320,320);		//ofxUICanvas(float x, float y, float width, float height)
    gui->addWidgetDown(new ofxUILabel("SUPR NRDY", OFX_UI_FONT_LARGE));
    gui->addTextInput("DIR", "/Users/brizo/Desktop/images", length-xInit);
    gui->addLabelButton("Select Folder", false);
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
    
    currentDirectory = "/Users/brizo/Desktop/images";
    
    ourZomby.setup(&ourImages);
    ourZomby.play();
    setDir = false;
}


//--------------------------------------------------------------
void testApp::update(){
    ourZomby.update();
    if(setDir) {
        openDialog();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    glBegin(GL_QUADS);
    glColor3f( 0.0f, 0.1f, 0.5f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glColor3f( 0.0f, 0.5f, 0.5f );
    glVertex3f( ofGetWidth(), 0.0f, 0.0f );
    glColor3f( 0.0f, 0.5f, 0.5f );
    glVertex3f( ofGetWidth(), ofGetHeight(), 0.0f );
    glColor3f( 0.0f, 0.5f, 0.5f );
    glVertex3f( 0.0f, ofGetHeight(), 0.0f );

    glEnd();
    
    ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString("FRAMERATE: " + ofToString(ofGetFrameRate()), 400,50);
    ourZomby.draw();
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