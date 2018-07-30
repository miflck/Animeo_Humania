//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Bird_hpp
#define Bird_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class Bird:public MovingObject {
    
public:
    Bird();
    virtual ~Bird();
    
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    float actualTriangleAngle;
    float triangleAngleTarget;

    
private:
   
};


#endif /* Bird_hpp */
