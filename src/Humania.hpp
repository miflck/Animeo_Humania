//
//  Humania.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 17.07.18.
//

#ifndef Humania_hpp
#define Humania_hpp

#include <stdio.h>
#include "ofMain.h"
#include "MovingObject.hpp"
#include "Avatar.hpp"
#include "helper.hpp"
#include "ofxJsonSettings.h"

#define IDLE 0
#define FACE 100
#define AVATAR 200
//#define FADEOUT 300



class Humania:public Avatar {
    
public:
    Humania();
    virtual ~Humania();
    
    
    virtual void drawAvatar();
    virtual void drawFaceAvatar();

    
    void setup();
    void update();
    void draw();
    
    int state;
    int stateBefore;
    void setState(int _state);
    int getState();
    
    
    // head easing
    float actualHeadRadius;
    float headRadiusTarget;
    float headEasingInitTime;
    float headEasingDuration;
    
    
    float actualEyeRadius;
    float actualEyeDiameter;
    float eyeDiameterTarget;

    float eyeRadiusTarget;
    float eyeEasingInitTime;
    float eyeEasingDuration;
    
    
    
    void setBigEyes();
    void setSmallEyes();

    
    
    //
    ofVec2f *startposition;
    void saveStartposition(ofVec2f _p);

    void resetToStart();
    
    
    
    //face
    
    //FACE
    ofVec2f eyePosition;
    ofVec2f eyeOffset;
    ofVec2f leftMouth;
    ofVec2f rightMouth;
    ofVec2f mouthCenterPosition;
    ofVec2f leftMouthOffset;
    ofVec2f rightMouthOffset;
    
    ofVec2f leftMouthCornerOffset;
    ofVec2f rightMouthCornerOffset;
    
    ofPath mouth;
    
    
    void updateFace();
    void drawFace();
    
    
    
};

#endif /* Humania_hpp */
