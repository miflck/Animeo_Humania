//
//  Line.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
class Line:public ApplicationBase{
public:
    Line();
    virtual ~Line();
    
    
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


#endif /* Line_hpp */
