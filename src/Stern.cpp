//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Stern.hpp"
#include "ofxEasing.h"

Stern::Stern(){
    
}

Stern::~Stern(){
    anchor.destroy();
}

void Stern::setup(){
    actualRadius=0;
    radiusTarget=100;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.setup(world, 0, 0, actualRadius);

    easingInitTime = ofGetElapsedTimef();
    state=MOVINGOBJECT;
    stateBefore=START;
    color=ofColor(255);
    fadeAlpha=255;
    
    
     fadeInitTime= ofGetElapsedTimef();
    fadeTarget=255;
    fadeDuration=3.0f;
    sternImg.load("bilder/stern.png");
    screen.set(0,0,ofGetWidth(),ofGetHeight());

    
}


void Stern::setup(ofImage img){
    actualRadius=0;
    radiusTarget=100;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.setup(world, 0, 0, actualRadius);
    
    easingInitTime = ofGetElapsedTimef();
    state=MOVINGOBJECT;
    stateBefore=START;
    color=ofColor(255);
    fadeAlpha=255;
    
    
    fadeInitTime= ofGetElapsedTimef();
    fadeTarget=255;
    fadeDuration=3.0f;
    sternImg=img;
    screen.set(0,0,ofGetWidth(),ofGetHeight());
    
    
}


void Stern::setup(ofImage *img){
    actualRadius=0;
    radiusTarget=100;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.setup(world, 0, 0, actualRadius);
    
    easingInitTime = ofGetElapsedTimef();
    state=MOVINGOBJECT;
    stateBefore=START;
    color=ofColor(255);
    fadeAlpha=255;
    
    
    fadeInitTime= ofGetElapsedTimef();
    fadeTarget=255;
    fadeDuration=3.0f;
    sternImg=*img;
    screen.set(0,0,ofGetWidth(),ofGetHeight());
    
    
}




void Stern::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto endFadeTime = fadeInitTime + fadeDuration;

    auto now = ofGetElapsedTimef();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
    
    fadeAlpha = ofxeasing::map_clamp(now, fadeInitTime, endFadeTime, fadeAlpha, fadeTarget, &ofxeasing::linear::easeIn);
    color=ofColor(color,fadeAlpha);

   // anchor.setup(world, 0, 0, radiusTarget);
    if(actualRadius!=radiusTarget)anchor.setRadius(actualRadius);

    
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
    
    switch (state) {
        case START:
            move();
            anchor.setPosition(getPosition());
            break;
            
        case MOVINGOBJECT:
            move();
            anchor.setPosition(getPosition());
            break;
            
        case PHYSICS:
            setPosition(anchor.getPosition().x, anchor.getPosition().y);
            if(!bHasGravity)anchor.addForce(ofVec2f(-world->GetGravity().x,-world->GetGravity().y), 1);
            break;
            
        case FADEOUT:
            if(fadeAlpha<1)bShouldRemove=true;
            if(stateBefore==MOVINGOBJECT){
                move();
                anchor.setPosition(getPosition());
            }
            if(stateBefore==PHYSICS){
                setPosition(anchor.getPosition().x, anchor.getPosition().y);
            }
            break;
            
        default:
            break;
    }
    
  /*  if(bPhysics) {
        setPosition(anchor.getPosition().x, anchor.getPosition().y);
    }else{
        move();
        anchor.setPosition(getPosition());
    }*/

}

void Stern::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition().x, getPosition().y);
    ofSetColor(color);
    sternImg.draw(-actualRadius,-actualRadius,actualRadius*2,actualRadius*2);
    ofPopStyle();
    ofPopMatrix();
}

void Stern::setWorld(b2World* w){
    world=w;
}


void Stern::turnPhysicsOn(bool _on){
    bPhysics=_on;
    if(bPhysics){
        anchor.setPosition(getPosition());
        anchor.setPhysics(50, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
    }
}

bool Stern::getIsPhysicsOn(){
    return bPhysics;
}

void Stern::setGravity(bool _b){
    bHasGravity=_b;
}

void Stern::setState(int _state){
    stateBefore=state;
    state=_state;
    switch (state) {
        case START:
            break;
        case MOVINGOBJECT:
            anchor.setPhysics(0, 0.5, 0.9);
            anchor.body->SetType(b2_kinematicBody);
            break;
        case PHYSICS:
            anchor.setPosition(getPosition());
            anchor.setPhysics(1, 0.5, 0.9);
            anchor.body->SetType(b2_dynamicBody);
            break;
        case FADEOUT:
            fadeInitTime=ofGetElapsedTimef();
            fadeTarget=0;
            break;
            
        default:
            break;
    }
}

int Stern::getState(){
    return state;
}

void Stern::setTargetRadius(int _r){
    radiusTarget=_r;
}
