//
//  ApplicationController.cpp
//  ColorWindows
//
//  Created by Michael Flueckiger on 06.09.12.
//  Copyright (c) 2012 Ich. All rights reserved.
//

#include <iostream>
#include "ApplicationController.h"
#include "LightPointApp.hpp"
#include "Line.hpp"
#include "Physics.hpp"
#include "PhysicsWorld.hpp"
#include "KinectV2.hpp"
#include "KinectV2Manager.hpp"
#include "Arm.hpp"
#include "EmotionWorld.hpp"
#include "Face.hpp"
#include "LinieApp.hpp"

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
    
    if(bUseOSC)oscmanager.setup();


	initialized=true;
    cout<<"init ApplicationController"<<endl;
   // ofSetLogLevel(OF_LOG_SILENT);



    
    mainOutputSyphonServer.setName("Screen Output");
    individualTextureSyphonServer.setName("Texture Output");
    
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
    
    applications["lightpoint"]=new LightPointApp();
    applications["lightpoint"]->setId("lightpoint");
    
    applications["line"]=new Line();
    applications["line"]->setId("line");
    
    applications["physics"]=new Physics();
    applications["physics"]->setId("physics");
    
    applications["physicsworld"]=new PhysicsWorld();
    applications["physicsworld"]->setId("physicsworld");
    
    applications["arm"]=new Arm();
    applications["arm"]->setId("arm");
    
    applications["emotionworld"]=new EmotionWorld();
    applications["emotionworld"]->setId("emotionworld");
    
    applications["face"]=new Face();
    applications["face"]->setId("face");
    
    applications["linieApp"]=new LinieApp();
    applications["linieApp"]->setId("linieApp");
    
    //applications["kinectV2"]=new KinectV2();
    //applications["kinectV2"]->setId("kinectV2");
	
        
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
    
    
    if(bUseOSC)oscmanager.update();

}

void ApplicationController::_draw(ofEventArgs &e) {
    KINECTMANAGER->draw();

    if(activeApplication!=NULL){

    activeApplication->draw();

    }
   // mainOutputSyphonServer.publishScreen();

    }



ApplicationBase* ApplicationController::getAppByName(string _identifier){
    map<string,ApplicationBase*>::iterator a;
    a=applications.find(_identifier);
    return (*a).second;
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

void ApplicationController::setAppNull(){
    if(activeApplication!=NULL){
        activeApplication->turnOff();
    }
    
    activeApplication=NULL;
}


void ApplicationController::sayHello(){
    cout<<"hello"<<endl;
}



//--------------------------------------------------------------
void ApplicationController::gotMessage(ofMessage &msg){
    
    cout<<"gotmessage:"<<msg.message<<endl;
   
}


void ApplicationController::toggleDebug(){
    debug=!debug;
}

