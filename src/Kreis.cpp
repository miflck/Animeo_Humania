//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Kreis.hpp"
#include "ofxEasing.h"

Kreis::Kreis(){
    
}

Kreis::~Kreis(){
    
}

void Kreis::setup(){
    radiusTarget=100;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.setup(world, 0, 0, radiusTarget);
    actualRadius=0;
    easingInitTime = ofGetElapsedTimef();
    actualRadius=20;
}

void Kreis::update(){
    
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    if(bPhysics) {
        setPosition(anchor.getPosition().x, anchor.getPosition().y);
    }else{
        move();
        anchor.setPosition(getPosition());
    }

}

void Kreis::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition().x, getPosition().y);
    ofSetColor(255);
    ofDrawCircle(0,0,actualRadius);
    ofPopStyle();
    ofPopMatrix();
}

void Kreis::setWorld(b2World* w){
    world=w;
}


void Kreis::turnPhysicsOn(bool _on){
    bPhysics=_on;
    if(bPhysics){
        anchor.setPosition(getPosition());
        anchor.setPhysics(50, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
    }
}

bool Kreis::getIsPhysicsOn(){
    return bPhysics;
}

