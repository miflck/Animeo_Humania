//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Dreieck.hpp"
#include "ofxEasing.h"

Dreieck::Dreieck(ofVec2f _a,ofVec2f _b,ofVec2f _c){
    a=ofVec2f(_a);
    b=ofVec2f(_b);
    c=ofVec2f(_c);
}

Dreieck::~Dreieck(){
    triangleAnchor.destroy();
}

void Dreieck::setup(){
    radiusTarget=1;
    actualRadius=0;
    easingInitTime = ofGetElapsedTimef();
    state=MOVINGOBJECT;
    stateBefore=START;
    color=ofColor(255);
    fadeAlpha=255;
    
    
     fadeInitTime= ofGetElapsedTimef();
    fadeTarget=255;
    fadeDuration=3.0f;
    

    center= getTriangleCenter(a, b, c);
    triangleAnchor.addTriangle(a, b, c);
    triangleAnchor.setPhysics(1.0, 0.3, 0.3);
    triangleAnchor.create(world);
    

}

void Dreieck::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto endFadeTime = fadeInitTime + fadeDuration;

    auto now = ofGetElapsedTimef();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    
    
    fadeAlpha = ofxeasing::map_clamp(now, fadeInitTime, endFadeTime, fadeAlpha, fadeTarget, &ofxeasing::linear::easeIn);
    color=ofColor(color,fadeAlpha);

    
    
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    ofPoint ct = triangleAnchor.getCentroid2D();

    
    switch (state) {
        case START:
            move();
            triangleAnchor.setPosition(getPosition());
            break;
            
        case MOVINGOBJECT:
            move();
            triangleAnchor.setPosition(getPosition());

            break;
            
        case PHYSICS:
            setPosition(triangleAnchor.getPosition().x-ct.x, triangleAnchor.getPosition().y-ct.y);
            break;
            
        case FADEOUT:
            if(fadeAlpha<1)bShouldRemove=true;
            if(stateBefore==MOVINGOBJECT){
                move();
                triangleAnchor.setPosition(getPosition());

            }
            if(stateBefore==PHYSICS){
                setPosition(triangleAnchor.getPosition().x, triangleAnchor.getPosition().y);

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

void Dreieck::draw(){
    triangleAnchor.draw();

    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition().x, getPosition().y);
    ofSetColor(color);
    ofSetColor(0,0,255);
    ofDrawCircle(0,0,10);
    ofPopStyle();
    ofPopMatrix();
    
    
    
    
    ofPushStyle();
    //ofSetColor(col);
    
    ofPoint ct = triangleAnchor.getCentroid2D();
    ofPushMatrix();
    ofTranslate(getPosition().x,getPosition().y);
    ofTranslate(ct.x,ct.y);
    ofRotate(triangleAnchor.getRotation());
    
    ofScale(actualRadius,actualRadius);
    ofDrawCircle(ct,5);

    ofTranslate(-ct.x,-ct.y);
    ofSetColor(255,0,0);
    ofFill();
   ofDrawTriangle(a, b,c);
    // ofSetColor(255,0,0);
    //  ofDrawCircle(center,5);
    // ofDrawCircle(b,5);
    ofDrawCircle(a,5);

   ofDrawCircle(b,5);
    ofDrawCircle(c,5);

    
    //  ofDrawCircle(0,0,radius);
    ofPopMatrix();
    ofNoFill();
    //ofSetColor(255);
    
    ofPopStyle();
    ofPushStyle();

     ct = triangleAnchor.getCentroid2D();
    ofFill();
    ofSetColor(0, 255, 0);
    ofDrawCircle(ct.x, ct.y, 10);
    ofPopStyle();
}

void Dreieck::setWorld(b2World* w){
    world=w;
}


void Dreieck::turnPhysicsOn(bool _on){
    bPhysics=_on;
    if(bPhysics){
        triangleAnchor.setPosition(getPosition());
        triangleAnchor.setPhysics(50, 0.5, 0.9);
        triangleAnchor.body->SetType(b2_dynamicBody);
    }
}

bool Dreieck::getIsPhysicsOn(){
    return bPhysics;
}

void Dreieck::setState(int _state){
    stateBefore=state;
    state=_state;
    switch (state) {
        case START:
            break;
        case MOVINGOBJECT:
            triangleAnchor.setPhysics(0, 0.5, 0.9);
            triangleAnchor.body->SetType(b2_kinematicBody);
            break;
        case PHYSICS:
            cout<<"State "<<state<<endl;
            triangleAnchor.setPosition(getPosition());
            triangleAnchor.setPhysics(50, 0.5, 0.9);
            triangleAnchor.body->SetType(b2_dynamicBody);
            break;
        case FADEOUT:
            fadeInitTime=ofGetElapsedTimef();
            fadeTarget=0;
            break;
            
        default:
            break;
    }
}

int Dreieck::getState(){
    return state;
}
