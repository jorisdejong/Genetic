#include "testApp.h"

void testApp::reset()
{
    escapeVelocity->clear();
    escapeVelocity->setBounds(0, 0, ofGetWidth(), ofGetHeight());
    addParticles(escapeVelocity);
    escapeVel=0.0;
    sendFloatMessage(escapeVel, "/escape/velocity");
    attractionLimit = 0.1;
    sendFloatMessage(attractionLimit, "/escape/gravity");
    escapeVelocityOpacity = 0.0;
    sendFloatMessage(escapeVelocityOpacity, "/escape/opacity");
    
    rubberBandit->clear();
    rubberBandit->setBounds(0, 0, ofGetWidth(), ofGetHeight());
    Particle* banditBoss = rubberBandit->makeParticle(50, center, 10);
    banditBoss->makeFixed();
    addParticles(rubberBandit);
    distance = 0.0;
    sendFloatMessage(distance, "/rubber/distance");
    force = 0.0;
    sendFloatMessage(force, "/rubber/force");
    rubberBanditOpacity = 0.0;
    sendFloatMessage(rubberBanditOpacity, "/rubber/opacity");
    
    bounceGrid->clear();
    bounceGrid->setBounds(0, 0, ofGetWidth(), ofGetHeight());
    Particle* bounceBoss = bounceGrid->makeParticle(50,center,0);
    bounceBoss->makeFixed();
    addParticles(bounceGrid);
    tension = 1.5;
    sendFloatMessage(tension, "/grid/tension");
    m.setAddress("/grid/xy");
    m.addFloatArg(0.5);
    m.addFloatArg(0.5);
    oscOut.sendMessage(m);
    m.clear();
    repulsion = 0.0;
    sendFloatMessage(repulsion, "/grid/repulse");
    bounceGridOpacity = 0.0;
    sendFloatMessage(bounceGridOpacity, "/grid/opacity");
    
    society->clear();
    society->setBounds(0, 0, ofGetWidth(), ofGetHeight());
    societyOpacity = 0.0;
    sendFloatMessage(societyOpacity, "/society/opacity");
    addParticles(society);
    m.setAddress("/society/xy");
    m.addFloatArg(0.5);
    m.addFloatArg(0.5);
    oscOut.sendMessage(m);
    m.clear();
    socForce = 0.0;
    sendFloatMessage(socForce, "/society/force");
    
    resetTime = ofGetElapsedTimef();
    
}
//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofEnableSmoothing();
    ofBackground(0);
    
    //osc
    osc.setup(8000);
    oscOut.setup("192.168.1.14", 7001);
    
    //handy var init
    center = ofVec3f(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0);
    color = ofColor(255,96,0);
    
    //escapeVelocity setup
    escapeVelocity = new ParticleSystem(0.0,0.01);
    
    

    
    //rubberBandit setup
    rubberBandit = new ParticleSystem(0.0,0.01);
    
    
    //bounceGrid setup
    bounceGrid = new ParticleSystem(0.0,0.01);

    
    //coSign setup
    coSign = new ParticleSystem(0.0, 0.01);
    coSign->clear();
    coSignBossCount=5;
    for(int i = 0; i < coSignBossCount; i++)
    {
        ofVec2f pos = center;
        pos.x = float(ofGetWidth())/float(coSignBossCount+1.0) * (i+1);
        
        Particle* coSignBoss = coSign->makeParticle(50, pos, 10);
        coSignBoss->makeFixed();
    }
    addParticles(coSign);
    coSignOpacity=0.0;
    
    //society setup
    society = new ParticleSystem(0.0,0.01);

    reset();
    
    //gui
    gui = new ofxUICanvas(10,10,200,ofGetHeight()-20);
    gui->addFPS();
//    gui->addSlider("Escape Velocity",0.0, 1.0, escapeVelocityOpacity);
//    gui->addSlider("Velocity", 0.0, 5.0, escapeVel);
//    gui->addSlider("Gravity",0.0,0.15,attractionLimit);
//    //gui->addSpacer();
//    gui->addSlider("Rubber Bandit", 0.0, 1.0, rubberBanditOpacity);
//    gui->addSlider("Force",-100.0,100.0,force);
//    gui->addSlider("Distance",0.0,1000.0,distance);
//    //gui->addSpacer();
//    gui->addSlider("Bounce Grid", 0.0, 1.0, bounceGridOpacity);
//    gui->add2DPad("Mouse", ofPoint(0,200), ofPoint(0,50), ofPoint(100,25), 200, 50);
//    gui->addSlider("Tension",0.0,2.0,tension);
//    gui->addSlider("Repulsion",0.0,-50.0,repulsion);
//    gui->addSlider("Society", 0.0, 1.0, societyOpacity);

    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    //gui->addSlider("k",0.0,1.0,0.0);
    //gui->addSlider("d",0.0,1.0,0.0);
    //gui->addSlider("rl",0.0,100.0,10.0);
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    
    //syphon
    server.setName("Genetic");
    

    





}

