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
    
    bool isOnScreen();
    
    
    ofVec2f getDistance(ofVec2f _t);
    
    void setRadius(int radius);
    int getRadius();
    
    void setSpeed(int _x, int _y);
    
    void setTarget(ofVec2f _target);
    void move();
    void applyForce(ofVec2f _force);
    void applyForce(ofVec2f _force,float _strength);

    
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
        
    void setSlowDown(bool _b);
    void setSlowDownDistance(int _d);
    
    
    int radius;

    
    
private:
    ofVec2f position;
    
    ofVec2f velocity;
    ofVec2f target;
   int maxspeed=50;
    
    ofVec2f acceleration;
    float seekforce=0.5;
    
    bool bSlowDown=true;
    int slowdowndistance=300;
    
};


#endif /* MovingObject_hpp */
