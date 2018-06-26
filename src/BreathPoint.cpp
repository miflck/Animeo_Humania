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
    ofDrawEllipse(getPosition().x, getPosition().y, actualRadius+r, actualRadius+v);
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

float BreathPoint::getRadius(){
    return actualRadius/2;
}

void BreathPoint::setReflectionPoint(ofVec2f _r){
    reflectionpoint.set(_r);
}

void BreathPoint::setReflection(bool _r){
    bReflect=_r;
}

