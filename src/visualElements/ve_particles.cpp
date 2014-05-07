//
//  ve_particles.cpp
//  SUPRNRDY
//
//  Created by Bryan Bui on 11/12/13.
//
//

#include "ve_particles.h"
#include <math.h>
#include "misc_functions.h"

//720p
// When using 720, the location of center image shifts... why??? Fix this!!!
//#define SCREEN_W 1280
//#define SCREEN_H 720
//1080p - Why is this really slow?  Uses much more cpu, how do we speed this up overall?
#define SCREEN_W 1920
#define SCREEN_H 1080

ParticleManager::ParticleManager() {
    MAX_PARTICLES = 8000;
    fboTrue = false;
    fboDisplay.allocate(SCREEN_W, SCREEN_H);//
    fboDisplay.allocate(SCREEN_W, SCREEN_H);//
    fboDisplay.begin();
    ofClear(255,255,255, 0);
    fboDisplay.end();
    loadNewImages = false;
    imageCount = 0;
    prevImg = -99;
    cameraRotation = 0;
    implode = true;
    animationNum = 0;
    animateStatus = HOLDING;
    lifeLength = 6;
}

void ParticleManager::play() {
    animateStatus = ANIMATING;
}

void ParticleManager::pause() {
    animateStatus = HOLDING;
}


// ------------------------------------------ load textures
void ParticleManager::loadTexture(string path, int cellsInRow, int cellsInCol) {
	ofDisableArbTex();
	texture.loadImage(path);
	ofEnableArbTex();
	texW = texture.getWidth();
	texH = texture.getHeight();
	cellRows  = cellsInRow;
	cellColls = cellsInCol;
}

void ParticleManager::loadTexture(ofFbo *texture, int cellsInRow, int cellsInCol) {
	fbotexture = texture;
    fboTrue = true;
	texW = texture->getWidth();
	texH = texture->getHeight();
	cellRows  = cellsInRow;
	cellColls = cellsInCol;
}

void ParticleManager::loadLogo(string logo) {
    bwLogo.loadImage(logo);
    bwLogo.setImageType(OF_IMAGE_GRAYSCALE);
    int w = bwLogo.getWidth();
    int h = bwLogo.getHeight();

    if(w > h) {
        if(w < SCREEN_W/3 * 2) {
            bwLogo.resize(SCREEN_W/3 * 2, (SCREEN_W/3 * 2 * h)/w);
        }
    } else {
        if(h < SCREEN_H/3 * 2) {
            bwLogo.resize((SCREEN_H/3 * 2 * w)/h, SCREEN_H/3 * 2);
        }
    }
    
    w = bwLogo.getWidth();
    h = bwLogo.getHeight();
    
    int division = 5;
    logoVector.clear();
    unsigned char* pixels = bwLogo.getPixels();
    for(int y = 0; y < h; y+=division) {
        for(int x = 0; x < w; x+=division) {
            int index = y * w + x;
            unsigned char cur = pixels[index];
            if(cur < 50) {
                ofVec3f point(x - w/2, y - h/2, -100);
                logoVector.push_back(point);
            }
        }
    }
    
}

