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
    
    
    void setup();
    void update();
    void draw();
    
    ofImage flashImage;
    
    
    int strokeWeight=6;
    
private:
   
};


#endif /* Flash_hpp */
