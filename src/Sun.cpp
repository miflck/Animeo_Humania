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
    cout<<"sun update"<<endl;
    setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
}





void Sun::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition().x, getPosition().y);
    ofDrawCircle(0,0,50);
    ofPopStyle();
    ofPopMatrix();
}


