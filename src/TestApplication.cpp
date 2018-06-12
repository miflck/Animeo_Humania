//
//  TestApplication.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "TestApplication.hpp"
TestApplication::TestApplication(){
    init();
}


TestApplication::~TestApplication(){
    
}

void TestApplication::init(){
    cout<<"init TestApplication"<<endl;
    bAddedListeners = false;
    mover.setup();
    mover.setTarget(ofVec2f(ofGetWidth(),ofGetHeight()));
}

void TestApplication::update(){
    cout<<"update TestApplication"<<endl;
    mover.update();
}


void TestApplication::draw(){
    ofDrawRectangle(0,0,10,10);
    mover.draw();
}

void TestApplication::exit(){
    cout<<"exit TestApplication"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void TestApplication::keyPressed(ofKeyEventArgs &e){
   
}



void TestApplication::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void TestApplication::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}


//--------------------------------------------------------------
void TestApplication::mouseMoved(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mousePressed(ofMouseEventArgs &a){
    mover.setTarget(ofVec2f(a.x,a.y));
}

//--------------------------------------------------------------
void TestApplication::mouseReleased(ofMouseEventArgs &a){
    
}

void TestApplication::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mouseExited(ofMouseEventArgs &a){
    
}
