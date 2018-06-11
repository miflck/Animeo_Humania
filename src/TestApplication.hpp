//
//  TestApplication.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef TestApplication_hpp
#define TestApplication_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
class TestApplication:public ApplicationBase{
public:
    TestApplication();
    virtual ~TestApplication();
    
    
    void keyPressed(ofKeyEventArgs &e);

    
    void turnOn();
    void turnOff();
    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();

    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    
    
};


#endif /* TestApplication_hpp */
