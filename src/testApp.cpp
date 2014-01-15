#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofEnableSmoothing();
    ofBackground(0);
    
    //handy var init
    center = ofVec3f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0);
    color = ofColor(255,96,0);
    
    //escapeWaves setup
    escapeWaves = new ParticleSystem(0.0,0.01);
    escapeWaves->clear();
    addParticles(escapeWaves);
    escapeVelocity=0.0;
    attractionLimit = 0.1;
    
    //rubberBandit setup
    rubberBandit = new ParticleSystem(0.0,0.01);
    rubberBandit->clear();
    Particle* banditBoss = rubberBandit->makeParticle(50, center, 10);
    banditBoss->makeFixed();
    addParticles(rubberBandit);
    distance = 0.0;
    force = 0.0;
    
    //bounceGrid setup
    bounceGrid = new ParticleSystem(0.0,0.01);
    bounceGrid->clear();
    Particle* bounceBoss = bounceGrid->makeParticle(50,center,0);
    bounceBoss->makeFixed();
    addParticles(bounceGrid);

    
    //gui
    gui = new ofxUICanvas(10,10,200,ofGetHeight()-20);
    gui->addFPS();
    gui->addLabel("Escape Waves");
    gui->addSlider("Escape Velocity", 0.0, 5.0, escapeVelocity);
    gui->addSlider("Gravity",0.0,1.0,attractionLimit);
    gui->addSpacer();
    gui->addLabel("Rubber Bandit");
    gui->addSlider("Force",-100.0,100.0,force);
    gui->addSlider("Distance",0.0,1000.0,distance);

    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    
    





}

//--------------------------------------------------------------
void testApp::update(){
    
    //updateParticles(escapeWaves);
    //updateParticles(rubberBandit);
    
    //update the clock
    //escapeWaves->tick();
    //rubberBandit->tick();
    bounceGrid->tick();

}

//--------------------------------------------------------------
void testApp::draw(){

    
    ofFill();

    //draw each system
    ofSetColor(255,128);
    //drawParticles(escapeWaves);
    //drawParticles(rubberBandit);
    drawParticles(bounceGrid);


    
    //draw a center circle
    ofSetColor(color,255);
    ofCircle(center,10);
    ofNoFill();
    ofSetColor(255,255);
    ofCircle(center,10);
    
}

void testApp::exit()
{
    delete gui;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    bounceGrid->getParticle(0)->position=ofVec3f(x,y,0.0);

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

void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();

    if(name =="Escape Velocity")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        escapeVelocity = value;        
    }
    else if (name=="Gravity")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        attractionLimit = value;
    }
    else if (name=="Force")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = -slider->getScaledValue();
        force = value;
    }
    
    else if (name=="Distance")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        distance = value;
    }
    
}

void testApp::addParticles(ParticleSystem* s)
{
    if(s == escapeWaves)
    {
        //create 10000 particles ready for recycling, using simple positioning, Traer is too much for this
        for(int i = 0; i < 10000; i++)
        {
            //place them in a circle
            float angle = ofRandom(2.0*PI);
            ofVec3f pos = center+(ofVec3f(cos(angle),sin(angle),0.0)*5);
            float mass = ofRandom(1.0,2.0);
            Particle* p = escapeWaves->makeParticle(mass, pos, mass);
            p->makeFixed();
        }
    }
    
    if(s == rubberBandit)
    {
        //create particles in a circle, connected with springs to each other and to the center, they are also repulsed from the center
        for(int i = 0; i < 36; i++)
        {
            //place them in a circle
            float angle = (2.0*PI)/36 * i;
            ofVec3f pos = center+(ofVec3f(cos(angle),sin(angle),0.0)*10);
            float mass = ofRandom(1.0,10.0);
            Particle* p = rubberBandit->makeParticle(mass, pos, 2.0);
            Attraction* a = rubberBandit->makeAttraction(rubberBandit->getParticle(0), p, force, 10.0);
        }
        
        //make springs between them and to the center
        for(int i = 0; i < 36; i++)
        {
            Spring* s = rubberBandit->makeSpring(rubberBandit->getParticle(0), rubberBandit->getParticle(i+1), 0.001, 1.0, distance);
            if(i<35)
                Spring* s = rubberBandit->makeSpring(rubberBandit->getParticle(i+1), rubberBandit->getParticle(i+2),0.01, 0.1, distance/12.5);
            else
                Spring* s = rubberBandit->makeSpring(rubberBandit->getParticle(i+1), rubberBandit->getParticle(1),0.01, 0.1, distance/12.5);
        }
    }
    
    if(s == bounceGrid)
    {
        //place all the particles in a grid and place springs between them
        int gridHorizontal = 33;
        int gridVertical = 9;
        float distX = ofGetWidth()/float(gridHorizontal+1);
        float distY = ofGetHeight()/float(gridVertical+1);
        
        for(int y = 0; y < gridVertical; y++)
        {
            for(int x = 0; x < gridHorizontal; x++)
            {

                ofVec3f pos;
                pos.x = distX * (x+1);
                pos.y = distY * (y+1);
                pos.z = 0.0;
                Particle* p = bounceGrid->makeParticle(10, pos, 5);
                if(x>0)
                    Spring* s = bounceGrid->makeSpring(p, bounceGrid->getParticle(x+y*gridHorizontal), 0.01, 0.1, distX);
                if(y>0)
                    Spring* s = bounceGrid->makeSpring(p, bounceGrid->getParticle(x+1+(y-1)*gridHorizontal), 0.01, 0.1, distY);
                
                //connect them to the boss, which is mouse controlled
                Attraction* a = bounceGrid->makeAttraction(p, bounceGrid->getParticle(0), -10.0, 10);
            }
        }
        
        for(int y = 0; y < gridVertical; y++)
        {
            for(int x = 0; x < gridHorizontal; x++)
            {
                
                ofVec3f pos;
                pos.x = distX * (x+1);
                pos.y = distY * (y+1);
                pos.z = 0.0;
                Particle* pFixed = bounceGrid->makeParticle(10, pos, 5);
                pFixed->makeFixed();
                Spring* s = bounceGrid->makeSpring(pFixed, bounceGrid->getParticle(x+y*gridHorizontal+1), 0.001, 0.2, 0);
            
                
            }
        }
    }
    
}

