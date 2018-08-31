//
//  OscManager.hpp
//  modul8_osc
//
//  Created by Flückiger Michael on 21.05.18.
//

#ifndef OscManager_hpp
#define OscManager_hpp

#include <stdio.h>
#include "ofxOsc.h"
#include "ofMain.h"

//#define HOST "localhost"
#define HOST "169.254.69.100"
#define PORT 8000


#define MUSICHOST "169.254.69.200"
#define MUSICPORT 9000

//#define TOUCHOSCHOST "169.254.112.125"
#define TOUCHOSCHOST "192.168.1.167"


#define TOUCHOSCPORT 9000


class OscManager {
public:
    void setup();
    void update();
    void draw();
    
    
    bool bIsInitialized=false;
    
   // ofxOscSender sender;
    
    //ofxOscSender musicSender;

    
    ofxOscReceiver touchOscReceiver;
    ofxOscSender touchOscSender;

    //this is the magic! This is the port on which your game gets incoming data.
    
    ofEvent<ofxOscMessage> onMessageReceived;
    ofEvent<ofxOscMessage> onOSCSetup;

    
    ofBuffer imgAsBuffer;
    
    
    
  /*  void sendMessage(int _x);
    void sendPositionToLayer(int _layer, int _x, int _y);
   
    void sendMusicMessage(int _x);
*/
    
    string remoteIp;
    bool bRemoteIpIsSet=false;
};



#endif /* OscManager_hpp */
