//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef BreathPoint_hpp
#define BreathPoint_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class BreathPoint:public MovingObject {
    
public:
    BreathPoint();
    virtual ~BreathPoint();
    
    
    void setup();
    void update();
    void draw();
    
    float getRadius();
    
    int strokeWeight=6;
    
    void setReflectionPoint(ofVec2f _r);
    ofVec2f reflectionpoint;
    bool bReflect=false;
    void setReflection(bool _r);
  bool  bBreathe=true;
private:
   
};


#endif /* BreathPoint_hpp */
