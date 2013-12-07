//
//  misc_functions.h
//  SUPRNRDY
//
//  Created by Bryan Bui on 12/5/13.
//
//

#ifndef SUPRNRDY_misc_functions_h
#define SUPRNRDY_misc_functions_h

static ofVboMesh gradientMesh;

void snBackgroundGradient(const ofColor& start, const ofColor& end, const float width, const float height, ofGradientMode mode = OF_GRADIENT_CIRCULAR) {
	float w = width, h = height;
	gradientMesh.clear();
	gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
#ifdef TARGET_OPENGLES
	if(ofIsGLProgrammableRenderer()) gradientMesh.setUsage(GL_STREAM_DRAW);
#else
	gradientMesh.setUsage(GL_STREAM_DRAW);
#endif
	if(mode == OF_GRADIENT_CIRCULAR) {
		// this could be optimized by building a single mesh once, then copying
		// it and just adding the colors whenever the function is called.
		ofVec2f center(w / 2, h / 2);
		gradientMesh.addVertex(center);
		gradientMesh.addColor(start);
		int n = 32; // circular gradient resolution
		float angleBisector = TWO_PI / (n * 2);
		float smallRadius = ofDist(0, 0, w / 2, h / 2);
		float bigRadius = smallRadius / cos(angleBisector);
		for(int i = 0; i <= n; i++) {
			float theta = i * TWO_PI / n;
			gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
			gradientMesh.addColor(end);
		}
	} else if(mode == OF_GRADIENT_LINEAR) {
		gradientMesh.addVertex(ofVec2f(0, 0));
		gradientMesh.addVertex(ofVec2f(w, 0));
		gradientMesh.addVertex(ofVec2f(w, h));
		gradientMesh.addVertex(ofVec2f(0, h));
		gradientMesh.addColor(start);
		gradientMesh.addColor(start);
		gradientMesh.addColor(end);
		gradientMesh.addColor(end);
	} else if(mode == OF_GRADIENT_BAR) {
		gradientMesh.addVertex(ofVec2f(w / 2, h / 2));
		gradientMesh.addVertex(ofVec2f(0, h / 2));
		gradientMesh.addVertex(ofVec2f(0, 0));
		gradientMesh.addVertex(ofVec2f(w, 0));
		gradientMesh.addVertex(ofVec2f(w, h / 2));
		gradientMesh.addVertex(ofVec2f(w, h));
		gradientMesh.addVertex(ofVec2f(0, h));
		gradientMesh.addVertex(ofVec2f(0, h / 2));
		gradientMesh.addColor(start);
		gradientMesh.addColor(start);
		gradientMesh.addColor(end);
		gradientMesh.addColor(end);
		gradientMesh.addColor(start);
		gradientMesh.addColor(end);
		gradientMesh.addColor(end);
		gradientMesh.addColor(start);
	}
	GLboolean depthMaskEnabled;
	glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
	glDepthMask(GL_FALSE);
	gradientMesh.draw();
	if(depthMaskEnabled){
		glDepthMask(GL_TRUE);
	}
}


#endif
