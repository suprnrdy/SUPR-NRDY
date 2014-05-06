//
//  ve_particles.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 11/12/13.
//
//

#ifndef SUPRNRDY_ve_particles_h
#define SUPRNRDY_ve_particles_h

#pragma once
#include "ofMain.h"
#include "visualElement.h"
#include "ofxSyphon.h"
#include "ofxTween.h"

// Animation Defines
#define ANIMATING   1
#define HOLDING     2

class PVert {
public:
	float x, y, z;
	PVert() {
		x=0.0; y=0.0; z=0.0;
	}
};

class PColor {
public:
	float r, g, b, a;
	PColor() {
		r=0.0; g=1.0; b=1.0; a=1.0;
	}
};

class PTexture {
public:
	float u, v;
	PTexture() {
		u = 0.0; v = 0.0;
	}
};


class ParticleManager {
private:
    float transX, transY, transZ;
    // Image Logo
    ofImage     bwLogo;
	vector<ofVec3f> logoVector;
    vector<ofVec3f> randomVector;
    vector<ofVec3f> sphereVector;
    
    ofFbo fboDisplay;
    
	// Image Textures
	ofImage		texture;
    bool        fboTrue;
    ofFbo       *fbotexture;
    ofImage     tempImg;
	int			cellRows;
	int			cellColls;
	float		texW, texH;
    int         imageCount;
    int         prevImg;
	
	// OpenGL
	GLuint		particleVBO[3];
	
	// Values
	PVert		pos[8000*4];		// vertex (quad) of particle
	float		dim[8000];			// particle size (w/h)
	PTexture	texcords[8000*4];	// texture coords
	PColor		color[8000*4];		// particle color rgba
	float		vel[8000][3];
	float		acc[8000][3];
	float		damping[8000];
	float		life[8000][2];		//	[life] [life rate]
    float       lifeLength;
    int MAX_PARTICLES;
    
	// ------------------------------------------
	void setParticleColor(int i, float r, float g, float b, float a=0.0);
	void setParticlePos(int i, float px, float py, float pz=0.0);
	void setParticleSize(int i, float particleDim);
    
	// ------------------------------------------ Add Position
	void addPosition(int i, float x, float y, float z);
	
	// ------------------------------------------ set the texture coords
	// The idea here is to create a texture that is broken up into cells
	// |----||----|
	// |  1 ||  2 |
	// |----||----|
	// |  3 ||  4 |
	// |----||----|
	// in each cell there is a image. You can then just shift the
	// texture coords and display a new image for the particle
	void setParticleTexCoords(int i, float columnID, float rowID);
    
    // ANIMATIONS
    int animateStatus;
    int numParticleComplete;
    int animationNum;
    
    // Logo functions & variables
    void moveParticle(ofVec3f pos, int i);
    void createLogo();
    void createSphere();
    void explode();
    void moveCamera();
    void floatingParticles();
    bool implode;
    bool loadNewImages;
    imageEngine *imageLoader;
    
    // Floating effect
    float radius,initTime,t;
    float cameraRotation;
    int imageNumber;
    
    // pause time
    float paused_time, start_time;
    
    // Camera
    ofCamera		camera;
    ofNode          subject;
    ofNode          logoSubject;
    ofVec3f         oldN, newN;
    ofxTween        camTween;
    ofxEasingCirc 	easingcirc;

    
public:
    ParticleManager();
	void loadTexture(string path, int cellsInRow, int cellsInCol);
    void loadTexture(ofFbo *texture, int cellsInRow, int cellsInCol);
	void setup(imageEngine *e);
	void update();
	// ------------------------------------------ Add Particles
	void addParticles(int amt, float _x, float _y, float _z);
	void render();
    void updateParticles(int count);
    void play();
    void pause();
    void loadLogo(string file);
    void resetLogo();
    
    ofxSyphonServer particleSyphonServer;
};


#endif
