//
//  ApplicationController.cpp
//  ColorWindows
//
//  Created by Michael Flueckiger on 06.09.12.
//  Copyright (c) 2012 Ich. All rights reserved.
//

#include <iostream>
#include "ApplicationController.h"
#include "TestApplication.hpp"
#include "Line.hpp"
#include "Physics.hpp"




ApplicationController* ApplicationController::instance = 0;

ApplicationController* ApplicationController::getInstance() {
	if (!instance) {
		instance = new ApplicationController();
	}
	return instance;
}

ApplicationController::ApplicationController() {
	
}

void ApplicationController::initialize() {
 
	initialized=true;
    cout<<"init ApplicationController"<<endl;
    ofSetLogLevel(OF_LOG_SILENT);



    
    
    
  //  ofRegisterGetMessages(this);

	/*ofAddListener(inter.inPose,this,&ApplicationController::inPose);
	ofAddListener(inter.inGesture,this,&ApplicationController::inGesture);
	ofAddListener(inter.userIn,this,&ApplicationController::userIn);
	ofAddListener(inter.userMove,this,&ApplicationController::userMove);
	ofAddListener(inter.userOut,this,&ApplicationController::userOut);
    ofAddListener(inter.gestureInit,this,&ApplicationController::gestureInit);
	ofAddListener(inter.notGestureInit,this,&ApplicationController::notGestureInit);
    
    ofAddListener(inter.completedMove,this,&ApplicationController::completedMove);*/
    
  
    activeApplication=NULL;
    
    applications["testApp"]=new TestApplication();
    applications["testApp"]->setId("testApp");
    
    applications["line"]=new Line();
    applications["line"]->setId("line");
    
    applications["physics"]=new Physics();
    applications["physics"]->setId("physics");
    
	
        
	/*applications["testClass"]=new TestClassController();
    applications["testClass"]->setId("testClass");
	applications["screenSaver"]=new ScreenSaverController();
    applications["screenSaver"]->setId("screenSaver");
    */
    

    
    //setAppById("screenSaver");

    ofAddListener(ofEvents().update, this, &ApplicationController::_update);
    ofAddListener(ofEvents().draw, this, &ApplicationController::_draw);

}

bool ApplicationController::isInitialized(){
	return initialized;
}

void ApplicationController::_update(ofEventArgs &e) {
    

    
    string id;
    if(activeApplication!=NULL){
    id=activeApplication->getId();
    }
    
    
  
}

void ApplicationController::_draw(ofEventArgs &e) {
   
   
    }



void ApplicationController::setAppById(string _identifier){
	
	
	if(activeApplication!=NULL){
		activeApplication->turnOff();
	}
	
        
    app=applications.find(_identifier);
    activeApplication=(*app).second;
	
    
	if(activeApplication!=NULL && !activeApplication->isRunning()){
        activeApplication->turnOn();
	}
	
	
}




void ApplicationController::sayHello(){
    cout<<"hello"<<endl;
}



//--------------------------------------------------------------
void ApplicationController::gotMessage(ofMessage &msg){
    
    cout<<"gotmessage:"<<msg.message<<endl;
   
}



