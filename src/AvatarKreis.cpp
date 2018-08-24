//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "AvatarKreis.hpp"
#include "ofxEasing.h"

AvatarKreis::AvatarKreis(){
    
}

AvatarKreis::~AvatarKreis(){
}

void AvatarKreis::setup(){
    radiusTarget=100;
    actualRadius=0;
    easingInitTime = ofGetElapsedTimef();
    state=MOVINGOBJECT;
    stateBefore=START;
    color=ofColor(255,0,0);
    fadeAlpha=255;
    
    
     fadeInitTime= ofGetElapsedTimef();
    fadeTarget=255;
    fadeDuration=3.0f;
    setMaxSpeed(10);
    startRadius=0;
    
}

void AvatarKreis::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto endFadeTime = fadeInitTime + fadeDuration;

    auto now = ofGetElapsedTimef();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, startRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
    
    fadeAlpha = ofxeasing::map_clamp(now, fadeInitTime, endFadeTime, fadeAlpha, fadeTarget, &ofxeasing::linear::easeIn);
    color=ofColor(color,fadeAlpha);

    
    
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
    
    switch (state) {
        case START:
            move();
            break;
            
        case MOVINGOBJECT:
            move();
            break;
            
        case RELEASED:
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
            }
            if(stateBefore==RELEASED){
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

void AvatarKreis::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition().x, getPosition().y);
    ofSetColor(color);
    ofDrawCircle(0,0,actualRadius);
    ofPopStyle();
    ofPopMatrix();
}




void AvatarKreis::setRadiusTarget(float r){
    radiusTarget=r;
}

void AvatarKreis::turnPhysicsOn(bool _on){
    bPhysics=_on;
    if(bPhysics){

    }
}

bool AvatarKreis::getIsPhysicsOn(){
    return bPhysics;
}

void AvatarKreis::setState(int _state){
    stateBefore=state;
    state=_state;
    switch (state) {
        case START:
            break;
        case MOVINGOBJECT:
        
            break;
        case RELEASED:
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

int AvatarKreis::getState(){
    return state;
}
