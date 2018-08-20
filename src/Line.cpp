//
//  Line.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "Line.hpp"
#include "ApplicationController.h"

Line::Line(){
    init();
}


Line::~Line(){
    
}

void Line::init(){
    cout<<"init Line"<<endl;
    bAddedListeners = false;
    
}

void Line::update(){
    cout<<"update Line"<<endl;

}


void Line::draw(){
    ofDrawEllipse(0,0,10,10);
    ///APPC->sayHello();
}

void Line::exit(){
    cout<<"exit Line"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void Line::keyPressed(ofKeyEventArgs &e){
   
}



void Line::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void Line::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
