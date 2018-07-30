//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Balloon_hpp
#define Balloon_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class Balloon:public MovingObject {
    
public:
    Balloon();
    virtual ~Balloon();
    
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    float actualTriangleAngle;
    float triangleAngleTarget;

    void startEasingIn();
    
private:
   
};


#endif /* Baloon_hpp */
