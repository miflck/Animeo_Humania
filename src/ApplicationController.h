//
//  ApplicationController.h
//  ColorWindows
//
//  Created by Michael Flueckiger on 06.09.12.
//  Copyright (c) 2012 Ich. All rights reserved.
//
#pragma once

#ifndef ApplicationController_h
#define ApplicationController_h


#include "ofMain.h"


#include "ApplicationBase.h"

#include "OscManager.hpp"
#include "GuiApp.h"




//#include "ofxXmlSettings.h"

//#include "TextureManager.h"
//#include "SettingsManager.h"

//#include "SoundinputController.h"
//#include "BlobTrackingController.h"

//#include "Constants.h";
//#include "Observable.h"

//for convenience
#define APPC ApplicationController::getInstance()
/*
struct lineStruct {  
    vector<ofVec2f> myline;
};
*/

class ApplicationController {
	
public:
	
	static ApplicationController* getInstance();
	void initialize();
	bool isInitialized();
    void sayHello();
    void setAppById(string _identifier);
    
    void setAppNull();

    
    void gotMessage(ofMessage &msg);
    
    void toggleDebug();

     
    bool debug=true;
    

    ApplicationBase* getAppByName(string _identifier);

	    OscManager oscmanager;
    bool bUseOSC=true;
    
    
    shared_ptr<GuiApp> gui;

    
private:
	ApplicationController();
	static ApplicationController* instance;
	bool initialized;
	void _update(ofEventArgs &e);
	void _draw(ofEventArgs &e);
    ApplicationBase* activeApplication;
	map<string, ApplicationBase*>applications;
	map<string,ApplicationBase*>::iterator app;
    
    
    
};

#endif
