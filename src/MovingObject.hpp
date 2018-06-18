//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef MovingObject_hpp
#define MovingObject_hpp
#include "ofMain.h"
#include <stdio.h>



class MovingObject {
    
public:
    
    MovingObject();
    virtual ~MovingObject();
    
   virtual void setup();
   virtual void update();
   virtual void draw();
    
    
    void setPosition(int x, int y);
    ofVec2f getPosition();
    
    void setRadius(int radius);
    int getRadius();
    
    void setSpeed(int _x, int _y);
    
    void setTarget(ofVec2f _target);
    void move();
    void applyForce(ofVec2f _force);
    
    ofVec2f seek(ofVec2f t, float f);

    bool bSeekTarget=false;
    bool bSeekMouse=false;
    
    void scaleTo(float _value);
    void scaleTo(float _value,float _duration);


    float easingInitTime;
    float radiusTarget;
    float actualRadius;
    float scaleDuration;
    
    
    void setSeekForce(float _f);
        
        

private:
    ofVec2f position;
    int radius;
    
    ofVec2f velocity;
    ofVec2f target;
   int maxspeed=50;
    
    ofVec2f acceleration;
    float seekforce=0.5;
    
};


#endif /* MovingObject_hpp */
