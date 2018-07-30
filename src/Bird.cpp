//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Bird.hpp"
#include "ofxEasing.h"

Bird::Bird(){
    
}

Bird::~Bird(){
    
}

void Bird::setup(){
    actualRadius=200;
    bWander=true;
    wanderforce=0.0;
    setSeekForce(0.5);

    bSeekTarget=true;
    
    setSpeed(0,-1);
    setMaxSpeed(5);

}

void Bird::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
  //  wanderforce=ofRandom(0,1);
//    setSeekForce(1-wanderforce);

    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
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





void Bird::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,0);
    ofSetLineWidth(strokeWeight);
    ofTranslate(getPosition().x, getPosition().y);
    ofPushMatrix();
   
    ofPopMatrix();
    ofSetColor(255,0,0);

    ofDrawEllipse(0,0,130,150);
    ofTranslate(0,75);
   
    ofRotate(-actualTriangleAngle);
    ofDrawTriangle(0, 0, 20, 30, -20, 30);

    ofPopStyle();
    ofPopMatrix();
}


