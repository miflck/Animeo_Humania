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
#include "MovingObject.hpp"
class TestApplication:public ApplicationBase{
public:
    TestApplication();
    virtual ~TestApplication();
    
    
    void keyPressed(ofKeyEventArgs &e);
    void mouseMoved(ofMouseEventArgs &a );
    void mouseDragged(ofMouseEventArgs &a);
    void mousePressed(ofMouseEventArgs &a);
    void mouseReleased(ofMouseEventArgs &a);
    void mouseScrolled(ofMouseEventArgs &a);
    void mouseEntered(ofMouseEventArgs &a);
    void mouseExited(ofMouseEventArgs &a);

    
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
    
    bool bSendOSCPosition=false;
    int m8layer=1;
    
    MovingObject mover;
    
    int skelettId=0;
    
};


#endif /* TestApplication_hpp */