// ------------------------------------------ init
void ParticleManager::setup(imageEngine *e) {
	printf("-------------------------------\n");
	printf("Max Particles: %i\n", MAX_PARTICLES);
	printf("Pos Size: %i\n", MAX_PARTICLES*4);
	printf("-------------------------------\n");
    imageLoader = e;
    //bwLogo.loadImage("logos/lexus.jpg");
    initTime = t;
    bwLogo.loadImage("logos/BMW_logo_black_white.png");
    bwLogo.setImageType(OF_IMAGE_GRAYSCALE);
    int w = bwLogo.getWidth();
    int h = bwLogo.getHeight();
    
    if(w > h) {
        if(w < SCREEN_W/2) {
            bwLogo.resize(SCREEN_W/3 * 2, (SCREEN_W/3 * 2 * h)/w);
        }
    } else {
        if(h < SCREEN_H/2) {
            bwLogo.resize((SCREEN_H/3 * 2 * w)/h, SCREEN_H/3 * 2);
        }
    }
    
    w = bwLogo.getWidth();
    h = bwLogo.getHeight();
    
    int sphereD = (w > h ? w : h);
    ofVec3f sphereC(0, 0, 0);
    float diameter = 10;
    int division = 6;
    unsigned char* pixels = bwLogo.getPixels();
    ofSetColor(0, 0, 255);
    for(int y = 0; y < h; y+=division) {
        for(int x = 0; x < w; x+=division) {
            int index = y * w + x;
            unsigned char cur = pixels[index];
            if(cur < 50) {
                ofVec3f point(x, y, 0);
                logoVector.push_back(point);
                float d = point.distance(sphereC);
                sphereVector.push_back(ofVec3f((x-1000)*1.3, (y-1000)*1.3, (1000 * cos((PI-0.3)/(sphereD/2) * d))));
                //                sphereVector.push_back(ofVec3f((x - 300)*1.3, (y - 300)*1.3, sqrt((sphereD/2)^2 - (x - 300)^2 - (y - 300)^2)));
            }
        }
    }
    cout << PI/(sphereD/2) << endl;
    cout << "black: " << logoVector.size() << " w: " << w << " h: " << h << endl;
	// Init All The Particles Values
	for(int i=0; i<MAX_PARTICLES; i++) {
		// Set the size of the particle
		setParticleSize(i, 6);
		// The Color Data
		setParticleColor(i, 1, 1, 1, 1);
		// Position and Texture
		float px = ofRandom(-1900, 1900);
		float py = ofRandom(-1900, 1900);
		float pz = ofRandom(-1900, 1900);
        randomVector.push_back(ofVec3f(px, py, pz));
		setParticlePos(i, px, py, pz);
		// The Texture Coords
		setParticleTexCoords(i, (int)ofRandom(0, 2), (int)ofRandom(0, 2));
		// Velocity and accelaration
		acc[i][0] = ofRandom(-1.0, 1.0);
		acc[i][1] = ofRandom(-1.0, 1.0);
		acc[i][2] = 0;//ofRandom(-1.0, 1.0);
		vel[i][0] = ofRandom(-1.0, 1.0);
		vel[i][1] = ofRandom(-1.0, 1.0);
		vel[i][2] = 0;//ofRandom(-1.0, 1.0);
		// The Damping
		damping[i] = 0.2;
		// The Life of the Particle
		life[i][0] = 90;
		life[i][1] = 0.05;
	}
	// Setup the VBO
	glGenBuffersARB(3, &particleVBO[0]);
	// color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*4*sizeof(float), color, GL_STREAM_DRAW_ARB);
	// vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*3*sizeof(float), pos, GL_STREAM_DRAW_ARB);
	// texture coords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*2*sizeof(float), texcords, GL_STREAM_DRAW_ARB);
    
    animateStatus = ANIMATING;
    animationNum = 0;
    // Camera Settings
    camera.setFov(60);
    camera.setPosition(0, 0, 900);
    subject.setGlobalPosition(300, 300, 0);
    subject.setOrientation(ofVec3f(180, 0, 135));
    //camera.clearParent();
    camera.setParent(subject);
    camera.lookAt(subject);
    //oldN = newN;
    newN = ofVec3f(300, 300, 0);
    //cout << oldN << "::" << newN << endl;
    unsigned delay = 0;
    unsigned duration = 10;
    camTween.setParameters(easingcirc, ofxTween::easeInOut, oldN.x, newN.x, duration, delay);
    camTween.addValue(oldN.y, newN.y);
    camTween.addValue(oldN.z, newN.z);
    camTween.start(); //dont forget to call start to sync all the tweens
}

void ParticleManager::updateParticles(int count) {
    MAX_PARTICLES = count;
    cout << MAX_PARTICLES << endl;
}

// ------------------------------------------ set texture coords
void ParticleManager::setParticleTexCoords(int i, float columnID, float rowID) {
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	if(columnID > cellColls) columnID = cellColls;
	if(rowID    > cellRows)	 rowID	  = cellRows;
    if(columnID < 0) columnID = 0;
	if(rowID < 0)	 rowID	  = 0;
	
	// get the cell image width
	float cellWidth  = texW / cellRows;
	float cellHeight = texH / cellColls;
	float row = rowID;
	float col = columnID;
	// P1
	texcords[(i*4)+0].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+0].v = (cellHeight * col)	/ texH;
	// P2
	texcords[(i*4)+1].u = ((cellWidth * row)	+ cellWidth)	/ texW;
	texcords[(i*4)+1].v = (cellHeight * col)	/ texH;
	// P2
	texcords[(i*4)+2].u = ((cellWidth * row) + cellWidth)		/ texW;
	texcords[(i*4)+2].v = ((cellHeight * col) + cellHeight)	/ texH;
	// P2
	texcords[(i*4)+3].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+3].v = ((cellHeight * col)+cellHeight)	/ texH;
}

