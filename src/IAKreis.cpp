//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "IAKreis.hpp"
#include "ofxEasing.h"

IAKreis::IAKreis(){
    
}

IAKreis::~IAKreis(){
    anchor.destroy();
}

void IAKreis::setup(){
    radiusTarget=100;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.setup(world, 0, 0, radiusTarget);
    actualRadius=0;
    easingInitTime = ofGetElapsedTimef();
    actualRadius=20;
    state=MOVINGOBJECT;
    stateBefore=START;
    color=ofColor(255);
    fadeAlpha=255;
    
    
     fadeInitTime= ofGetElapsedTimef();
    fadeTarget=255;
    fadeDuration=3.0f;
    setMaxSpeed(10);
    
}

void IAKreis::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto endFadeTime = fadeInitTime + fadeDuration;

    auto now = ofGetElapsedTimef();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
    
    fadeAlpha = ofxeasing::map_clamp(now, fadeInitTime, endFadeTime, fadeAlpha, fadeTarget, &ofxeasing::linear::easeIn);
    color=ofColor(color,fadeAlpha);

    
    
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
            //setPosition(anchor.getPosition().x, anchor.getPosition().y);
            move();
            if(getPosition().y<actualRadius){
                bShouldRemove=true;
            }
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

void IAKreis::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition().x, getPosition().y);
    ofSetColor(color);
    ofDrawCircle(0,0,actualRadius);
    ofPopStyle();
    ofPopMatrix();
}

void IAKreis::setWorld(b2World* w){
    world=w;
}


void IAKreis::setRadiusTarget(float r){
    radiusTarget=r;
}

void IAKreis::turnPhysicsOn(bool _on){
    bPhysics=_on;
    if(bPhysics){
        anchor.setPosition(getPosition());
        anchor.setPhysics(50, 0.5, 0.6);
        anchor.body->SetType(b2_dynamicBody);
    }
}

bool IAKreis::getIsPhysicsOn(){
    return bPhysics;
}

void IAKreis::setState(int _state){
    stateBefore=state;
    state=_state;
    switch (state) {
        case START:
            break;
        case MOVINGOBJECT:
            anchor.setPhysics(0, 0.5, 0.4);
            anchor.body->SetType(b2_kinematicBody);
            break;
        case PHYSICS:
            cout<<"State "<<state<<endl;
           /* anchor.setPosition(getPosition());
            anchor.setPhysics(50, 0.5, 0.4);
            anchor.body->SetType(b2_dynamicBody);*/
            setSpeed(getSpeedBefore());
            setTarget(ofVec2f(getPosition().x,-200));
            break;
        case FADEOUT:
            fadeInitTime=ofGetElapsedTimef();
            fadeTarget=0;
            break;
            
        default:
            break;
    }
}

int IAKreis::getState(){
    return state;
}
