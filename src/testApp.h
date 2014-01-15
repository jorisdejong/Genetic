#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxTraerPhysics.h"
using namespace traer::physics;

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //gui shizz
    ofxUICanvas *gui;
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    
    //traer physics lib
    
    //different particle systems for each look
    ParticleSystem* escapeWaves;
    float escapeVelocity;
    float attractionLimit;
    
    ParticleSystem* rubberBandit;
    float distance;
    
    
    //generic functions, different results for each system
    void addParticle(ParticleSystem* s);
    void drawParticles(ParticleSystem* s);
    void updateParticles(ParticleSystem* s);
    
    //handy vars
    ofVec3f center;
    
    
    //math that oF doens't do
    float angleBetween(ofVec3f vec1, ofVec3f vec2 );
    
    
    
};
