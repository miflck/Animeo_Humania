//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Baloon.hpp"
#include "ofxEasing.h"

Baloon::Baloon(){
    
}

Baloon::~Baloon(){
    
}

void Baloon::setup(){
    actualRadius=200;
    bWander=true;
    wanderforce=0.1;
    setSeekForce(0.01);

    bSeekTarget=true;
    
    setSpeed(0,-1);
    setMaxSpeed(5);

}

void Baloon::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
}





void Baloon::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,0);
    ofSetLineWidth(strokeWeight);
    ofTranslate(getPosition().x, getPosition().y);
    ofPushMatrix();
   
    ofPopMatrix();
    ofSetColor(255,0,0);

    ofDrawEllipse(0,0,130,150);
    ofPopStyle();
    ofPopMatrix();
}