// ------------------------------------------ set color
void ParticleManager::setParticleColor(int i, float r, float g, float b, float a) {
	if(i < 0) i = 0;
	if(i > MAX_PARTICLES) i = MAX_PARTICLES;
	// Color 1
	color[(i*4)+0].r = r;
	color[(i*4)+0].g = g;
	color[(i*4)+0].b = b;
	color[(i*4)+0].a = a;
	// Color 2
	color[(i*4)+1].r = r;
	color[(i*4)+1].g = g;
	color[(i*4)+1].b = b;
	color[(i*4)+1].a = a;
	// Color 3
	color[(i*4)+2].r = r;
	color[(i*4)+2].g = g;
	color[(i*4)+2].b = b;
	color[(i*4)+2].a = a;
	// Color 4
	color[(i*4)+3].r = r;
	color[(i*4)+3].g = g;
	color[(i*4)+3].b = b;
	color[(i*4)+3].a = a;
}

// ------------------------------------------ set position
void ParticleManager::setParticlePos(int i, float px, float py, float pz) {
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	// P1
	pos[(i*4)+0].x = px;
	pos[(i*4)+0].y = py;
	pos[(i*4)+0].z = pz;
	// P2
	pos[(i*4)+1].x = px + dim[i];
	pos[(i*4)+1].y = py;
	pos[(i*4)+1].z = pz;
	// P2
	pos[(i*4)+2].x = px + dim[i];
	pos[(i*4)+2].y = py + dim[i];
	pos[(i*4)+2].z = pz;
	// P2
	pos[(i*4)+3].x = px;
	pos[(i*4)+3].y = py + dim[i];
	pos[(i*4)+3].z = pz;
}

// ------------------------------------------ set size of particle
void ParticleManager::setParticleSize(int i, float particleDim) {
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	dim[i] = particleDim;
}

// ------------------------------------------ add to the current position
void ParticleManager::addPosition(int i, float x, float y, float z) {
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
    
	// P1
	pos[(i*4)+0].x += x;
	pos[(i*4)+0].y += y;
	pos[(i*4)+0].z += z;
	// P2
	pos[(i*4)+1].x += x;
	pos[(i*4)+1].y += y;
	pos[(i*4)+1].z += z;
	// P2
	pos[(i*4)+2].x += x;
	pos[(i*4)+2].y += y;
	pos[(i*4)+2].z += z;
	// P2
	pos[(i*4)+3].x += x;
	pos[(i*4)+3].y += y;
	pos[(i*4)+3].z += z;
    
    if(animationNum == 3 && i < 12) {
        //cout << i << " = " << pos[(i*4)+0].x  << 'x' << pos[(i*4)+0].y << 'x' << pos[(i*4)+0].z << endl;
    }
}

// ------------------------------------------ Move the particle to the new position at acceleration 'pos'
void ParticleManager::moveParticle(ofVec3f pos, int i) {
    acc[i][0] = pos.x;
    acc[i][1] = pos.y;
    acc[i][2] = pos.z;
    vel[i][0] += acc[i][0];
    vel[i][1] += acc[i][1];
    vel[i][2] += acc[i][2];
    addPosition(i, vel[i][0], vel[i][1], vel[i][2]);
    vel[i][0] *= damping[i];
    vel[i][1] *= damping[i];
    vel[i][2] *= damping[i];
    
    if(animationNum == 3) {
        //cout << i << " = " << vel[i][0] << " x " << vel[i][1] << " x " << vel[i][2] << endl;
    }
}

// ------------------------------------------ Add Particles
void ParticleManager::addParticles(int amt, float _x, float _y, float _z) {
	
}

