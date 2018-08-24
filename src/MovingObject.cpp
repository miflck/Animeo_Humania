//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "MovingObject.hpp"
#include "ofxEasing.h"


MovingObject:: MovingObject(){
    
}
MovingObject:: ~ MovingObject(){
}

void MovingObject::setup(){
    scaleDuration=1.f;
    actualRadius=0;
    easingInitTime = ofGetElapsedTimef();
    radiusTarget=50;
   // makeNewWanderTarget();
    
     wanderR=300.f;
     wanderD=400.0f;
     change=0.1f;
    
}

void MovingObject::update(){
    auto endTime = easingInitTime + scaleDuration;
    auto now = ofGetElapsedTimef();
    move();
    actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
    velocityBefore=velocity;
    
    // check if reached
    ofVec2f distance=target-position;
    if(distance.length()<1 && velocity.length()< 0.1)bHasReached=true;
    cout<<distance.length()<<" vel "<<velocity.length()<<" reached "<<bHasReached<<endl;
    }



void MovingObject::scaleTo(float _value){
    easingInitTime = ofGetElapsedTimef();
    radiusTarget=_value;
}

void MovingObject::scaleTo(float _value, float _duration){
    easingInitTime = ofGetElapsedTimef();
    radiusTarget=_value;
    scaleDuration=_duration;
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
    
   
    
    if(bSeekTarget) applyForce(seek(target,seekforce));
    if(bWander) applyForce(wander(wanderforce));
    //applyForce(arrive(target),1);
   // arrive(target);
    
    velocity+=acceleration;
    
    velocity.limit(maxspeed);
   // if(bSlowDown)velocity.limit(getArriveSpeed(target));

    
    position+= velocity;
    acceleration.set(0,0);
    velocity*=0.99;
    

    

}

void MovingObject::makeNewWanderTarget(){
    float wanderR=30.f;
    float wanderD=50.0f;
    float change=0.25f;
    
    
    ofVec3f circlepos=velocity;
    circlepos.normalize();
    circlepos*=wanderD;
    circlepos+=position;
    
    ofVec3f rad=velocity;
    rad.normalize();
    rad*=wanderR;
    wandertheta+=ofRandom(-change,change);
    rad.rotate(wandertheta, ofVec3f(0,0,1));
    target=position+rad;
    
    
    
    
    
}



void MovingObject::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    //ofSetColor(255);
    ofDrawEllipse(position.x, position.y, actualRadius, actualRadius);
    ofPopStyle();
    ofPopMatrix();
    
    
    
}


void MovingObject::setPosition(int _x, int _y){
    position.set(_x, _y);
}

void MovingObject::setPosition(ofVec2f _p){
    position.set(_p);
}


void MovingObject::setRadius(int _radius){
    radiusTarget=_radius;
}

int MovingObject::getRadius(){
    return actualRadius;

}

void MovingObject::setSpeed(int _x, int _y){
    velocity.set(_x,_y);
}


void MovingObject::setSpeed(ofVec2f _s){
    velocity.set(_s);
}

ofVec2f MovingObject::getSpeed(){
    return velocity;
}

ofVec2f MovingObject::getSpeedBefore(){
    return velocityBefore;
}


void MovingObject::setTarget(ofVec2f _target){
    oldtarget.set(target);
    target.set(_target);
    bHasReached=false;
    
    // make slower moves if target move slow
  /*  if(bMovingMaxspeed){
        ofVec2f p(position);
        ofVec2f desired=target-p;
        float d = ABS(desired.length());
        if(d<200){
            maxspeed=ofMap(d,0,100,0,initmaxspeed);
            cout<<d<<" "<<maxspeed<<endl;
        }else{
            maxspeed=initmaxspeed;
        }
    }else{
        maxspeed=initmaxspeed;
    }*/
}


ofVec2f MovingObject::getTarget(){
    return target;
}

bool MovingObject::getReached(){
    return bHasReached;
}

void MovingObject::setReached(bool r){
    bHasReached=r;
}

