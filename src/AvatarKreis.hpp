//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef AvatarKreis_hpp
#define AvatarKreis_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"
#define START 1
#define MOVINGOBJECT 2
#define RELEASED 3
#define FADEOUT 4


class AvatarKreis:public MovingObject {
    
public:
    AvatarKreis();
    virtual ~AvatarKreis();
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    bool bPhysics=false;
    void turnPhysicsOn(bool _on);
    bool getIsPhysicsOn();
    
    float actualRadius;
    float easingInitTime;
    float radiusTarget;
    float startRadius;
    float scaleDuration=1;
    
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
    
    void setRadiusTarget(float r);
    
    bool bShouldRemove=false;
    
    static bool shouldRemoveFromScreen(shared_ptr<AvatarKreis> shape) {
        bool r=false;
        if(shape->bShouldRemove)r=true;
       // return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
        return r;
    }
    
private:
   
};


#endif /* Kreis_hpp */