// ------------------------------------------ Do the calculations and updating here
void ParticleManager::update() {
    t = ofGetElapsedTimef();
	t = t - initTime;
    cameraRotation =  ofSignedNoise(t*0.2) * 0.75;
    if(animationNum == 0) {
        createLogo();
    } else if(animationNum == 1) {
        createSphere();
    } else if(animationNum == 2) {
        explode();
    } else if(animationNum == 3) {
        moveCamera();
        //floatingParticles();
    }
    for(int i=0; i<MAX_PARTICLES; i++) {
		// fade by the life rate
		life[i][0] -= life[i][1];
        setParticleColor(i, 1.0, 1.0, 1.0, 1.0);
	}
}

// ------------------------------------------ Render everything
void ParticleManager::render() {
    if(loadNewImages){
        cout << "Updating textures" << endl;
        ofDisableArbTex();
        fbotexture->begin();
        ofClear(0, 0);
        ofSetColor(0, 0, 0);
        ofRect(0, 0, texW, texH);
        ofSetColor(255, 255, 255);
        float x  = texW / cellRows;
        float y = texH / cellColls;
        for(int m = 0; m < cellRows; m++){
            for (int n = 0; n < cellColls; n++) {
                if(imageLoader->newImages()){
                    int i = (m * 4) + n;
                    cout << i << endl;
                    imageCount++;
                    tempImg.loadImage(imageLoader->getImagePath());
                    tempImg.draw(m * x, n * y, x, y);
                    setParticleTexCoords(i, m, n);
                } else {
                    ofFill();
                    ofSetColor(0, 0, 255);
                    ofRect(m * x, n * y, x, y);
                    ofSetColor(255, 255, 255);
                    ofDrawBitmapString("PLACE HOLDER", m * x + 25, n * y + 25)  ;
                    
                }
            }
        }
        ofEnableArbTex();
        fbotexture->end();
        /*
         for(int i= cellColls * (cellRows - 1); i<MAX_PARTICLES; i++) {
            setParticleTexCoords(i, (int)ofRandom(0, 4), (int)ofRandom(0, 4));
        }
         */
        loadNewImages = false;
        cout << "Loaded images: " << imageCount << endl;
    }
    
    fboDisplay.begin();
    ofClear(0, 0, 0, 0);
    camera.begin();
    ofPushMatrix();
    ofSetColor(255, 128, 255);
    ofVec3f v1 = camera.getGlobalPosition();
    ofVec3f v2 = subject.getGlobalPosition();
    //ofLine(v1,v2);
    ofSetColor(255, 255, 255);
    //snBackgroundGradient(155, 0, SCREEN_W, SCREEN_H);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	// bind tex coors
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*2*sizeof(float), texcords);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	// bind color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*4*sizeof(float), color);
	glColorPointer(4, GL_FLOAT, 0, 0);
	// bind vertices [these are quads]
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*3*sizeof(float), pos);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	// draw the vbo
    glEnable(GL_DEPTH_TEST);
    ofEnableArbTex();
    ofEnableAlphaBlending();
    if(fboTrue) {
        fbotexture->getTextureReference().bind();
    } else {
        texture.getTextureReference().bind();
    }
	glDrawArrays(GL_QUADS, 0, MAX_PARTICLES*4);
    if(fboTrue) {
        fbotexture->getTextureReference().unbind();
    } else {
        texture.getTextureReference().unbind();
    }
	ofDisableAlphaBlending();
	ofDisableArbTex();
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    
//    ofSetColor(255, 0, 0);
//    ofLine(-100, -100, 1090, pos[11].x, pos[11].y, pos[11].z);
    
    ofPopMatrix();
    camera.end();
    fboDisplay.end();
    if(fboDisplay.isAllocated()) {
        ofNoFill();
        ofSetColor(255, 255, 255, 255);
        fboDisplay.draw(400, 30, 600, 350);
        particleSyphonServer.publishTexture(&fboDisplay.getTextureReference());
    }
    
    fbotexture->draw(400, 400, 600, 350);
}

// ------------------------------------------ Manage all of our animations here