//--------------------------------------------------------------
void testApp::update(){
    
    if(escapeVelocityOpacity!=0.0)
        updateParticles(escapeVelocity);

    if(rubberBanditOpacity!=0.0)
        updateParticles(rubberBandit);

    if(bounceGridOpacity!=0.0 )
        updateParticles(bounceGrid);
    
    if(coSignOpacity!=0.0)
        updateParticles(coSign);
    
    if(societyOpacity!=0.0 || ofGetElapsedTimef()-resetTime<5.0) //give society five seconds for positioning
        updateParticles(society);
    
    while(osc.hasWaitingMessages())
    {
        osc.getNextMessage(&m);
        string add = m.getAddress();
        //cout<<add<<endl;
        if(add == "/escape/opacity")
            escapeVelocityOpacity =  m.getArgAsFloat(0);
        if(add == "/escape/velocity")
            escapeVel =  m.getArgAsFloat(0);
        if(add == "/escape/gravity")
            attractionLimit =  m.getArgAsFloat(0);
        if(add == "/rubber/opacity")
            rubberBanditOpacity = m.getArgAsFloat(0);
        if(add == "/rubber/force")
            force =  m.getArgAsFloat(0);
        if(add == "/rubber/distance")
            distance =  m.getArgAsFloat(0);
        if(add == "/grid/opacity")
            bounceGridOpacity = m.getArgAsFloat(0);
        if(add == "/grid/repulse")
            repulsion = m.getArgAsFloat(0);
        if(add == "/grid/tension")
            tension = m.getArgAsFloat(0);
        if(add == "/grid/xy")
        {
            ofVec3f value = ofVec3f(m.getArgAsFloat(1)*ofGetWidth(), m.getArgAsFloat(0)*ofGetHeight(), 0);
            bounceGrid->getParticle(0)->position=value;
        }
        if(add == "/society/xy")
        {
            ofVec3f value = ofVec3f(m.getArgAsFloat(1)*ofGetWidth(), m.getArgAsFloat(0)*ofGetHeight(), 0);
            boss->position=value;
        }
        if(add == "/society/force")
            socForce = m.getArgAsFloat(0);
        if(add == "/society/opacity")
            societyOpacity = m.getArgAsFloat(0);
        if(add == "/reset")
            reset();
    }


}

//--------------------------------------------------------------
void testApp::draw(){

    
    //draw each system

    if(escapeVelocityOpacity!=0.0)
        drawParticles(escapeVelocity);
    if(rubberBanditOpacity!=0.0)
        drawParticles(rubberBandit);
    if(bounceGridOpacity!=0.0)
        drawParticles(bounceGrid);
    if(coSignOpacity!=0.0)
        drawParticles(coSign);
    if(societyOpacity)
        drawParticles(society);


    
    //draw a center circle
    ofFill();
    ofSetColor(color,255);
    ofCircle(center,10);
    ofNoFill();
    ofSetColor(255,255);
    ofCircle(center,10);
    
    server.publishScreen();
    
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

    if(name =="Velocity")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        escapeVel = value;        
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
    
    else if (name=="Tension")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        tension = value;
    }
    else if (name=="Escape Velocity")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        escapeVelocityOpacity = value;
    }
    else if (name=="Rubber Bandit")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        rubberBanditOpacity = value;
    }
    else if (name=="Bounce Grid")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        bounceGridOpacity = value;
    }
    else if (name=="Escape Velocity")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        escapeVelocityOpacity = value;
    }
    
    else if (name=="Society")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        societyOpacity = value;
    }
    
    else if (name=="Mouse")
    {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        ofVec3f value = pad->getPercentValue();
        value.x*=ofGetWidth();
        value.y*=ofGetHeight();
        if(bounceGridOpacity>0.0)
            bounceGrid->getParticle(0)->position=ofVec3f(value);
        if(societyOpacity>0.0)
            boss->position=ofVec3f(value);
    }
    
    else if (name=="Repulsion")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        repulsion = value;
    }
    
    else if (name=="k")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        for(int i = 0; i < escapeVelocity->numberOfSprings();i++)
        {
            escapeVelocity->getSpring(i)->setStrength(value);
        }
    }
    else if (name=="d")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        for(int i = 0; i < escapeVelocity->numberOfSprings();i++)
        {
            escapeVelocity->getSpring(i)->setDamping(value);
        }
    }
    else if (name=="rl")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        float value = slider->getScaledValue();
        for(int i = 0; i < escapeVelocity->numberOfSprings();i++)
        {
            escapeVelocity->getSpring(i)->setRestLength(value);
        }
    }
    
}

