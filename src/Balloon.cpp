//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Balloon.hpp"
#include "ofxEasing.h"

Balloon::Balloon(){
    
}

Balloon::~Balloon(){
    
}

void Balloon::setup(){
    actualRadius=0.2;
    radiusTarget=1;
    scaleDuration=20;
    
    bWander=true;
    wanderforce=0.0;
    setSeekForce(0.5);

    bSeekTarget=true;
    
    setSpeed(0,-1);
    setMaxSpeed(8);

}

void Balloon::update(){
    
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



void Balloon::startEasingIn(){
    actualRadius=0.2;
    easingInitTime=ofGetElapsedTimef();
}

void Balloon::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,0);
    ofSetLineWidth(strokeWeight);
    ofTranslate(getPosition().x, getPosition().y);
    ofPushMatrix();
    ofScale(actualRadius, actualRadius);

    ofSetColor(255);

    ofDrawEllipse(0,0,150,150);
    ofTranslate(0,75);
   
    ofRotate(-actualTriangleAngle);
    ofDrawTriangle(0, 0, 20, 30, -20, 30);
    ofPopMatrix();

    ofPopStyle();
    ofPopMatrix();
}