void ParticleManager::resetLogo() {
    animationNum = 0;
    int logoPos = 0;
    
    for(int i=0; i<MAX_PARTICLES; i++) {
        if(logoPos >= logoVector.size()){
            logoPos = 0;
        }
        setParticleSize(i, 6);
        setParticlePos(i, logoVector[logoPos].x, logoVector[logoPos].y, logoVector[logoPos].z);
        
        if (MAX_PARTICLES < logoVector.size()) {
            if(logoVector.size()/MAX_PARTICLES > 1) {
                logoPos += logoVector.size()/MAX_PARTICLES;
            } else {
                logoPos += 2;
            }
        } else {
            logoPos++;
        }
    }
    camera.setPosition(0, 0, 900);
    subject.setGlobalPosition(300, 300, 0);
    subject.setOrientation(ofVec3f(180, 0, 135));
    //camera.clearParent();
    camera.setParent(subject);
    camera.lookAt(subject);
    //oldN = newN;
    newN = ofVec3f(300, 300, 0);
    //cout << oldN << "::" << newN << endl;
    unsigned delay = 0;
    unsigned duration = 10;
    camTween.setParameters(easingcirc, ofxTween::easeInOut, oldN.x, newN.x, duration, delay);
    camTween.addValue(oldN.y, newN.y);
    camTween.addValue(oldN.z, newN.z);
    camTween.start(); //dont forget to call start to sync all the tweens
    
}

void ParticleManager::createLogo() {
    int logoPos = 0;
//    cout << "create logo" << endl;
    if(animateStatus == ANIMATING) {
        numParticleComplete = 0;
        subject.setGlobalPosition(camTween.update(), camTween.getTarget(1), camTween.getTarget(2));
        ofVec3f v1 = camera.getGlobalPosition();
        ofVec3f v2 = subject.getGlobalPosition();
        if(v1.distance(v2) < 900) {
            camera.dolly(10);
        }
        for(int i=0; i<MAX_PARTICLES; i++) {
            ofVec3f position(pos[i*4].x, pos[i*4].y, pos[i*4].z);
            if(logoPos >= logoVector.size()){
                logoPos = 0;
            }
            ofVec3f dir = logoVector[logoPos] - position;
            dir.normalize();
            float d = position.distance(logoVector[logoPos]);
            if(d < 150 & d > 1) {
                float size = ofMap(d, 0, 150, 8, dim[i]);
                //setParticleSize(i, size);
                //setParticlePos(i, pos[i*4].x, pos[i*4].y, pos[i*4].z);
            }
            
            (d < 50) ? dir *= 1 : dir *= ofRandom(20, 12);
            if(d > 2000) dir *= 200;
            if(d < 2) {
                numParticleComplete++;
            } else {
                moveParticle(dir, i);
            }
            if (MAX_PARTICLES < logoVector.size()) {
                if(logoVector.size()/MAX_PARTICLES > 1) {
                    logoPos += logoVector.size()/MAX_PARTICLES;
                } else {
                    logoPos += 2;
                }
            } else {
                logoPos++;
            }
        }
        if(numParticleComplete >= MAX_PARTICLES) {
            animateStatus = ANIMATING;
            cout << "Holding" << endl;
            if(imageLoader->newImages()){
                animationNum = 2;
            }
        }
    }
}

void ParticleManager::createSphere() {
    int spherePos = 0;
    if(animateStatus == ANIMATING) {
        numParticleComplete = 0;
        for(int i=0; i<MAX_PARTICLES; i++) {
            ofVec3f position(pos[i*4].x, pos[i*4].y, pos[i*4].z);
            if(spherePos >= sphereVector.size()){
                spherePos = 0;
            }
            ofVec3f dir = sphereVector[spherePos] - position;
            float d = position.distance(sphereVector[spherePos]);
            if(d < 150) {
                float size = ofMap(d, 0, 150, 8, dim[i]);
                //setParticleSize(i, size);
                //setParticlePos(i, pos[i*4].x, pos[i*4].y, pos[i*4].z);
            }
            dir.normalize();
            (d < 50) ? dir *= 1 : dir *= 5;
            if(d < 2) {
                numParticleComplete++;
            } else {
                moveParticle(dir, i);
            }
            if (MAX_PARTICLES < sphereVector.size()) {
                if(sphereVector.size()/MAX_PARTICLES > 1) {
                    spherePos += sphereVector.size()/MAX_PARTICLES;
                } else {
                    spherePos += 2;
                }
            } else {
                spherePos++;
            }
        }
        if(numParticleComplete >= MAX_PARTICLES) {
            animateStatus = ANIMATING;
            cout << "Holding" << endl;
            animationNum = 2;
        }
    }
}

