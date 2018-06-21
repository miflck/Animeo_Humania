#include "ofApp.h"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    Settings::get().load("data.json");

    
    APPC->initialize();
    APPC->gui=gui;
    
    KINECTMANAGER->initialize();
    
    ofSetVerticalSync(true);
    ofEnableAntiAliasing();
    ofBackground(0);
    
    
    ofSetCircleResolution(100);


    //mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    //mClient.set("","Simple Server");
    

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    //mClient.draw(50, 50);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case 'y':
        cout<<"set app"<<endl;
        APPC->setAppById("lightpoint");
        break;
    case 'x':
            cout<<"set app"<<endl;
            APPC->setAppById("linieApp");
    break;

        case 'c':
            cout<<"set app"<<endl;
            APPC->setAppById("physics");
    break;
            
     
            
        case 'v':
            cout<<"set app"<<endl;
            APPC->setAppById("arm");
            break;
            
        case 'n':
            cout<<"set app"<<endl;
            APPC->setAppById("physicsworld");
            break;
            
        case 'm':
            cout<<"set app"<<endl;
            APPC->setAppById("emotionworld");
            break;
            
        case ',':
            cout<<"set app"<<endl;
            APPC->setAppById("face");
            break;
            
        case '.':
            cout<<"set app"<<endl;
            APPC->setAppById("line");
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
    case 'd':
            APPC->toggleDebug();
    break;
            
        case '-':
            KINECTMANAGER->addInputListeners();
            break;
            
        case '_':
            KINECTMANAGER->removeInputListeners();
            break;
 
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
