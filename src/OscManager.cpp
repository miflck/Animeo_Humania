//
//  OscManager.cpp
//  modul8_osc
//
//  Created by Flückiger Michael on 21.05.18.
//

#include "OscManager.hpp"

void OscManager::setup(){
    // open an outgoing connection to HOST:PORT
    //sender.setup(HOST, PORT);
     //touchOscSender.setup(TOUCHOSCHOST, TOUCHOSCPORT);
    //musicSender.setup(MUSICHOST, MUSICPORT);
    touchOscReceiver.setup(7000);
    /*this is the port your game will receive data on.
     For us this is the important one! Set your mobile device to send on this port.*/
    touchOscSender.setup(TOUCHOSCHOST, TOUCHOSCPORT);
}

void OscManager::update(){
    //our simple while loop to make sure we get all of our messages
    while (touchOscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        //Pass a reference to that message to the receiver
        //we set up above using the getNextMessage function in the OSC add on.
        touchOscReceiver.getNextMessage(&m);
        
        remoteIp=m.getRemoteIp();

        //cout<<"--------- ip "<<m.getAddress()<<" "<<remoteIp<<m.getRemotePort()<<endl;

        if(!bRemoteIpIsSet){
            remoteIp=m.getRemoteIp();
            if(remoteIp!=TOUCHOSCHOST){
            bRemoteIpIsSet=true;
            touchOscSender.setup(remoteIp, TOUCHOSCPORT);
            cout<<m.getRemoteIp()<<endl;
            ofNotifyEvent(onOSCSetup, m, this);
            }
        }
        //This will be the message we send back from our game
        //to our device letting it know what value we received
        //from it and displaying that back to us so we know what our
        //current game settings are at.
        ofxOscMessage sendBack;
        ofNotifyEvent(onMessageReceived, m, this);
    }
}


void OscManager::draw(){
    
}


/*void OscManager::sendMessage(int _x){
    ofxOscMessage m;
    m.setAddress("/md8key/ctrl_layer_position_x/1"); 
    m.addIntArg(_x);
    sender.sendMessage(m, false);
    cout<<_x<<endl;

}

void OscManager::sendMusicMessage(int _x){
    ofxOscMessage m;
    m.setAddress("/test");
    m.addIntArg(_x);
    musicSender.sendMessage(m, false);
    cout<<_x<<endl;
    
}

void OscManager::sendPositionToLayer(int _layer, int _x, int _y){
    
    ofxOscMessage m;
    m.setAddress("/md8key/ctrl_layer_position_x/"+ofToString(_layer));
    m.addIntArg(_x);
    sender.sendMessage(m, false);
    
    ofxOscMessage m2;
    m2.setAddress("/md8key/ctrl_layer_position_y/"+ofToString(_layer));
    m2.addIntArg(_y);
    sender.sendMessage(m2, false);
}

*/