void ParticleManager::explode() {
//    cout << "explode" << endl;
    if(animateStatus == ANIMATING) {
        numParticleComplete = 0;
        for(int i=0; i<MAX_PARTICLES; i++) {
            ofVec3f position(pos[i*4].x, pos[i*4].y, pos[i*4].z);
            float d;
            ofVec3f dir;
            if(implode){
                dir = ofVec3f(300, 300, 0) - position;
                d = position.distance(ofVec3f(300, 300, 0));
            } else {
                if (position.x > SCREEN_W + 100 || position.x < - (SCREEN_W + 100)) {
                    position = randomVector[i];
                } else if (position.y > SCREEN_H + 100 || position.y < -(SCREEN_H + 100)) {
                    position = randomVector[i];
                } else if (position.z > 1090) {
                    position = randomVector[i];
                    numParticleComplete = MAX_PARTICLES;
                }
                dir = randomVector[i] - position;
                d = position.distance(randomVector[i]);
            }
            if(d < 150) {
                if(!implode) {
                    float size = ofMap(150 - d, 0, 150, dim[i], 20);
                    //setParticleSize(i, size);
                    //setParticlePos(i, pos[i*4].x, pos[i*4].y, pos[i*4].z);
                }
            }
            dir.normalize();
            if(d < 50) {
                dir *= 1;
            } else {
                (implode) ? dir *= 20 : dir *= 15;
            }
            if(d < 2) {
                numParticleComplete++;
                acc[i][0] = ofRandom(-1.0, 1.0);
                acc[i][1] = ofRandom(-1.0, 1.0);
                acc[i][2] = ofRandom(-1.0, 1.0);
                vel[i][0] = ofRandom(-1.0, 1.0);
                vel[i][1] = ofRandom(-1.0, 1.0);
                vel[i][2] = ofRandom(-1.0, 1.0);
                damping[i] = 0.1;
            } else {
                moveParticle(dir, i);
            }
        }
        
        if(numParticleComplete >= MAX_PARTICLES) {
            if(implode) {
                cout << "Holding implode" << endl;
                implode = false;
                loadNewImages = true;
            } else {
                cout << "Holding explode" << endl;
                animateStatus = ANIMATING;
                animationNum++;
                oldN = ofVec3f(subject.getX(), subject.getY(), subject.getZ());
                
                unsigned delay = 0;
                unsigned duration = 3000;
                imageCount--;
                newN = ofVec3f(pos[imageCount*4].x + 3, pos[imageCount*4].y + 3, pos[imageCount*4].z);
                camTween.setParameters(easingcirc, ofxTween::easeInOut, oldN.x, newN.x, duration, delay);
                camTween.addValue(oldN.y, newN.y);
                camTween.addValue(oldN.z, newN.z);
                camTween.start(); //dont forget to call start to sync all the tweens
            }
        }
    }
}

void ParticleManager::floatingParticles() {
    if(animateStatus == ANIMATING) {
//      cout << "FLoating Particles" << endl;
        numParticleComplete = 0;
        ofVec3f prevDir;
        int offset = -100;
        if(imageCount > 0) {
            int i = imageCount - 1;
            if(prevImg > 0) {
                //cout << "Prev image: " << prevImg << endl;
//                moveParticle(prevDir, prevImg);
                int i = prevImg;
                vel[i][0] += acc[i][0] * ofRandom(1, 2);
                vel[i][1] += acc[i][1] * ofRandom(1, 2);
                vel[i][2] += acc[i][2] * ofRandom(1, 2);
                addPosition(i, vel[i][0], vel[i][1], vel[i][2]);
                vel[i][0] *= damping[i];
                vel[i][1] *= damping[i];
                vel[i][2] *= damping[i];
            }
            ofVec3f position(pos[i*4].x, pos[i*4].y, pos[i*4].z);
            ofVec3f dir = ofVec3f(offset, offset, 1199) - position;
            float d = position.distance(ofVec3f(offset, offset, 1199));
            dir.normalize();
            
            
            (d < 50) ? dir *= 2 : dir *= ofRandom(8, 5);
            if(d > 2) {
                moveParticle(dir, i);
                //setParticlePos(i, -100, -100, 1090);
                cout << "Image #: " << i  << " Position: " << position << " Life: " << life[i][0] << endl;
            } else {
                //cout << "image: " << imageCount << " life: " << life[i][0] << endl;
                life[i][0] -= life[i][1];
                if (life[i][0] < 0.0) {
                    life[i][0] = lifeLength;
                    imageCount--;
                    prevImg = i;
                }
            }
        } else {
            animationNum = 0;
            int prevImg = -99;
            return;
        }
        
        int tW = SCREEN_W * 2;
        int tH = SCREEN_H * 2;
        int startPos;
        if(prevImg > 0) {
            startPos = prevImg + 1;
        } else {
            startPos = imageCount;
        }
        for(int i = startPos; i<MAX_PARTICLES; i++) {
            ofVec3f position(pos[i*4].x, pos[i*4].y, pos[i*4].z);
            damping[i] = 0.01;
            // Velocity and accelaration
			vel[i][0] += acc[i][0];
            vel[i][1] += acc[i][1];
            vel[i][2] += acc[i][2];
            addPosition(i, vel[i][0], vel[i][1], vel[i][2]);
            if (position.z < -900) {
                setParticlePos(i, ofRandom(-tW, tW), ofRandom(-tH, tH), 1150);
            }
            if (position.z > 1200) {
                setParticlePos(i, ofRandom(-tW, tW), ofRandom(-tH, tH), -900);
            }
            vel[i][0] *= damping[i];
            vel[i][1] *= damping[i];
            vel[i][2] *= damping[i];
        }
    }
}

