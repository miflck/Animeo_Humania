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

#define FREE 1
#define SCARED 2
#define HOME 3
#define INSIDE 4
#define WATCH 5



class BreathPoint:public MovingObject {
    
public:
    BreathPoint();
    virtual ~BreathPoint();
    
    
    void setup();
    void update();
    void draw();
    
    void setTarget(ofVec2f _target);
    
    float getRadius();
    
    int strokeWeight=6;
    
    void setReflectionPoint(ofVec2f _r);
    ofVec2f reflectionpoint;
    bool bReflect=false;
    void setReflection(bool _r);
  bool  bBreathe=true;
    
    
    bool bWatch=false;
    void setWatch(bool _b);
    bool getWatch();
    
    float initTime;
    
    
    
    int state;
    int stateBefore;
    void setState(int _state);
    int getState();
    
    
    int size1=20;
    int size2=75;
    int size3=150;
  //  int sizeWatch=60;
    int sizeWatch=120;


    float seekforce0=0.9;

    int seekforce1=5;
    int seekforce2=10;
    int seekforce3=50;

    
private:
   
};


#endif /* BreathPoint_hpp */
