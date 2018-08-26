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
    
    
     startLerp=0;
    actualLerp=0;
     lerpInitTime= ofGetElapsedTimef();
     lerpTarget=1;
     lerpDuration;
    
    
     lerpToColor=ofColor(0,255,0);;
    startColor=ofColor(255,0,0);
    
    
}

void AvatarKreis::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto endFadeTime = fadeInitTime + fadeDuration;
    auto endLerpTime=lerpInitTime+lerpDuration;

    auto now = ofGetElapsedTimef();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, startRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
    actualLerp=ofxeasing::map_clamp(now, lerpInitTime, endLerpTime, startLerp, lerpTarget, &ofxeasing::linear::easeIn);
    //color=ofColor(color,fadeAlpha);

    
    //cout<<"fade"<<fadeAlpha<<endl;
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    // check if reached
    ofVec2f distance=getTarget()-getPosition();
    if(distance.length()<1 && getSpeed().length()< 0.1)setReached(true);
    
    
 
    ofColor c=startColor;
    c.lerp(lerpToColor, actualLerp); // now purple!
    
    switch (state) {
            
        case IDLE:
            break;
            
        case START:
            //move();
            break;
            
        case MOVINGOBJECT:
            move();
            color=c;

            break;
            
        case RELEASED:
            //setPosition(anchor.getPosition().x, anchor.getPosition().y);
            move();
            color=c;

          
            
            if(getPosition().y<actualRadius){
                bShouldRemove=true;
            }
            break;
            
        case FADEOUT:
          //  if(fadeAlpha<1)bShouldRemove=true;
            if(stateBefore==MOVINGOBJECT){
             //   move();
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


void AvatarKreis::setScaleDuration(float t){
    scaleDuration=t;
}


void AvatarKreis::setRadiusTarget(float r){
    radiusTarget=r;
}

void AvatarKreis::setStartRadius(float r){
    startRadius=r;
}

void AvatarKreis::setActualRadius(float r){
    actualRadius=r;
}

void AvatarKreis::setRadiusTargetWithStartRadius(float r, float rs){
    radiusTarget=r;
    startRadius=rs;
}

void AvatarKreis::updateRadiusTarget(float r){
    radiusTarget=r;
    startRadius=getActualRadius();
}

float  AvatarKreis::getActualRadius(){
    return actualRadius;
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
        case IDLE:
            setSpeed(0, 0);
            setActualRadius(0);
            setStartRadius(0);
            setRadiusTarget(0);

            break;
            
        case START:
            
            break;
        case MOVINGOBJECT:
           
            lerpInitTime= ofGetElapsedTimef();

            break;
        case RELEASED:
            cout<<"State "<<state<<endl;
            lerpInitTime= ofGetElapsedTimef();

           /* anchor.setPosition(getPosition());
            anchor.setPhysics(50, 0.5, 0.4);
            anchor.body->SetType(b2_dynamicBody);*/
            //setSpeed(getPositionDifference());
           // setTarget(ofVec2f(getPosition().x,-200));
            break;
        case FADEOUT:
            color=ofColor(0,0);

            break;
            
        default:
            break;
    }
}

int AvatarKreis::getState(){
    return state;
}