void ParticleManager::moveCamera() {
    numParticleComplete = 0;
    
    for(int i=1; i<MAX_PARTICLES; i++) {
        if(i != imageCount){
            ofVec3f position(pos[i*4].x, pos[i*4].y, pos[i*4].z);
            // Velocity and accelaration
            //vel[i][0] += acc[i][0];
            //vel[i][1] += acc[i][1];
            //vel[i][2] += acc[i][2];
            addPosition(i, vel[i][0], vel[i][1], vel[i][2]);
            if (position.z < -900) {
                setParticlePos(i, ofRandom(-1500, 1500), ofRandom(-1500, 1500), 1150);
            }
            if (position.z > 1200) {
                setParticlePos(i, ofRandom(-1500, 1500), ofRandom(-1500, 1500), -900);
            }
            //vel[i][0] *= damping[i];
            //vel[i][1] *= damping[i];
            //vel[i][2] *= damping[i];
        }
    }
    
    if(animateStatus == ANIMATING) {
        subject.setGlobalPosition(camTween.update(), camTween.getTarget(1), camTween.getTarget(2));
        ofVec3f v1 = camera.getGlobalPosition();
        ofVec3f v2 = subject.getGlobalPosition();
        if(v1.distance(v2) > 100) {
            camera.dolly(-10);
        } else if(v1.distance(v2) > 12) {
            camera.dolly(-5);
        }
        start_time = ofGetElapsedTimeMillis();
    }
    if(camTween.isCompleted()) {
        animateStatus = HOLDING;
        paused_time = ofGetElapsedTimeMillis();
        if (paused_time - start_time > 2500) {
            if(imageCount > 0) {
                int i = imageCount-1;
                //cout << "Prev image: " << prevImg << endl;
                //                moveParticle(prevDir, prevImg);
                oldN = newN;
                newN = ofVec3f(pos[i*4].x + 3, pos[i*4].y + 3, pos[i*4].z);
                unsigned delay = 0;
                unsigned duration = 3000;
                camTween.setParameters(easingcirc, ofxTween::easeInOut, oldN.x, newN.x, duration, delay);
                camTween.addValue(oldN.y, newN.y);
                camTween.addValue(oldN.z, newN.z);
                camTween.start(); //dont forget to call start to sync all the tweens
                imageCount--;
            } else {
                cout << "images done" << endl;
                implode = true;
                animationNum = 0;
                oldN = newN;
                newN = ofVec3f(300, 300, 0);
                //cout << oldN << "::" << newN << endl;
                unsigned delay = 0;
                unsigned duration = 300;
                camTween.setParameters(easingcirc, ofxTween::easeInOut, oldN.x, newN.x, duration, delay);
                camTween.addValue(oldN.y, newN.y);
                camTween.addValue(oldN.z, newN.z);
                camTween.start(); //dont forget to call start to sync all the tweens
            }
            animateStatus = ANIMATING;
        }
        //cout << oldN << "::" << newN << endl;
        //animationNum++;
    }
}

