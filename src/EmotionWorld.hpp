//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef EmotionWorld_hpp
#define EmotionWorld_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "ofxBox2d.h"
#include "MovingObject.hpp"
#include "Sun.hpp";



class EmotionWorld:public ApplicationBase{
public:
   EmotionWorld();
    virtual ~EmotionWorld();
    
    

    
    void turnOn();
    void turnOff();
    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();
    void keyPressed(ofKeyEventArgs &e);
    
    void mouseMoved(ofMouseEventArgs &a );

     void mouseDragged(ofMouseEventArgs &a);
     void mousePressed(ofMouseEventArgs &a);
     void mouseReleased(ofMouseEventArgs &a);
     void mouseScrolled(ofMouseEventArgs &a);
     void mouseEntered(ofMouseEventArgs &a);
     void mouseExited(ofMouseEventArgs &a);

    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    
    void toggleMouseActive();
    ofFbo screen;
    
    
    Sun sun;
    
    
    
    void toggleHearts();
    void toggleSun();

    
private:
    ofxBox2d                                  box2d;   // the box2d world
    ofxBox2dCircle                            anchor;  // fixed anchor
    vector      <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles
    vector      <shared_ptr<ofxBox2dJoint> >  joints;  // joints
    vector    <shared_ptr<ofxBox2dRect> >   boxes;           // default box2d rects
    ofxBox2dRect box;
    ofxBox2dRect leftbox;
    ofxBox2dRect rightbox;

    ofxBox2dCircle                            anchor2;  // fixed anchor
    vector      <ofVec2f>   positions;  // joints

    ofPolyline line;
    
    ofxBox2dJoint mousejoint;
    bool bIsMouseActive=false;
  
   vector<MovingObject> movingObjects;
    //vector<shared_ptr<Agent> > movingObjects;

    ofImage herz;
    
    bool bEmitHearts=false;
    bool bShowSun=false;
    
};


#endif /* Physics_hpp */
