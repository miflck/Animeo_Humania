//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "EmotionEllipse.hpp"
#include "ofxEasing.h"

EmotionEllipse::EmotionEllipse(){
    
}

EmotionEllipse::~EmotionEllipse(){
    
}

void EmotionEllipse::setup(){
    actualRadius=0.2;
    radiusTarget=1;
    scaleDuration=20;
    
    bWander=false;
    wanderforce=0.0;
    setSeekForce(0.5);

    bSeekTarget=true;
    
    setSpeed(0,-1);
    setMaxSpeed(5);

}

void EmotionEllipse::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
  //  wanderforce=ofRandom(0,1);
//    setSeekForce(1-wanderforce);

    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
    //cout<<actualRadius<<endl;
    
    ofVec2f s=getSpeed();
    float a=s.angle(ofVec2f(0,-1));
    a=ofMap(a,-90,90,-20,20,true);
    a=ofMap(s.length(),0,5,0,a,true);

    triangleAngleTarget=a;
    actualTriangleAngle = ofLerp(actualTriangleAngle, triangleAngleTarget, 0.1);

    
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
}



void EmotionEllipse::startEasingIn(){
    actualRadius=0.2;
    easingInitTime=ofGetElapsedTimef();
}

void EmotionEllipse::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,0);
    ofSetLineWidth(strokeWeight);
    ofTranslate(getPosition().x, getPosition().y);
    ofPushMatrix();
    ofScale(actualRadius, actualRadius);

    ofSetColor(255,0,0);

    ofDrawEllipse(0,0,130,150);
    ofTranslate(0,75);
   
    ofRotate(-actualTriangleAngle);
    ofDrawTriangle(0, 0, 20, 30, -20, 30);
    ofPopMatrix();

    ofPopStyle();
    ofPopMatrix();
}

