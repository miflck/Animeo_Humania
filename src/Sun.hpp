//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Sun_hpp
#define Sun_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class Sun:public MovingObject {
    
public:
    Sun();
    virtual ~Sun();
    
    
    void setup();
    void update();
    void draw();
    
    
    
private:
   
};


#endif /* Sun_hpp */
