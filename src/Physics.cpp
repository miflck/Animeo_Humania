//
//  Physics.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "Physics.hpp"
#include "ApplicationController.h"

Physics::Physics(){
    init();
}


Physics::~Physics(){
    
}

void Physics::init(){
    cout<<"init Physics"<<endl;
    bAddedListeners = false;
    
}

void Physics::update(){

}


void Physics::draw(){
}

void Physics::exit(){
    cout<<"exit Physics"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void Physics::keyPressed(ofKeyEventArgs &e){
   
}



void Physics::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void Physics::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
