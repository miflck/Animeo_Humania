//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Baloon_hpp
#define Baloon_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class Baloon:public MovingObject {
    
public:
    Baloon();
    virtual ~Baloon();
    
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
private:
   
};


#endif /* Baloon_hpp */
