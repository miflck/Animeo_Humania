//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Sun.hpp"

Sun::Sun(){
    
}

Sun::~Sun(){
    
}

void Sun::setup(){
    
}

void Sun::update(){
    move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
}





void Sun::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255,255,0);
    ofSetLineWidth(8);
    ofTranslate(getPosition().x, getPosition().y);
    ofPushMatrix();
    ofVec2f ray=ofVec2f(2000,0);
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


