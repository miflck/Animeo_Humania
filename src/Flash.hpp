//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Flash_hpp
#define Flash_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class Flash:public MovingObject {
    
public:
    Flash();
    virtual ~Flash();
    
    ofRectangle screen;

    
    void setup();
    void setup(ofImage *img);

    void update();
    void draw();
    
    ofImage *flashImage;
    
    
    int strokeWeight=6;
    
    static bool shouldRemoveOffScreen(shared_ptr<Flash> shape) {
        return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
    }
    
private:
   
};


#endif /* Flash_hpp */
