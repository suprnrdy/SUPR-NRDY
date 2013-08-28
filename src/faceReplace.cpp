//
//  faceReplace.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 8/16/13.
//
//

#include "faceReplace.h"

faceReplace::faceReplace() {
//    original->loadImage("test2.jpg");
    mouthImg.loadImage("zombiemouth.png");
    righteyeImg.loadImage("zombie_eye_right.png");
    lefteyeImg.loadImage("zombie_eye_left.png");
    cout << "Zombie overlays loaded" << endl;
    finder.setup("haarcascade_frontalface_alt_tree.xml");
    mouthFinder.setup("haarcascade_mcs_mouth.xml");
    mouthFinder.setNeighbors(5);
    mouthFinder.setScaleHaar(1.01);
    leftEyeFinder.setup("LEye18x12.1.xml");
    leftEyeFinder.setNeighbors(2);
    leftEyeFinder.setScaleHaar(1.2);
    rightEyeFinder.setup("REye18x12.1.xml");
    rightEyeFinder.setNeighbors(2);
    rightEyeFinder.setScaleHaar(1.2);
    
    loadLUT("Earlybird.cube");
}

faceReplace::~faceReplace() {

}

int faceReplace::zombify(ofImage *original, ofFbo *newFbo) {
    int n = 0;
    zombified = newFbo;
    original->resize(400, 300);
    // Allocate space if we haven't already
    if(!gray.bAllocated) {
        gray.allocate(original->width, original->height);
    }
    
    if(!lutImg.isAllocated()) {
        lutImg.allocate(original->width, original->height, OF_IMAGE_COLOR);
    }
    
    // Get grayscale of image
	if( original->type == OF_IMAGE_COLOR ){
		ofxCvColorImage color;
		color.allocate(original->width, original->height);
		color = original->getPixels();
		gray = color;
	}else if( original->type == OF_IMAGE_GRAYSCALE ){
		gray = original->getPixels();
	}else{
		ofLog(OF_LOG_ERROR, "ofxCvHaarFinder::findHaarObjects doesn't support OF_IMAGE_RGBA ofImage");
		return 0;
	}
    
    // Find our face(s)
	finder.findHaarObjects(*original);
    
    // Apply our color edit (like Instagrams)
    applyLUT(original->getPixelsRef());
    
    // BEGIN DRAWING TO OUR FBO
    zombified->begin();
    ofClear(0, 0);
    ofEnableAlphaBlending();
    //original->draw(0, 0);
    lutImg.draw(0,0);
    ofNoFill();
    for(int i = 0; i < finder.blobs.size(); i++) {
        //finder.blobs[i].draw();
        ofRectangle cur = finder.blobs[i].boundingRect;
        
        // FIND MOUTH
        n = mouthFinder.findHaarObjects(gray, cur.x, cur.y + cur.height*2/3, cur.width, cur.height - cur.height*2/3);
        int k = mouthFinder.blobs.size();
        for(int j = 0; j < k; j++) {
            ofRectangle mouth = mouthFinder.blobs[j].boundingRect;
            //mouthFinder.blobs[j].draw();
            //ofRect(mouth.x, mouth.y, mouth.width, mouth.height);
            mouthImg.draw(mouth.x - mouth.width*2/5, mouth.y - mouth.height/3, mouth.width*2, mouth.height*2);
        }
        
        // FIND LEFT EYE
        n = leftEyeFinder.findHaarObjects(gray, cur.x, cur.y + cur.height/5, cur.width/2, cur.height - cur.height*5/8);
        for(int j = 0; j < k; j++) {
            ofRectangle lEye = leftEyeFinder.blobs[j].boundingRect;
            //leftEyeFinder.blobs[j].draw();
            lefteyeImg.draw(lEye.x, lEye.y + lEye.height/11, lEye.width, lEye.height*2);
            //ofRect(lEye.x, lEye.y, lEye.width, lEye.height);
            //ofRect(cur.x, cur.y + cur.height/5, cur.width/2, cur.height - cur.height*3/4);
        }
        
        // FIND RIGHT EYE
        n = rightEyeFinder.findHaarObjects(gray, cur.x + cur.width/2, cur.y + cur.height/5, cur.width/2, cur.height - cur.height*5/8);
        for(int j = 0; j < k; j++) {
            ofRectangle rEye = rightEyeFinder.blobs[j].boundingRect;
            //rightEyeFinder[j].draw();
            righteyeImg.draw(rEye.x, rEye.y + rEye.height/11, rEye.width, rEye.height*2);
            //ofRect(lEye.x, lEye.y, lEye.width, lEye.height);
            //ofRect(cur.x, cur.y + cur.height/5, cur.width/2, cur.height - cur.height*3/4);
        }
    }
    zombified->end();
}

void faceReplace::loadLUT(string path){
	LUTloaded=false;
	
	ofFile file(path);
	string line;
	for(int i = 0; i < 5; i++) {
		getline(file, line);
		ofLog() << "Skipped line: " << line;
	}
	for(int z=0; z<32; z++){
		for(int y=0; y<32; y++){
			for(int x=0; x<32; x++){
				ofVec3f cur;
				file >> cur.x >> cur.y >> cur.z;
				lut[x][y][z] = cur;
			}
		}
	}
	
	LUTloaded = true;
}

//--------------------------------------------------------------
void faceReplace::applyLUT(ofPixelsRef pix){
	if (LUTloaded) {
		
		for(int y = 0; y < pix.getHeight(); y++){
			for(int x = 0; x < pix.getWidth(); x++){
				
				ofColor color = pix.getColor(x, y);
				
				int lutPos [3];
				for (int m=0; m<3; m++) {
					lutPos[m] = color[m] / 8;
					if (lutPos[m]==31) {
						lutPos[m]=30;
					}
				}
				
				ofVec3f start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				ofVec3f end = lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1];
				
				for (int k=0; k<3; k++) {
					float amount = (color[k] % 8) / 8.0f;
					color[k]= (start[k] + amount * (end[k] - start[k])) * 255;
				}
				
				lutImg.setColor(x, y, color);
				
			}
		}
		
		lutImg.update();
	}
}