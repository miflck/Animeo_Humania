//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Stern_hpp
#define Stern_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"
#include "ofxBox2d.h"
#define START 1
#define MOVINGOBJECT 2
#define PHYSICS 3
#define FADEOUT 4


class Stern:public MovingObject {
    
public:
    Stern();
    virtual ~Stern();
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    b2World* world;
    void setWorld(b2World* w);
    ofxBox2dCircle anchor;

    bool bPhysics=false;
    void turnPhysicsOn(bool _on);
    bool getIsPhysicsOn();
    
    float actualRadius;
    float easingInitTime;
    float radiusTarget;
    float scaleDuration=8;
    
    bool bFadeOut=false;
    int state;
    int stateBefore;
    void setState(int _state);
    int getState();
    
    ofColor color;
    int fadeAlpha=255;
    float fadeInitTime;
    float fadeTarget;
    float fadeDuration;
    
    
    ofImage sternImg;
    
    void setTargetRadius(int _r);
    
    bool bShouldRemove=false;
    
    
    ofRectangle screen;

    static bool shouldRemoveFromScreen(shared_ptr<Stern> shape) {
     //   bool r=false;
       // if(shape->bShouldRemove)r=true;
       return !ofRectangle(-300, -300, shape.get()->screen.getWidth()+600, shape.get()->screen.getHeight()+600).inside(shape.get()->getPosition());
      //  return r;
    }
    
private:
   
};


#endif /* Stern_hpp */
