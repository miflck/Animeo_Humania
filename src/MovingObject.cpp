//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "MovingObject.hpp"
void MovingObject::setup(){
    radius=50;
    
    
}

void MovingObject::update(){
    move();
    //position+=speed;
    
    
}

void MovingObject::move(){
   /* ofVec2f d;
    d.set(target);
    d-=position;
    if(d.length()>0.5){
    d.normalize();
    d*=maxspeed;
    speed.set(d);
    }else{
        speed.set(0,0);
    }

    */
    
    applyForce(seek(target,0.5));

    velocity+=acceleration;
    velocity*=0.98;
    position+= velocity;
    acceleration.set(0,0);
    
    
}

void MovingObject::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofDrawEllipse(position.x, position.y, radius, radius);
    ofPopStyle();
    ofPopMatrix();
    
}


void MovingObject::setPosition(int _x, int _y){
    position.set(_x, _y);


}


void MovingObject::setRadius(int _radius){
    radius=_radius;
}

int MovingObject::getRadius(){
    return radius;

}

void MovingObject::setSpeed(int _x, int _y){
    velocity.set(_x,_y);
}


void MovingObject::setTarget(ofVec2f _target){
    target.set(_target);
}

void MovingObject::applyForce(ofVec2f _force){
    acceleration+= _force;
}

ofVec2f MovingObject::seek(ofVec2f t, float f){
    ofVec2f p(position);
    ofVec2f desired=t-p;
    float d = desired.length();
    desired.normalize();
    if(d<300){
        float m=ofMap(d,0,100,0,maxspeed);
        desired*=m;
    }else{
        desired*=maxspeed;
    }
    ofVec2f steer=desired-velocity;
    steer.limit(f);
    return steer;
}
