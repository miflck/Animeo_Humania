//
//  EmotionWorld.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "EmotionWorld.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


EmotionWorld::EmotionWorld(){
    init();
}


EmotionWorld::~EmotionWorld(){
    
}

void EmotionWorld::init(){
    cout<<"init EmotionWorld"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, -10);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    anchor.setPhysics(50, 0.5, 0.9);
    anchor.setup(box2d.getWorld(), 0, 0, 50);
    box.setup(box2d.getWorld(), ofGetWidth()/2, -20, ofGetWidth(), 20);
    leftbox.setup(box2d.getWorld(), 0, 150,20,300);
    rightbox.setup(box2d.getWorld(), ofGetWidth(),150, 20,300);

    
    screen.allocate(1920,1080, GL_RGB);
    screen.begin();
    ofClear(0,0,0,0);
    screen.end();
    
    herz.loadImage("herz.png");
    
    sun.setup();
    sun.bSeekTarget=true;
    
}

void EmotionWorld::update(){
    box2d.setGravity(0, APPC->gui->emotionsgravity);
    box2d.update();
    
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()<=0){
    sun.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    }else{
        ofVec2f hand;
        hand=mskel[0].leftHand;
        hand=ofVec2f(hand.x, hand.y);
        sun.setTarget(hand);

    }
    sun.update();
    
    for(int i=0;i<movingObjects.size();i++){
        movingObjects[i].setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        movingObjects[i].update();
    }
    
    if(mskel.size()>0){
        ofVec2f head;
        head=mskel[0].head;
        head=ofVec2f(head.x, head.y);

        anchor.setPosition(mskel[0].leftHand.x, mskel[0].leftHand.y);
        
       // box2d.setGravity(0, (mskel[0].head.y-mskel[0].rightHand.y)/10);

    
        float rAdd=ofRandom(1);
        if(rAdd>0.7 && bEmitHearts){
            float r = ofRandom(10, 40);        // a random radius 4px - 20px
            circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
            circles.back().get()->setPhysics(3.0, 0.8, 0.1);
            circles.back().get()->setup(box2d.getWorld(), head.x,head.y, r);
            circles.back().get()->setVelocity(ofRandom(-10,10), ofRandom(-10,-10));
        }
    }
    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
  
}


void EmotionWorld::draw(){
   
    if(APPC->debug){
        ofPushStyle();
        for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
            circles[i].get()->draw();
        }
        ofSetColor(255,0,0);
        anchor.draw();
        
        leftbox.draw();
        rightbox.draw();
        ofPopStyle();
    }
    ofPushStyle();

    
    sun.draw();
    
    for(int i=0; i<circles.size(); i++) {
        ofSetColor(220+ofRandom(-20,20),37+ofRandom(-20,20),151+ofRandom(-20,20));
        ofPushMatrix();
        ofTranslate(circles[i]->getPosition().x,circles[i]->getPosition().y);
        ofRotate(circles[i]->getRotation());
        herz.draw(-circles[i]->getRadius(),-circles[i]->getRadius(),circles[i]->getRadius()*2,circles[i]->getRadius()*2);
        ofPopMatrix();
    }
    ofPopStyle();

    for(int i=0;i<movingObjects.size();i++){
        movingObjects[i].draw();
    }
    ofPopStyle();
    
   /*   screen.begin();

  ofPushStyle();
    ofSetColor(255,100);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 10);
    screen.end();
   
    screen.draw(0,0);
   */
    
}

void EmotionWorld::exit(){
    cout<<"exit EmotionWorld"<<endl;
}


void EmotionWorld::toggleHearts(){
    bEmitHearts=!bEmitHearts;
}


//KEY LISTENER
//--------------------------------------------------------------
void EmotionWorld::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'e') {
        
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
        ofVec2f head=ofVec2f(ofGetMouseX(), ofGetMouseY());
        if(mskel.size()>0){
            head=mskel[0].head;
            head=ofVec2f(head.x, head.y);
        }
        
        cout<<"Add Circles"<<endl;
        float r = ofRandom(10, 40);        // a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        circles.back().get()->setVelocity(ofRandom(-5,5), ofRandom(-1,-5));

    }
    
    
    if(e.key == 'o') {
        MovingObject m;
        m.bSeekTarget=true;
        m.setRadius(10);
        m.setPosition(ofGetWidth()/2, 0);
        m.setTarget(ofVec2f(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight())));
        movingObjects.push_back(m);
    }
    
    if(e.key == 'p') {
 
        for(int i=0;i<movingObjects.size();i++){
            ofVec2f r=ofVec2f(ofRandom(50),ofRandom(50));
            r.rotate(ofRandom(360));
            movingObjects[i].applyForce(r);
        }
    }
    
    if(e.key=='h'){
        toggleHearts();
    }
    
}


//--------------------------------------------------------------
void EmotionWorld::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void EmotionWorld::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void EmotionWorld::mousePressed(ofMouseEventArgs &a){
   

}

//--------------------------------------------------------------
void EmotionWorld::mouseReleased(ofMouseEventArgs &a){
    
}

void EmotionWorld::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void EmotionWorld::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void EmotionWorld::mouseExited(ofMouseEventArgs &a){
    
}


void EmotionWorld::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void EmotionWorld::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void EmotionWorld::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