void MovingObject::applyForce(ofVec2f _force){
    acceleration+= _force;
}


void MovingObject::applyForce(ofVec2f _force, float _strength){
    _force.limit(_strength);
    acceleration+= _force;
}


ofVec2f MovingObject::getPosition(){
    return position;
}

void MovingObject::setSeekForce(float _f){
    seekforce=_f;
}


ofVec2f MovingObject::getDistance(ofVec2f _t){
    ofVec2f p(position);
    ofVec2f desired=_t-p;
    return desired;
}

void MovingObject::setSlowDown(bool _b){
    bSlowDown=_b;
}

void MovingObject::setSlowDownDistance(int _d){
    slowdowndistance=_d;
}


bool MovingObject::shouldRemoveOffScreen(shared_ptr<MovingObject> shape) {
    return !ofRectangle(0, 0, ofGetWidth(), ofGetHeight()).inside(shape.get()->getPosition());
}



bool MovingObject::isOnScreen(){
    bool isInside=false;
    ofRectangle r;
    r.set(0,0,ofGetWidth(),ofGetHeight());
    if(r.inside(position))isInside=true;
    return isInside;
}

void MovingObject::setMaxSpeed(float _m){
    maxspeed=_m;
}


ofVec2f MovingObject::seek(ofVec2f t, float f){
    ofVec2f p(position);
    ofVec2f desired=t-p;
    float d = desired.length();
    desired.normalize();
  
    if(d<slowdowndistance && bSlowDown){
       // desired_velocity = normalize(desired_velocity) * max_velocity * (distance / slowingRadius)
        float m=ofMap(d,0,slowdowndistance,0,maxspeed);
         //float m=ofMap(d,0,slowdowndistance,0,-velocity.length());

        //float m=ofMap(d,0,slowdowndistance,0,-velocity);
    //    desired*=maxspeed*(d/slowdowndistance);
        desired*=m;
       //cout<<d<<" "<<desired.length()<<endl;
       // desired*=maxspeed;

    }else{
        desired*=maxspeed;
    }
    
    ofVec2f steer=desired-velocity;
    steer.limit(f);
    return steer;
}


ofVec2f MovingObject::arrive(ofVec2f t){
    ofVec2f p(position);
    ofVec2f desired=t-p;
    float d = desired.length();
    desired.normalize();
    /*if(d<slowdowndistance && bSlowDown){
        // desired_velocity = normalize(desired_velocity) * max_velocity * (distance / slowingRadius)
        
        //float m=ofMap(d,0,slowdowndistance,0,maxspeed);
        desired*=maxspeed*(d/slowdowndistance);
    }*/
    ofVec2f steer=desired-velocity;
    steer.limit(1);
    return steer;
}



float MovingObject::getArriveSpeed(ofVec2f t){
    ofVec2f p(position);
    ofVec2f desired=t-p;
    float ms=maxspeed;
    float d = desired.length();
    desired.normalize();
    if(d<slowdowndistance){
        // desired_velocity = normalize(desired_velocity) * max_velocity * (distance / slowingRadius)
        
        float m=ofMap(d,0,slowdowndistance,0,maxspeed);
        ms=m;
    }
    return ms;
}


ofVec3f MovingObject::wander(float f){
    /*   float wanderR=50;
     float wanderD=500.0f;
     float change=0.3f;
     */
    
    ofVec3f circlepos=velocity;
    circlepos.normalize();
    circlepos*=wanderD;
    //circlepos+=position;
    
    ofVec3f rad=velocity;
    rad.normalize();
    rad*=wanderR;
    wandertheta+=ofRandom(-change,change);
    rad.rotateRad(wandertheta, ofVec3f(0,0,1));
    ofVec3f wandertarget=position+rad+circlepos;
    
    
    ofVec3f p(position);
    ofVec3f desired=wandertarget-p;
    desired.normalize();
    desired*=maxspeed;
    ofVec3f steer=desired-velocity;
    steer.limit(f);
    
    //cout<<circlepos<<" "<<steer<<endl;

    return steer;

}




