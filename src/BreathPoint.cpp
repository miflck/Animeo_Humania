//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "BreathPoint.hpp"
#include "ofxEasing.h"

BreathPoint::BreathPoint(){
    
}

BreathPoint::~BreathPoint(){
    
}

void BreathPoint::setup(){
    radius=50;
    scaleDuration=1.f;
    actualRadius=0;
    easingInitTime = ofGetElapsedTimef();
    radiusTarget=50;
    reflectionpoint=ofVec2f(ofGetWidth()/2,ofGetHeight()/2);
    initTime=ofGetElapsedTimeMillis();


}

void BreathPoint::update(){
    
    
   
    
   /* auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    */
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
}





void BreathPoint::draw(){
    
  /*  ofPushMatrix();
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
   */
    
    
    if(bReflect){
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255);
    
    
    int dist=150;
    
    if(getDistance(reflectionpoint).length()<dist){
        float f;
        float a;
        if(getPosition().x-reflectionpoint.x<0){
             f=ofMap(getPosition().x-reflectionpoint.x, -dist, 0, 0, 360);
             a=ofMap(getPosition().x-reflectionpoint.x, -dist, 0, 90, 0);
            
        }else{
             f=ofMap(getPosition().x-reflectionpoint.x, 0, dist, 360, 0);
             a=ofMap(getPosition().x-reflectionpoint.x, 0, dist, -360, -90);
        }
        
        for(int i=0;i<f;i++){
        ofVec2f v(0,-1);
        v.normalize();
        v*=-800;
            v.rotate(i+a);
        ofDrawLine(getPosition(),getPosition()+v);
        }
    }
    ofPopStyle();
    ofPopMatrix();
    }
    
    
    float r=0;
    float v=0;
    if(bBreathe){
        r=4.f*sin(ofGetElapsedTimef()*2);
        v=2.f*cos(ofGetElapsedTimef()*2);
    }
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255);
    ofTranslate(getPosition().x, getPosition().y);
    //ofDrawEllipse(getPosition().x, getPosition().y, actualRadius+r, actualRadius+v);
    ofDrawEllipse(0,0, actualRadius+r, actualRadius+v);
    
    
    
    switch (state) {
        case FREE:
            break;
        case SCARED:
            break;
        case HOME:
            break;
        case INSIDE:
            break;
        case WATCH:
         
           
            
            break;
            
            
        default:
            break;
    }
    
    
    if(bWatch){
        
        ofPushStyle();
        ofSetLineWidth(3);
        ofSetColor(0);
        
        ofPushMatrix();
        int angle=90;
        for(int i=0;i<4;i++){
            ofRotate(angle);
            ofDrawLine(actualRadius/2, 0, actualRadius/2-10, 0);
        }
        ofPopMatrix();
        
        angle=30;
        ofPushMatrix();
        
        for(int i=0;i<12;i++){
            ofRotate(angle);
            ofDrawLine(actualRadius/2, 0, actualRadius/2-5, 0);
        }
        
        ofPopMatrix();
        
        ofPushMatrix();
        ofRotate(-30);
        ofDrawLine(-5, 0, actualRadius/2, 0);
        ofRotate(-110);
        ofDrawLine(-5, 0, actualRadius/3, 0);
        ofPopMatrix();
        
        ofPushMatrix();
        
        int t=ofGetElapsedTimeMillis();
        int time=t/1000;
        time=time%60;
        int a=int(ofMap(time, 0, 60, 0, 360));
        ofRotate(a);
        ofSetLineWidth(3);
        
        ofDrawLine(0, 0, actualRadius/2, 0);
        
        ofDrawCircle(0, 0, 2);
        ofPopMatrix();
        ofPopStyle();
     
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    if(bReflect){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(5);
        ofDrawEllipse(reflectionpoint,100,300);
      //  ofDrawRectangle(reflectionpoint, 800,800);
        ofPopStyle();
        ofPopMatrix();
    }
    
}



bool BreathPoint::getWatch(){
    return bWatch;
}

void BreathPoint::setWatch(bool _b){
    bWatch=_b;
    if(_b){
        setState(WATCH);
    }
    else {
        setState(FREE);
    };
}

float BreathPoint::getRadius(){
    return actualRadius/2;
}

void BreathPoint::setReflectionPoint(ofVec2f _r){
    reflectionpoint.set(_r);
}

void BreathPoint::setReflection(bool _r){
    bReflect=_r;
}


void BreathPoint::setTarget(ofVec2f _target){
    MovingObject::setTarget(_target);
}


void BreathPoint::setState(int _state){
    stateBefore=state;
    state=_state;
    switch (state) {
        case FREE:
            break;
        case SCARED:
            mover.scaleTo(size1,0.5);

            break;
        case HOME:
            break;
        case INSIDE:
            break;
        case WATCH:
            break;
       
            
        default:
            break;
    }
}

int BreathPoint::getState(){
    return state;
}

