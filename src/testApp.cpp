#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    gui = new ofxUICanvas(0,0,320,320);		//ofxUICanvas(float x, float y, float width, float height)
    gui->addWidgetDown(new ofxUILabel("SUPR NRDY", OFX_UI_FONT_LARGE));
    gui->addTextInput("TEXT INPUT", "Input Text", length-xInit);
    gui->addLabelButton("Select Folder", false);
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
    
    ourZomby.setup(&ourImages);
    ourZomby.play();
}


//--------------------------------------------------------------
void testApp::update(){
    ourZomby.update();
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
    
    ourZomby.draw();
    /*
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.0f, 1.0f); // make this vertex purple
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // make this vertex red
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // make this vertex green
    glVertex3f(1.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f); // make this vertex yellow
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
     */
}

void testApp::exit()
{
	gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
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