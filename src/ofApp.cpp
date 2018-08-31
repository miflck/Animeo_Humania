#include "ofApp.h"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    Settings::get().load("data.json");
    
    savedKinectPosition=&Settings::getVec2("Kinect/kinectPosition");
    savedBeamerPosition=&Settings::getVec2("Kinect/beamerPosition");
    kinectScaleFact=&Settings::getFloat("Kinect/kinectScaleFact");
    beamerFOV=&Settings::getFloat("Kinect/beamerFOV");

    ofAddListener(APPC->oscmanager.onMessageReceived, this, &ofApp::onMessageReceived);

    
    

    soundStream.printDeviceList();
    //if you want to set a different device id
    // soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
   soundStream.setDeviceID(3);
    
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
    //ofDisableAntiAliasing();
    ofSetFrameRate(60);
    ofEnableAntiAliasing();

    
    
    ofSetCircleResolution(50);


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
    
    
    
    ofEnableSmoothing();

    if(bGreenScreen){
        ofColor c=ofColor(255,0,0);
        c.setHueAngle(139);
        c.setBrightness(100);
        ofBackground(c);
    }else{
        ofBackground(0);
    }
    //mClient.draw(50, 50);
    
    string info = "";
    info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
    // info += "FPS: "+ofToString(APPC->ofGetFrameRate())+"\n";
    
  
    
    // draw the average volume:
    ofPushStyle();
    ofPushMatrix();
     if(APPC->debug){
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
    }

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
            APPC->setAppById("avatarapp");
    break;
            
     
            

            
   
            
        case 'm':
            cout<<"set app"<<endl;
            APPC->setAppById("emotionworld");
            break;
            
  
        case '.':
            APPC->cycleSkelettonIndex();
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
    case 'd':
            APPC->toggleDebug();
    break;
            
            
        case 'l':
            ofShowCursor();
            break;
        case 'L':
            ofHideCursor();
            break;
            
            
        case '-':
            KINECTMANAGER->addInputListeners();
            break;
            
        case '_':
            KINECTMANAGER->removeInputListeners();
            break;
        
            
        case '$':
            APPC->setAppNull();
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


