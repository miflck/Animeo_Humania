//
//  PhysicsWorld.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "PhysicsWorld.hpp"
#include "ApplicationController.h"

PhysicsWorld::PhysicsWorld(){
    init();
}


PhysicsWorld::~PhysicsWorld(){
    
}

void PhysicsWorld::init(){
    cout<<"init PhysicsWorld"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 3);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    float xPos=(ofGetWidth()/3)*2;
    anchor.setup(box2d.getWorld(), xPos, 0, 10);
    anchor2.setup(box2d.getWorld(), xPos, ofGetHeight(), 10);
    
    
    int num=30;
    int jointlength=(ofGetHeight()/(num+1));
    
    // first we add just a few circles
    for(int i=0; i<num; i++) {
        auto circle = std::make_shared<ofxBox2dCircle>();
        
        // if(i%2==0){
        //   circle.get()->setPhysicsWorld(10.0, 0.53, 0.9);
        
        // }else{
        circle.get()->setPhysics(3, 0.53, 5);
        
        // }
        circle.get()->setup(box2d.getWorld(), xPos, (ofGetHeight()/(num+1))+(ofGetHeight()/(num+1))*i, 5);
        circles.push_back(circle);
        positions.push_back(ofVec2f(xPos, (ofGetHeight()/(num+1))+(ofGetHeight()/(num+1))*i));
    }
    
    // now connect each circle with a joint
    for(int i=0; i<circles.size(); i++) {
        auto joint = std::make_shared<ofxBox2dJoint>();
        // if this is the first point connect to the top anchor.
        if(i == 0) {
            joint.get()->setup(box2d.getWorld(), anchor.body, circles[i].get()->body,20,1);
        }
        else {
            joint.get()->setup(box2d.getWorld(), circles[i-1].get()->body, circles[i].get()->body,10,10);
        }
        joint.get()->setLength(jointlength);
        joints.push_back(joint);
    }
    
    auto joint = std::make_shared<ofxBox2dJoint>();
    joint.get()->setup(box2d.getWorld(), circles[circles.size()-1].get()->body, anchor2.body,20,0.1);
    joint.get()->setLength(jointlength);
    
    joints.push_back(joint);
    
    
}

void PhysicsWorld::update(){
    box2d.update();
    
    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
    float minDis = ofGetMousePressed() ? 100 : 50;
    
    for(int i=0; i<circles.size(); i++) {
        float dis = mouse.distance(circles[i].get()->getPosition());
     //   if(bIsMouseActive)circles[i].get()->addRepulsionForce(mouse, 9);
       if(dis < minDis && bIsMouseActive) circles[i].get()->addRepulsionForce(mouse,10);
       //  else circles[i].get()->addAttractionPoint(mouse, 4.0);
       // circles[i].get()->setDamping(0.98);
        
    }
    
    anchor.addAttractionPoint(mouse,40);

    
    anchor2.setDamping(0.9);

    //ofVec2f distance=mouse-anchor.getPosition();
  //  anchor2.setVelocity(distance.normalize()*-1);
    ofPolyline cur;
    
    cur.addVertex(anchor.getPosition());
    
    for(int i=0; i<circles.size(); i++) {
        cur.addVertex( circles[i].get()->getPosition());
    }
    
    cur.addVertex(anchor2.getPosition());
    
    line=cur;
}


void PhysicsWorld::draw(){
    ofSetColor(0);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    
    ofSetHexColor(0xf2ab01);


    if(APPC->debug){
        
        anchor.draw();
        anchor2.draw();
        
       ofDrawRectangle(anchor.getPosition().x,anchor.getPosition().y,10,10);
        
            for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
             circles[i].get()->draw();
        }
        
        for(int i=0; i<joints.size(); i++) {
            ofSetHexColor(0x444342);
              joints[i].get()->draw();
        }
    }
    
    line=line.getResampledBySpacing(2);
   // line=line.getSmoothed(5);
    
    ofSetColor(255);
    ofSetLineWidth(3);
    line.draw();
    
    string info = "";
    info += "Press [n] to add a new joint\n";
    info += "click and pull the chain around\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
}

void PhysicsWorld::exit(){
    cout<<"exit PhysicsWorld"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void PhysicsWorld::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'c') {
        cout<<"Add Circles"<<endl;
        float r = ofRandom(4, 20);        // a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        
    }
}


//--------------------------------------------------------------
void PhysicsWorld::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void PhysicsWorld::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void PhysicsWorld::mousePressed(ofMouseEventArgs &a){
    cout<<"Mouse"<< anchor.isFixed()<<endl;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.body->SetType(b2_staticBody);
    //anchor.setup(box2d.getWorld(), a.x, 0, 10);
    

}

//--------------------------------------------------------------
void PhysicsWorld::mouseReleased(ofMouseEventArgs &a){
    
}

void PhysicsWorld::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void PhysicsWorld::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void PhysicsWorld::mouseExited(ofMouseEventArgs &a){
    
}


void PhysicsWorld::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void PhysicsWorld::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void PhysicsWorld::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
