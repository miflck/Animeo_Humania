//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Sun.hpp"
#include "ofxEasing.h"

Sun::Sun(){
    
}

Sun::~Sun(){
    
}

void Sun::setup(){
    actualRadius=200;
}

void Sun::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
}





void Sun::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,0);
    ofSetLineWidth(strokeWeight);
    ofTranslate(getPosition().x, getPosition().y);
    ofPushMatrix();
    ofVec2f ray=ofVec2f(actualRadius,0);
    for(int i=0;i<360;i+=5){
        ray.rotate(5);
        ofDrawLine(0, 0, ray.x, ray.y);
    }
    ofPopMatrix();
    ofSetColor(0);
    ofDrawCircle(0,0,60);
    ofSetColor(255,255,0);
    ofDrawCircle(0,0,50);
    ofPopStyle();
    ofPopMatrix();
}