void testApp::updateParticles(ParticleSystem* s)
{
    if(s==escapeWaves)
    {
        for(int i = 0; i < escapeWaves->numberOfParticles(); i++)
        {
            Particle* particle = escapeWaves->getParticle(i);
            //if the particle are close to the center, shoot them out based on the current velocity value
            float dist = center.distance(particle->position);
            if(dist<10.0)
            {
                particle->velocity=(particle->position-center).limit(1)*particle->mass*escapeVelocity;
                //if they're stuck in the middle, shoot them out randomly
                if(dist<=1.0)
                {
                    float angle = ofRandom(2.0*PI);
                    particle->velocity = (ofVec3f(cos(angle),sin(angle),0.0).limit(1)*particle->mass*escapeVelocity);
                }
            }
            
            //reel em back in if they go too far
            if((particle->position-center).length()>center.length())
                particle->position=center;
            
            //do my own physics integration
            ofVec3f att = (particle->position-center).limit(attractionLimit);
            particle->velocity-=att;
            particle->velocity.limit(10);
            particle->position+=particle->velocity;
        }
    }
    
    if(s==rubberBandit)
    {
        for(int i = 0; i < rubberBandit->numberOfAttractions(); i++)
            rubberBandit->getAttraction(i)->setStrength(force);
        
        for(int i = 0; i< rubberBandit->numberOfSprings(); i++)
        {
            Spring* spring = rubberBandit->getSpring(i);
            if(spring->getOneEnd()->fixed || spring->getTheOtherEnd()->fixed)
                spring->setRestLength(distance);
            else
                spring->setRestLength(distance/12.5);
        }
    }

}

void testApp::drawParticles(ParticleSystem* s)
{
    if(s!=bounceGrid)
    {
        for(int i = 0; i < s->numberOfParticles(); i++)
        {
                Particle *part = s->getParticle(i);
                ofCircle(part->position, part->radius);
        }
        
        for(int i = 0; i < s->numberOfSprings(); i++)
        {
                Spring *spring = s->getSpring(i);
                ofLine(spring->getOneEnd()->position, spring->getTheOtherEnd()->position);
        }
    }
    
    if(s == rubberBandit)
    {
        ofVec3f pos3 = rubberBandit->getParticle(0)->position;
        for(int i = 1; i < rubberBandit->numberOfParticles(); i++)
        {
            ofVec3f pos2;
            ofVec3f pos1 = rubberBandit->getParticle(i)->position;
            if(i<rubberBandit->numberOfParticles()-1)
                pos2 = rubberBandit->getParticle(i+1)->position;
            else
                pos2 = rubberBandit->getParticle(1)->position;
            
            if(fmod(i,2.0)==0.0)
                ofSetColor(color,128);
            else
                ofSetColor(255,128);
            ofTriangle(pos1, pos2, pos3);
        }
    }
    
    if(s == bounceGrid)
    {
        for(int i = 0; i <= (s->numberOfParticles()-1)/2.0; i++)
        {
            Particle *part = s->getParticle(i);
            ofCircle(part->position, part->radius);
        }
        
        for(int i = 0; i < s->numberOfSprings()-(s->numberOfParticles()-1)/2.0; i++)
        {
            Spring *spring = s->getSpring(i);
            ofLine(spring->getOneEnd()->position, spring->getTheOtherEnd()->position);
        }
        
    }
}

float testApp::angleBetween(ofVec3f vec1, ofVec3f vec2)
{
    ofVec2f xAxis(1, 0);
    float angle = xAxis.angle(vec2 - vec1);
    return angle;
}