void testApp::addParticles(ParticleSystem* s)
{
    if(s == escapeVelocity)
    {
        //create 3000 particles ready for recycling, using simple positioning, Traer is too much for this
        for(int i = 0; i < 1000; i++)
        {
            //place them in a circle
            float angle = ofRandom(2.0*PI);
            ofVec3f pos = center+(ofVec3f(cos(angle),sin(angle),0.0)*5);
            float mass = ofRandom(1.0,2.0);
            Particle* p = escapeVelocity->makeParticle(mass, pos, mass*2);
            p->makeFixed();
            float k = 0.01;
            float d = 0.17;
            float rl = 10.0;
            
            if(fmod(i,2.0)==0 && i > 0)
            {
                Particle* p1 = escapeVelocity->makeParticle(mass, pos, mass*2);
                Spring* s1 = escapeVelocity->makeSpring(p, p1, k, d, rl);
                //p1->makeFixed();
                //Spring* s3 = escapeVelocity->makeSpring(p1, p2, k, d, rl);
            }
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
            float mass = fmod(i,2.0)*10.0+1;
            Particle* p = rubberBandit->makeParticle(mass, pos, 2.0);
            Attraction* a = rubberBandit->makeAttraction(rubberBandit->getParticle(0), p, force, 10.0);
        }
        
        //make springs between them and to the center
        for(int i = 0; i < 36; i++)
        {
            Spring* s = rubberBandit->makeSpring(rubberBandit->getParticle(0), rubberBandit->getParticle(i+1), 0.001, 1.0, (1.0)*distance);
            if(i<35)
                Spring* s = rubberBandit->makeSpring(rubberBandit->getParticle(i+1), rubberBandit->getParticle(i+2),0.01, 0.1, 1/12.5*distance);
            else
                Spring* s = rubberBandit->makeSpring(rubberBandit->getParticle(i+1), rubberBandit->getParticle(1),0.01, 0.1, 1/12.5*distance);
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
                if(pos==center || (x==0 && y ==0)  || (x==gridHorizontal-1 && y==0) || (x==0 && y==gridVertical-1) || (x==gridHorizontal-1 && y==gridVertical-1))
                    p->makeFixed();
                if(x>0)
                    Spring* s = bounceGrid->makeSpring(p, bounceGrid->getParticle(x+y*gridHorizontal), 0.1, tension, distX);
                if(y>0)
                    Spring* s = bounceGrid->makeSpring(p, bounceGrid->getParticle(x+1+(y-1)*gridHorizontal), 0.1, tension, distY);
                
                //connect them to the boss, which is mouse controlled
                Attraction* a = bounceGrid->makeAttraction(p, bounceGrid->getParticle(0), repulsion, 10);
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
                Spring* s = bounceGrid->makeSpring(pFixed, bounceGrid->getParticle(x+y*gridHorizontal+1), 0.01, 0.2, 0);
            
                
            }
        }
    }
    
    if(s == coSign)
    {
        //create 10000 particles ready for recycling
        for(int i = 0; i < 10000; i++)
        {
            //place them in a circle
            float angle = ofRandom(2.0*PI);
            ofVec3f pos = center+(ofVec3f(cos(angle),sin(angle),0.0)*100);
            float mass = ofRandom(1.0,2.0);
            Particle* p = coSign->makeParticle(mass, pos, mass*2);
            for(int i = 0; i < coSignBossCount; i++)
                Attraction* a = coSign->makeAttraction(p, coSign->getParticle(i), 0.1, 10);
        }
    }
    
    if(s == society)
    {
        //create 500 particles ready for recycling
        boss = society->makeParticle(50, center, 10);
        boss->makeFixed();
        for(int i = 0; i < 500; i++)
        {
            //float angle = (2.0*PI)/100*i;
            ofVec3f pos = ofVec3f(ofRandomWidth(),ofRandomHeight(),0.0);
            float mass = ofRandom(1.0,4.0);
            Particle* p = society->makeParticle(mass/10.0, pos, mass*2);
            Attraction* a = society->makeAttraction(p, boss, 10.0, 150);
            centerAttract.push_back(a);
            
        }
        for(int i = 0; i < society->numberOfParticles(); i++)
        {
            Particle* p = society->getParticle(i);
            if(!p->fixed)
            {
                for(int j = 0; j < society->numberOfParticles(); j++)
                {
                    Particle* p2 = society->getParticle(j);
                    if(!p2->fixed)
                    {
                        //add a slight negative force between each particle
                        Attraction* a = society->makeAttraction(p, p2, -20, 2.0);
                        a->setMax(60.0);
                        forceField.push_back(a);
                    }
                }

            }
        }
        
    }
    
}

