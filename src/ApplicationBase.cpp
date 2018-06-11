//
//  TestClassController.cpp
//  ColorWindows
//
//  Created by Michael Flueckiger on 06.09.12.
//  Copyright (c) 2012 Ich. All rights reserved.
//

#include <iostream>
#include "ApplicationBase.h"
#include "ApplicationController.h"


ApplicationBase::ApplicationBase(){
    init();
}


ApplicationBase::~ApplicationBase(){
    
}


//------------------------------------------------------


void ApplicationBase::init(){
    cout<<"init ApplicationBase"<<endl;
    bAddedListeners = false;
}


void ApplicationBase::update(){
    cout<<"ApplicationBase is updating"<<endl;
}

void ApplicationBase::draw(){
    cout<<"ApplicationBase is Drawing"<<endl;
}


void ApplicationBase::exit(){
    cout<<"exit TestClass"<<endl;
}



void ApplicationBase::setId(string _id){
    myId=_id;
}

string ApplicationBase::getId(){
    return myId;
}


//EVENTS
//------------------------------------------------------

void ApplicationBase::addListeners() {
    if(!bAddedListeners){
	ofAddListener(ofEvents().update, this, &ApplicationBase::_update);
    ofAddListener(ofEvents().draw, this, &ApplicationBase::_draw);
    ofAddListener(ofEvents().exit, this, &ApplicationBase::_exit);
    ofAddListener(ofEvents().keyPressed, this, &ApplicationBase::keyPressed);

    }
    bAddedListeners = true;
}


void ApplicationBase::removeListeners() {
	ofRemoveListener(ofEvents().update, this, &ApplicationBase::_update);
    ofRemoveListener(ofEvents().draw, this, &ApplicationBase::_draw);
    ofRemoveListener(ofEvents().exit, this, &ApplicationBase::_exit);
    ofRemoveListener(ofEvents().keyPressed, this, &ApplicationBase::keyPressed);

    bAddedListeners = false;
}


//EVENT HANDLER
//------------------------------------------------------

void ApplicationBase :: _update( ofEventArgs &e )
{
	update();
}

void ApplicationBase :: _draw( ofEventArgs &e )
{
	draw();
}


void ApplicationBase :: _exit( ofEventArgs &e )
{
	exit();
}


//Event Helper
//------------------------------------------------------

void ApplicationBase::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
}


void ApplicationBase::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
}


bool ApplicationBase::isRunning(){
    return bAddedListeners;
}


void ApplicationBase::reset(){
    //reset App
}



//KEY LISTENER
//--------------------------------------------------------------
void ApplicationBase::keyPressed(ofKeyEventArgs &e){
    int key=e.key;
}
