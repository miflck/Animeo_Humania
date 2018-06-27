#include "ofApp.h"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    Settings::get().load("data.json");

    soundStream.printDeviceList();
    //if you want to set a different device id
    // soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
   soundStream.setDeviceID(0);
    
    int bufferSize = 256;
    
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    bufferCounter    = 0;
    drawCounter        = 0;
    smoothedVol     = 0.0;
    scaledVol        = 0.0;
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
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
    //lets scale the vol up to a 0-1 range
   // cout<<smoothedVol<<endl;
    //scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    APPC->audioInVolume=smoothedVol;
}

//--------------------------------------------------------------
void ofApp::draw(){
    //mClient.draw(50, 50);
    
    
    // draw the average volume:
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()-200, ofGetHeight()-200, 0);
    ofScale(0.5,0.5);
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofDrawRectangle(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofDrawCircle(200, 200, scaledVol * 190.0f);
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, 400);
        
        ofVertex(i, 400 - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i, 400);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
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
            
            
        case OF_KEY_CONTROL:
            cout<<"KEY Controll"<<endl;
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

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]        = input[i*2]*0.5;
        right[i]    = input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}