void testApp::updateParticles(ParticleSystem* s)
{
    
    if(s==escapeVelocity)
    {
        escapeVelocity->tick();
        
        for(int i = 0; i < escapeVelocity->numberOfParticles(); i++)
        {
            Particle* particle = escapeVelocity->getParticle(i);
           // if(particle->fixed)
            {
                //if the particle are close to the center, shoot them out based on the current velocity value
                float dist = center.distance(particle->position);
                if(dist<10.0)
                {
                    particle->velocity=(particle->position-center).limit(1)*particle->mass*escapeVel;
                    //if they're stuck in the middle, shoot them out randomly
                    if(dist<=1.0)
                    {
                        float angle = ofRandom(2.0*PI);
                        particle->velocity = (ofVec3f(cos(angle),sin(angle),0.0).limit(1)*particle->mass*escapeVel);
                    }
//                    if(fmod(i,2.0)==0.0)
//                    {
//                        Particle* other = escapeVelocity->getParticle(i+1);
//                        ofVec3f random = ofVec3f(ofRandom(-1,1),ofRandom(-1,1),0.0);
//                        other->position=particle->position+random*5.0;
//                        other->velocity=particle->velocity+random*5.0;
//                    }
                }
                //do my own physics integration
                ofVec3f att = (particle->position-center).limit(attractionLimit);
                particle->velocity-=att;
                particle->velocity.limit(10);
                particle->velocity*=1.0-attractionLimit/10.0;
                particle->position+=particle->velocity;
                

            }
            //reel em back in if they go too far
            //if((particle->position-center).length()>center.length())
              //  particle->position=center;
            

            

        }
    }
    
    if(s==rubberBandit)
    {
        rubberBandit->tick();
        for(int i = 0; i < rubberBandit->numberOfAttractions(); i++)
            rubberBandit->getAttraction(i)->setStrength(force);
        
        for(int i = 0; i< rubberBandit->numberOfSprings(); i++)
        {
            Spring* spring = rubberBandit->getSpring(i);
            if(spring->getOneEnd()->fixed || spring->getTheOtherEnd()->fixed)
                spring->setRestLength(1.0*distance);
            else
                spring->setRestLength(1/12.5*distance);
        }
    }
    
    if(s==bounceGrid)
    {
        bounceGrid->tick();
        for(int i = 0; i< bounceGrid->numberOfSprings(); i++)
        {
            Spring* spring = bounceGrid->getSpring(i);
            if(!spring->getOneEnd()->fixed && !spring->getTheOtherEnd()->fixed)
                spring->setDamping(tension);
        }
        
        for(int i = 0; i < bounceGrid->numberOfAttractions(); i++)
            bounceGrid->getAttraction(i)->setStrength(repulsion);
    }
    
    if(s==coSign)
    {
        for(int i = 0; i < coSign->numberOfAttractions(); i++)
        {
            Attraction* a = coSign->getAttraction(i);
            Particle* boss;
            if(a->getOneEnd()->fixed) 
                boss = a->getOneEnd();
            else if(a->getTheOtherEnd()->fixed)
                boss = a->getTheOtherEnd();
            int j = boss->position.x / (ofGetWidth()/(float(coSignBossCount)+1.0));
            //cout<<j<<endl;
            a->setStrength((cos(ofGetElapsedTimef()+(float(j)))+1.0)*2.0);
        }
        coSign->tick();
    }
    
    if(s==society)
    {
        society->tick();
        for(int i =0; i<centerAttract.size(); i++)
        {
            //if(fmod(i,2.0)==0.0)
              //  force=-force;
            centerAttract[i]->setStrength(socForce);
        }
        
        //boss->position.set(ofNoise(ofGetElapsedTimef()/2.0)*ofGetWidth(),ofNoise(ofGetElapsedTimef()/3.0)*ofGetHeight()*2.0-ofGetHeight()/2.0, 0.0);
        //boss->position.set(ofGetMouseX(), ofGetMouseY(),0.0);
        //
//        for(int i = 0; i < society->numberOfParticles(); i++)
//        {
//            Particle* p = society->getParticle(i);
//            if(!p->fixed)
//            {
//                if(p->position.y > ofGetHeight()-p->radius)
//                {
//                    p->position.y=ofGetHeight()-p->radius;
//                    p->velocity.y*=-0.9;
//                }
//                if(p->position.y < 0+p->radius)
//                {
//                    p->position.y=0+p->radius;
//                    p->velocity.y*=-0.9;
//                }
//                if(p->position.x > ofGetWidth()-p->radius)
//                {
//                    p->position.x=ofGetWidth()-p->radius;
//                    p->velocity.x*=-0.9;
//                }
//                if(p->position.x < 0+p->radius)
//                {
//                    p->position.x=0+p->radius;
//                    p->velocity.x*=-0.9;
//                }
//            }
//            
//        }
    }

}

