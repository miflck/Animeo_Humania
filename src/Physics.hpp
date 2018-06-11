//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef Physics_hpp
#define Physics_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
class Physics:public ApplicationBase{
public:
    Physics();
    virtual ~Physics();
    
    
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


#endif /* Physics_hpp */
