//
//  ApplicationBase.h
//
//  Created by Michael Flueckiger on 06.09.12.
//  Copyright (c) 2012 Ich. All rights reserved.
//

#ifndef ApplicationBase_h
#define ApplicationBase_h

#include "ofMain.h"
//#include "ApplicationController.h"


class ApplicationBase {
public:
    ApplicationBase();
    virtual ~ApplicationBase();
    
    
    //events
    virtual void addListeners		();
	virtual void removeListeners	();
    
    virtual void _update(ofEventArgs &e);
    virtual void _draw(ofEventArgs &e);
    virtual void _exit(ofEventArgs &e);
    virtual void keyPressed(ofKeyEventArgs &e);
  //  virtual void keyReleased(ofKeyEventArgs &e);

    
    virtual void mouseMoved(ofMouseEventArgs &a );
    virtual void mouseDragged(ofMouseEventArgs &a);
    virtual void mousePressed(ofMouseEventArgs &a);
    virtual void mouseReleased(ofMouseEventArgs &a);
    virtual void mouseScrolled(ofMouseEventArgs &a);
    virtual void mouseEntered(ofMouseEventArgs &a);
    virtual void mouseExited(ofMouseEventArgs &a);

    
    virtual bool isRunning();
    virtual void turnOn();
    virtual void turnOff();
    virtual void reset();
    virtual string getId();
    virtual void setId(string _id);

    
    
    //structure
   virtual void init();
   virtual void update();
   virtual void draw();
   virtual void exit();
    
    
    
    bool bAddedListeners;
    bool bIsRunning;
    
    

    
 private: 
    //events
     string myId;


    
    
};

#endif
