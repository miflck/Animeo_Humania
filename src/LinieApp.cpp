//
//  LinieApp.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "LinieApp.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


LinieApp::LinieApp(){
    init();
}


LinieApp::~LinieApp(){
    
}

void LinieApp::init(){
    cout<<"init LinieApp"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.createGround();

    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    float xPos=(ofGetWidth()-250);

    anchorPositionTop.set(xPos, ofGetHeight()/3);
    anchorPositionBottom.set(xPos,ofGetHeight());
    
    
    //anchor.setPhysics(50, 0.5, 0.9);

    anchor.setup(box2d.getWorld(), anchorPositionTop.x, anchorPositionTop.y, 10);
    anchor2.setup(box2d.getWorld(), anchorPositionBottom.x, anchorPositionBottom.y, 10);
    
    
    int num=10;
    int jointlength=25;
    
    // first we add just a few circles
    for(int i=0; i<num; i++) {
        
        auto circle = std::make_shared<ofxBox2dCircle>();
        
        // if(i%2==0){
        //   circle.get()->setPhysics(10.0, 0.53, 0.9);
        
        // }else{
        circle.get()->setPhysics(5, 0.53, 0.9);
        
        // }
        circle.get()->setup(box2d.getWorld(), xPos, ((anchorPositionBottom.x-anchorPositionTop.x)/(num+1))+((anchorPositionBottom.x-anchorPositionTop.x)/(num+1))*i, 5);
        circles.push_back(circle);
        positions.push_back(ofVec2f(xPos, (ofGetHeight()/(num+1))+(ofGetHeight()/(num+1))*i));
    }
    
    // now connect each circle with a joint
    for(int i=0; i<circles.size(); i++) {
        auto joint = std::make_shared<ofxBox2dJoint>();
        // if this is the first point connect to the top anchor.
        if(i == 0) {
            joint.get()->setup(box2d.getWorld(), anchor.body, circles[i].get()->body,5,1);
        }
        else {
            joint.get()->setup(box2d.getWorld(), circles[i-1].get()->body, circles[i].get()->body,5,1);
        }
       joint.get()->setLength(jointlength);
        joints.push_back(joint);
    }
    
    auto joint = std::make_shared<ofxBox2dJoint>();
    joint.get()->setup(box2d.getWorld(), circles[circles.size()-1].get()->body, anchor2.body,5,10);
    joint.get()->setLength(jointlength);
    
    joints.push_back(joint);
    
    
}

void LinieApp::update(){
    box2d.update();
    
    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
    float minDis = ofGetMousePressed() ? 200 : 100;
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    ofVec2f leftHand;
    if(mskel.size()>0){
         leftHand=mskel[0].leftHand;
    }
    
    for(int i=0; i<circles.size(); i++) {
       /* float dis = mouse.distance(circles[i].get()->getPosition());
        if(dis < minDis && bIsMouseActive) circles[i].get()->addRepulsionForce(mouse,10);
            if(mskel.size()>0){
                float handDist = leftHand.distance(circles[i].get()->getPosition());
                if(dis < minDis) circles[i].get()->addRepulsionForce(leftHand,10);
            }*/
       circles[i].get()->setDamping(0.98);
    }
    
    anchor.setDamping(0.98);
    anchor2.setDamping(0.98);

    
    if( bUseHand){
        if(mskel.size()>0 ){
            for(int i=0; i<circles.size(); i++) {
                float handDist = leftHand.distance(circles[i].get()->getPosition());
                if(repulse){
                    //anchor.addRepulsionForce(leftHand,10);
                    if(handDist < minDis) circles[i].get()->addRepulsionForce(leftHand,10);
                }else{
                   // anchor.addAttractionPoint(leftHand,1000);
                    if(handDist < minDis) circles[i].get()->addRepulsionForce(leftHand,-10);
                }
            }
        }
    }else{
        for(int i=0; i<circles.size(); i++) {
             float dis = mouse.distance(circles[i].get()->getPosition());

            if(repulse){
                if(dis < minDis) circles[i].get()->addRepulsionForce(mouse,30);

                //anchor.addRepulsionForce(mouse,50);
            }else{
                if(dis < minDis) circles[i].get()->addAttractionPoint(mouse,100);

                //anchor.addAttractionPoint(mouse,50);
            }
        }
    }
    
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
    line.getSmoothed(5);
}


