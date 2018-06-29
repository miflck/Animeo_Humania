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
    ofSetColor(0);
    ofDrawEllipse(0,0,60,80);
    ofSetColor(255,255,0);
    ofDrawCircle(0,0,50);
    ofPopStyle();
    ofPopMatrix();
}


