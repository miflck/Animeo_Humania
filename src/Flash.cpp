//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Flash.hpp"
#include "ofxEasing.h"

Flash::Flash(){
    
}

Flash::~Flash(){
    
}

void Flash::setup(){
    actualRadius=200;
    flashImage.load("flash.png");
}


void Flash::setup(ofImage *img){
    actualRadius=200;
    flashImage=*img;
}

void Flash::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
}





void Flash::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,255);
    ofTranslate(getPosition().x, getPosition().y);
    ofVec2f s =getSpeed();
    float angle = atan2(s.y, s.x) + PI/2;
    ofPushMatrix();
    ofRotate(ofRadToDeg(angle));
    flashImage.draw(-flashImage.getWidth()/2,-flashImage.getHeight());
    ofPopMatrix();
      ofPopStyle();
    ofPopMatrix();
}


