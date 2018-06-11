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
    
}

void TestApplication::update(){
    cout<<"update TestApplication"<<endl;

}


void TestApplication::draw(){
    ofDrawRectangle(0,0,10,10);    
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