void ofApp::onMessageReceived(ofxOscMessage &msg){
    
    // Beamerpos
    if(msg.getAddress() == "/Settings/fader21")
    {
        float x = msg.getArgAsFloat(0);
        float mappedX=ofMap(x,-1,1,-800,800);
        ofxOscMessage m;
        m.addFloatArg(mappedX);
        m.setAddress("/Settings/label86");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        ofVec3f p;
        p.set(mappedX,APPC->gui->beamerPosition->y,APPC->gui->beamerPosition->z);
        APPC->gui->beamerPosition.set(p);
    }
    if(msg.getAddress() == "/Settings/fader22")
    {
        float y = msg.getArgAsFloat(0);
        float mappedY=ofMap(y,-1,1,-10000,10000);
        ofxOscMessage m;
        m.addFloatArg(mappedY);
        m.setAddress("/Settings/label85");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        ofVec3f p;
        p.set(APPC->gui->beamerPosition->x,mappedY,APPC->gui->beamerPosition->z);
        APPC->gui->beamerPosition.set(p);
    }
    if(msg.getAddress() == "/Settings/fader23")
    {
        float z = msg.getArgAsFloat(0);
        float mappedZ=ofMap(z,0,1,500,15000);
        ofxOscMessage m;
        m.addFloatArg(mappedZ);
        m.setAddress("/Settings/label84");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        ofVec3f p;
        p.set(APPC->gui->beamerPosition->x,APPC->gui->beamerPosition->y,mappedZ);
        APPC->gui->beamerPosition.set(p);
    }
    
    
    //Kinectpos
    if(msg.getAddress() == "/Settings/fader26")
    {
        float x = msg.getArgAsFloat(0);
        float mappedX=ofMap(x,-1,1,-700,700);
        ofxOscMessage m;
        m.addFloatArg(mappedX);
        m.setAddress("/Settings/label89");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        ofVec3f p;
        p.set(mappedX,APPC->gui->kinectPosition->y,APPC->gui->kinectPosition->z);
        APPC->gui->kinectPosition.set(p);
    }
    if(msg.getAddress() == "/Settings/fader25")
    {
        float y = msg.getArgAsFloat(0);
        float mappedY=ofMap(y,-1,1,-700,700);
        ofxOscMessage m;
        m.addFloatArg(mappedY);
        m.setAddress("/Settings/label88");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        ofVec3f p;
        p.set(APPC->gui->kinectPosition->x,mappedY,APPC->gui->kinectPosition->z);
        APPC->gui->kinectPosition.set(p);
    }
    if(msg.getAddress() == "/Settings/fader24")
    {
        float z = msg.getArgAsFloat(0);
        float mappedZ=ofMap(z,-1,1,-9000,9000);
        ofxOscMessage m;
        m.addFloatArg(mappedZ);
        m.setAddress("/Settings/label87");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        ofVec3f p;
        p.set(APPC->gui->kinectPosition->x,APPC->gui->kinectPosition->y,mappedZ);
        APPC->gui->kinectPosition.set(p);
    }
    
    
    if(msg.getAddress() == "/Settings/fader30")
    {
        float z = msg.getArgAsFloat(0);
        float mappedfov=ofMap(z,0,1,0,80);
        ofxOscMessage m;
        m.addFloatArg(mappedfov);
        m.setAddress("/Settings/label91");
        APPC->oscmanager.touchOscSender.sendMessage(m);
       
        APPC->gui->beamerFov.set(mappedfov);
    }
    
    //LOAD
    if(msg.getAddress() == "/Settings/push59")
    {
        bGreenScreen=!bGreenScreen;
        
    }

    
    
    //LOAD
    if(msg.getAddress() == "/Settings/push51")
    {
        loadSettings();
    }
    
    //SAVE
    
    if(msg.getAddress() == "/Settings/push52")
    {
        saveSettings();
    }
    
    if(msg.getAddress() == "/Settings/push52")
    {
        saveSettings();
    }
    
}

void ofApp::saveSettings(){
    cout<<"SAVE ME!!"<<endl;

    APPC->gui->gui.saveToFile("settings.xml");

}

void ofApp::loadSettings(){
    
    cout<<"load"<<APPC->gui->beamerPosition->x<<endl;
    //beamer
    float mappedV=ofMap(APPC->gui->beamerPosition->x,-800,800,-1,1);
    cout<<"load"<<mappedV<<endl;

    ofxOscMessage m;
    m.addFloatArg(mappedV);
    m.setAddress("/Settings/fader21");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
    mappedV=ofMap(APPC->gui->beamerPosition->y,-10000,10000,-1,1);

    m.clear();
    m.addFloatArg(mappedV);
    m.setAddress("/Settings/fader22");
    APPC->oscmanager.touchOscSender.sendMessage(m);
   
    mappedV=ofMap(APPC->gui->beamerPosition->z,500,15000,0,1);
    m.clear();
    m.addFloatArg(mappedV);
    m.setAddress("/Settings/fader23");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
    
    
    //kinect
    mappedV=ofMap(APPC->gui->kinectPosition->x,-700,700,-1,1);

     m.clear();
    m.addFloatArg(mappedV);
    m.setAddress("/Settings/fader26");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
    mappedV=ofMap(APPC->gui->kinectPosition->y,-700,700,-1,1);

    m.clear();
    m.addFloatArg(mappedV);
    m.setAddress("/Settings/fader25");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
    
    mappedV=ofMap(APPC->gui->kinectPosition->z,-9000,9000,-1,1);


    m.clear();
    m.addFloatArg( mappedV);
    m.setAddress("/Settings/fader24");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
    mappedV=ofMap(APPC->gui->beamerFov,0,80,0,1);
    cout<<mappedV<<endl;
    m.clear();
    m.addFloatArg( mappedV);
    m.setAddress("/Settings/fader30");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
}

