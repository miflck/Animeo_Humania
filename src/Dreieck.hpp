//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Dreieck_hpp
#define Dreieck_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"
#include "ofxBox2d.h"
#define START 1
#define MOVINGOBJECT 2
#define PHYSICS 3
#define FADEOUT 4


class Dreieck:public MovingObject {
    
public:
    Dreieck(ofVec2f _a,ofVec2f _b,ofVec2f _c);
    virtual ~Dreieck();
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    b2World* world;
    void setWorld(b2World* w);
    ofxBox2dPolygon triangleAnchor;
    ofVec2f a,b,c;
    ofVec2f center;
    

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
    
    
    bool bShouldRemove=false;
    
    static bool shouldRemoveFromScreen(shared_ptr<Dreieck> shape) {
        bool r=false;
        if(shape->bShouldRemove)r=true;
       // return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
        return r;
    }
    
private:
   
};


#endif /* Dreieck_hpp */
