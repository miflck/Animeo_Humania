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
#include "AvatarKreis.hpp"

#define IDLE 0
#define FACE 100
#define AVATAR 200
//#define FADEOUT 300



class Humania:public Avatar {
    
public:
    Humania();
    virtual ~Humania();
    
    
    //virtual void drawAvatar();
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
    
    
    float headRadiusBig;
    float headRadiusNormal;
    
    
    
    float actualEyeRadius;
    float actualEyeDiameter;
    float eyeDiameterTarget;

    float eyeRadiusTarget;
    float eyeEasingInitTime;
    float eyeEasingDuration;
    
    
    // head position cant always be from tracking
    // but tracked head is used for eyes. so we need both
    ofVec2f relativeHeadPosition;
    
    bool bHeadIsBound=false;
    
    
    void setBigEyes();
    void setSmallEyes();
    
    void openEyes();
    void closeEyes();
    
    bool bHasEyes=false;
    bool bEyesAreBound=false;
    bool bHasMouth=false;
    bool bMouthIsBound=false;
    
    bool bHasCheecks=false;
    float actualCheeksAlpha=0;
    float cheeksAlphaTarget=0;
    float cheeksInitTime;
    float cheeksEasingDuration;
    void toggleCheeks();
    void showCheeks(bool _b);

    
    
    bool bHasNose=false;
    float actualNoseAlpha=0;
    float noseAlphaTarget=0;
    float noseInitTime;
    float noseEasingDuration;
    void toggleNose();
    void showNose(bool _b);

    bool bHasHair=false;

    float actualHairAlpha=0;
    float hairAlphaTarget=0;
    float hairInitTime;
    float hairEasingDuration;
    void   showHair(bool _b);
    float hairRotationTarget;
    float actualHairRotation;
    
    bool bHasBody=false;
    void toggleBody();
    void showBody(bool _b);

    
    
    int bigEyeRadius=50;
    int smallEyeRadius=10;
    
    //
    ofVec2f *startposition;
    void saveStartposition(ofVec2f _p);

    void resetToStart();
    
    
    
    float scaleFactor;
    
    float bBindPosition=true;
    void bindPosition(bool _b);
    
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
    
    void drawBody();
    
    
    
    MovingObject leftEyeMover;
    MovingObject rightEyeMover;
    ofVec2f leftEyeTarget;
    ofVec2f rightEyeTarget;
    vector<MovingObject*>eyemovers;

    
    MovingObject leftMouthMover;
    MovingObject rightMouthMover;
    vector<MovingObject*>mouthmovers;
    
    ofVec2f leftMouthTarget;
    ofVec2f rightMouthTarget;
    

    ofVec2f getLeftEyePosition();
    ofVec2f getRightEyePosition();

    ofVec2f getLeftMouthPosition();
    ofVec2f getRightMouthPosition();
    ofVec2f getMouthCenterPosition();

    AvatarKreis faceCircle;

    
    
};

#endif /* Humania_hpp */
