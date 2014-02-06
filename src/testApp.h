#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxTraerPhysics.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"

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
    
    void reset();
    float resetTime;
    
    //gui shizz
    ofxUICanvas *gui;
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    
    //traer physics lib stuff
    //different particle systems for each look
    ParticleSystem* escapeVelocity;
    float escapeVel;
    float attractionLimit;
    float escapeVelocityOpacity;
    
    ParticleSystem* rubberBandit;
    float force;
    float distance;
    float rubberBanditOpacity;
    
    ParticleSystem* bounceGrid;
    float tension;
    float repulsion;
    float bounceGridOpacity;
    
    ParticleSystem* coSign;
    float coSignOpacity;
    int coSignBossCount;
    
    ParticleSystem* society;
    float societyOpacity;
    vector <Attraction*> centerAttract;
    vector <Attraction*> forceField;
    float socForce;
    Particle* boss;
    float freeWill;
    
    
    
    //generic functions, different results for each system
    void addParticles(ParticleSystem* s);
    void drawParticles(ParticleSystem* s);
    void updateParticles(ParticleSystem* s);
    
    //handy vars
    ofVec3f center;
    ofColor color;
    
    
    //math that oF doens't do
    float angleBetween(ofVec3f vec1, ofVec3f vec2 );
    
    //syphon stuff
    ofxSyphonServer server;
    
    //osc
    ofxOscReceiver osc;
    ofxOscMessage m;
    ofxOscSender oscOut;
    void sendFloatMessage(float v, string s);
    
    
    
};
