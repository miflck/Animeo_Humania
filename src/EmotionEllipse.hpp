//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef EmotionEllipse_hpp
#define EmotionEllipse_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class EmotionEllipse:public MovingObject {
    
public:
    EmotionEllipse();
    virtual ~EmotionEllipse();
    
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    float actualTriangleAngle;
    float triangleAngleTarget;

    void startEasingIn();
    
    
    bool bShouldRemove=false;
    
  /*  static bool shouldRemoveFromScreen(shared_ptr<Ellipse> shape) {
        bool r=false;
        if(shape->bShouldRemove)r=true;
        // return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
        return r;
    }*/
    
private:
   
};


#endif /* Ellipse_hpp */