void LinieApp::draw(){
    ofSetColor(0);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    
    
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(5);
    line.draw();
    
   // ofDrawCircle(anchor.getPosition().x,anchor.getPosition().y,50);
    ofPopStyle();
    
    
    ofSetHexColor(0xf2ab01);


    if(APPC->debug){
        ofPushStyle();
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

        ofVec2f leftHand;
        if(mskel.size()>0){
            leftHand=mskel[0].leftHand;
        }
        anchor.draw();
        anchor2.draw();
        ofDrawEllipse(leftHand,20,20);
        
            for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
             circles[i].get()->draw();
        }
        
        for(int i=0; i<joints.size(); i++) {
            ofSetHexColor(0x444342);
              joints[i].get()->draw();
        }
        ofPopStyle();

    }
    
   // line=line.getResampledBySpacing(20);
  // line=line.getSmoothed(10);
    


    
    
    string info = "";
    info += "Press [n] to add a new joint\n";
    info += "click and pull the chain around\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
}

void LinieApp::exit(){
    cout<<"exit LinieApp"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void LinieApp::keyPressed(ofKeyEventArgs &e){
   if(e.key=='R'){
        anchor.setPhysics(10, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
        
    }
    
    
    if(e.key=='r'){
        anchor2.setPhysics(10, 1, 10);
        anchor2.body->SetType(b2_dynamicBody);
        
    }
    
    if(e.key=='l'){
        bUseHand=!bUseHand;
        cout<<bUseHand<<endl;
    }
    
    if(e.key=='h'){
        anchor.setPosition(0, ofGetHeight()/2);
        anchor.setPhysics(0, 0.5, 0.9);
        anchor.body->SetType(b2_staticBody);

        anchor2.setPosition(ofGetWidth(), ofGetHeight()/2);
        anchor2.setPhysics(0, 0.5, 0.9);
        anchor2.body->SetType(b2_staticBody);
        
    }
    
    
    if(e.key=='a'){
        anchor2.setPosition(ofGetMouseX(), ofGetMouseY());
    }
    
    
    if(e.key=='s'){
        anchor.setPosition(ofGetMouseX(), ofGetMouseY());
    }
    
    if(e.key=='w'){
        repulse=!repulse;
    }
    
    
    if(e.key=='-'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(joints[i].get()->getFrequency()-0.1);
        }
        cout<<joints[0].get()->getFrequency()<<endl;
    }
    
    if(e.key=='+'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(joints[i].get()->getFrequency()+0.1);
        }
        cout<<joints[0].get()->getFrequency()<<endl;
    }
    
   /* if(e.key=='d'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping( joints[i].get()->getDamping()+0.01);
        }
        cout<<joints[0].get()->getDamping()<<endl;
    }
    if(e.key=='D'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping( joints[i].get()->getDamping()-0.01);
        }
        cout<<joints[0].get()->getDamping()<<endl;
    }
    */
    
    if(e.key=='m'){
        toggleMouseActive();
    }
    
    
}


//--------------------------------------------------------------
void LinieApp::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void LinieApp::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LinieApp::mousePressed(ofMouseEventArgs &a){
  //  cout<<"Mouse"<< anchor.isFixed()<<endl;
    //anchor.setPhysics(0, 0.5, 0.9);
    //anchor.body->SetType(b2_staticBody);
    //anchor.setup(box2d.getWorld(), a.x, 0, 10);
    

}

//--------------------------------------------------------------
void LinieApp::mouseReleased(ofMouseEventArgs &a){
    
}

void LinieApp::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LinieApp::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LinieApp::mouseExited(ofMouseEventArgs &a){
    
}


void LinieApp::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void LinieApp::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void LinieApp::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
