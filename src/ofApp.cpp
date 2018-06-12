#include "ofApp.h"
#include "ApplicationController.h"

//--------------------------------------------------------------
void ofApp::setup(){
    APPC->initialize();
    ofSetVerticalSync(true);
    ofEnableAntiAliasing();
    ofBackground(0);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case '1':
        cout<<"set app"<<endl;
        APPC->setAppById("testApp");
        break;
    case '2':
            cout<<"set app"<<endl;
            APPC->setAppById("line");
    break;

    case '3':
            cout<<"set app"<<endl;
            APPC->setAppById("physics");
    break;
            
        case '4':
            cout<<"set app"<<endl;
            APPC->setAppById("physicsworld");
            break;
            
            
        case '5':
            cout<<"set app"<<endl;
            APPC->setAppById("kinectV2");
            break;
            
            
    case 'd':
            APPC->toggleDebug();
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
