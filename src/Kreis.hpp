//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Kreis_hpp
#define Kreis_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"
#include "ofxBox2d.h"



class Kreis:public MovingObject {
    
public:
    Kreis();
    virtual ~Kreis();
    
    
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
        
    
private:
   
};


#endif /* Kreis_hpp */