void testApp::drawParticles(ParticleSystem* s)
{
    ofFill();
    
    float opacity;
    if(s==escapeVelocity)
        opacity=escapeVelocityOpacity;
    else if(s==rubberBandit)
        opacity=rubberBanditOpacity;

    else if(s==bounceGrid)
        opacity=bounceGridOpacity;
    else if(s==coSign)
        opacity=coSignOpacity;
    else if(s==society)
        opacity=societyOpacity;

    
    
    if(s!=bounceGrid)
    {
        for(int i = 0; i < s->numberOfParticles(); i++)
        {
            if(fmod(i,2.0)==0.0)// && s != escapeVelocity)
                ofSetColor(color,128*opacity);
            else
                ofSetColor(255,128*opacity);
            
            Particle *part = s->getParticle(i);
            if(s!= society || !part->fixed)
                ofCircle(part->position, part->radius);
        }
        
        for(int i = 0; i < s->numberOfSprings(); i++)
        {
                ofSetColor(255,128*opacity);
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
                ofSetColor(color,128*opacity);
            else
                ofSetColor(255,128*opacity);
            ofTriangle(pos1, pos2, pos3);
        }
    }
    
    if(s == bounceGrid)
    {

        
        for(int i = 0; i <= (s->numberOfParticles()-1)/2.0; i++)
        {
            if(fmod(i,2.0)==0.0)
                ofSetColor(color,128*opacity);
            else
                ofSetColor(255,128*opacity);
            Particle *part = s->getParticle(i);
            ofCircle(part->position, part->radius);
        }
        
        for(int i = 0; i < s->numberOfSprings()-(s->numberOfParticles()-1)/2.0; i++)
        {
            ofSetColor(255,128*opacity);
            Spring *spring = s->getSpring(i);
            ofLine(spring->getOneEnd()->position, spring->getTheOtherEnd()->position);
        }
        
    }
    
//    if(s== society)
//    {
//        for(int i = 0; i < forceField.size(); i++)
//        {
//            Attraction* a = forceField[i];//getAttraction(i);
//            Particle* p1 = a->getOneEnd();
//            Particle* p2 = a->getTheOtherEnd();
//            if(p1->position.distance(p2->position)<10.0)
//                ofLine(p1->position, p2->position);
//        }
//    }
}

float testApp::angleBetween(ofVec3f vec1, ofVec3f vec2)
{
    ofVec2f xAxis(1, 0);
    float angle = xAxis.angle(vec2 - vec1);
    return angle;
}

void testApp::sendFloatMessage(float v, string s)
{
    m.clear();
    m.setAddress(s);
    m.addFloatArg(v);
    oscOut.sendMessage(m);
    m.clear();
}

