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

#define TOUCHOSCHOST "169.254.73.86"
#define TOUCHOSCPORT 9000


class OscManager {
public:
    void setup();
    void update();
    void draw();
    
    ofxOscSender sender;
    
    ofxOscReceiver touchOscReceiver;
    ofxOscSender touchOscSender;

    //this is the magic! This is the port on which your game gets incoming data.
    
    ofEvent<ofxOscMessage> onMessageReceived;

    
    ofBuffer imgAsBuffer;
    
    void sendMessage(int _x);
    void sendPositionToLayer(int _layer, int _x, int _y);


};



#endif /* OscManager_hpp */
