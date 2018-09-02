//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef AvatarApp_hpp
#define AvatarApp_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "ofxBox2d.h"
#include "MovingObject.hpp"
#include "Avatar.hpp"
#include "ofxOsc.h"
#include "Humania.hpp"
#include "AvatarKreis.hpp"



class AvatarApp:public ApplicationBase{
public:
   AvatarApp();
    virtual ~AvatarApp();
    
    

    

    
    void turnOn();
    void turnOff();
    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();
    void keyPressed(ofKeyEventArgs &e);
    
    void mouseMoved(ofMouseEventArgs &a );

     void mouseDragged(ofMouseEventArgs &a);
     void mousePressed(ofMouseEventArgs &a);
     void mouseReleased(ofMouseEventArgs &a);
     void mouseScrolled(ofMouseEventArgs &a);
     void mouseEntered(ofMouseEventArgs &a);
     void mouseExited(ofMouseEventArgs &a);

    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    
    void toggleMouseActive();
    
    
    
    void startImitate();
    void stopImitate();
    void setImitate(bool _b);
    bool bIsImitating=false;
    
    
    
    // record
    bool bIsRecodig=false;
    void setIsRecording(bool b);
   // bool getIsRecording();
    
    bool bIsPlayback=false;
    void setIsPlayback(bool _b);
    
    void setAvatarReactionSpeed(float _speed);
    
    
    float scaleAvatar=1;
    
    
private:
   
    
    
    
    bool bIsMouseActive=false;

    //AvatarApp
    ofVec2f facePosition;
    ofVec2f eyePosition;
    ofVec2f eyeOffset;
    ofVec2f leftMouth;
    ofVec2f rightMouth;
    ofVec2f mouthCenterPosition;

    ofVec2f leftMouthOffset;
    ofVec2f rightMouthOffset;
    
    ofVec2f leftMouthCornerOffset;
    ofVec2f rightMouthCornerOffset;
    
  

    bool bHasEyes=false;
    bool bEyeReactive=false;
    
    bool bHasFace=false;
    bool bHasMouth=false;
    
    
    Humania humania;
    
    vector<Avatar *>avatars;
    
    void addAvatar();
    void removeAvatar();
    bool bRemoveAvatar=false;

    
    ofVec2f avatarOffset;
    ofVec2f mainAvatarOffset;

    
    void sendAllOSCControlls();
    
    
    void onMessageReceived(ofxOscMessage &msg);
    
    
    // Skeletton choose
    int skelettonId=0;
    void setSkelettonId(int id);
    int cycleSkelettonId();
    
    bool bindPositionToSkeletton=false;
    
    AvatarKreis faceCircle;
    AvatarKreis leftEye;
    AvatarKreis rightEye;
    
    ofVec2f startposition;
    
    ofVec2f *savedAvatarAddPosition;
    ofVec2f avatarAddPosition;
    ofVec2f *savedMainAvatarOffset;
    
    vector<ofSoundPlayer>ploppsounds;
    void playRandomPlopp();
    
    
    void reset();
    void resetHumania();
    
    
};


#endif /* Physics_hpp */
