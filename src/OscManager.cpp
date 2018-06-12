//
//  OscManager.cpp
//  modul8_osc
//
//  Created by Flückiger Michael on 21.05.18.
//

#include "OscManager.hpp"

void OscManager::setup(){
    
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
    
}

void OscManager::update(){
    
   
    
    
}


void OscManager::draw(){
    
}


void OscManager::sendMessage(int _x){
    ofxOscMessage m;
    m.setAddress("/md8key/ctrl_layer_position_x/1");
    m.addIntArg(_x);
    sender.sendMessage(m, false);
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


