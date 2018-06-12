//
//  KinectV2.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "KinectV2.hpp"
#include "ApplicationController.h"

KinectV2::KinectV2(){
    init();
}


KinectV2::~KinectV2(){
    
}

void KinectV2::init(){
    cout<<"init KinectV2"<<endl;
    bAddedListeners = false;
    
  
    
}

void KinectV2::update(){
 
}


void KinectV2::draw(){
 
    
}

void KinectV2::exit(){
}



//KEY LISTENER
//--------------------------------------------------------------
void KinectV2::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'c') {
     
    }
}


//--------------------------------------------------------------
void KinectV2::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void KinectV2::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2::mousePressed(ofMouseEventArgs &a){
  

}

//--------------------------------------------------------------
void KinectV2::mouseReleased(ofMouseEventArgs &a){
    
}

void KinectV2::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2::mouseExited(ofMouseEventArgs &a){
    
}


void KinectV2::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void KinectV2::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void KinectV2::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
