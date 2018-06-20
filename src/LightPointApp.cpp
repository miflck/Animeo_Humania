//
//  LightPointApp.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "LightPointApp.hpp"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"


LightPointApp::LightPointApp(){
    init();
}


LightPointApp::~LightPointApp(){
    
}

void LightPointApp::init(){
    cout<<"init LightPointApp"<<endl;
    bAddedListeners = false;
    mover.setup();
    mover.setTarget(ofVec2f(ofGetWidth(),ofGetHeight()));
    
    screen.allocate(1920,1080, GL_RGB);
    screen.begin();
    ofClear(0,0,0,0);
    ofClearAlpha();

    screen.end();
    
    ofEnableAlphaBlending();
    ofVec2f homeposition(0,0);
    
    ofAddListener(APPC->oscmanager.onMessageReceived, this, &LightPointApp::onMessageReceived);

}

void LightPointApp::update(){
   
    screen.begin();
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofSetColor(3);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofFill();
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofDisableBlendMode();
    screen.end();
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    switch (skelettonNodeId) {
        case 5:
            mover.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
            break;
    }
    
    if(mskel.size()>skelettId){
        
        
        if(bRepusion){
            ofVec2f f;
            f=mover.getDistance(mskel[skelettId].head);
            if(f.length()>50 && f.length()<100){
                mover.applyForce(f,5);
            }
        }
        
        mover.setReflectionPoint(mskel[skelettId].spineBase);

        
    //for(int i=0;i<mskel.size();i++){
        switch (skelettonNodeId) {
            case 8:
                   mover.setTarget(mskel[skelettId].head);
                break;
                
            case 6:
                mover.setTarget(mskel[skelettId].leftHand);
                break;
                
            case 4:
                mover.setTarget(mskel[skelettId].rightHand);
                break;
                
       
                
            default:
                break;
        }
     
        
   // }
    }
    
    
    
    mover.update();
    
  if(bSendOSCPosition)APPC->oscmanager.sendPositionToLayer(m8layer,mover.getPosition().x-ofGetWidth()/2,-mover.getPosition().y+ofGetHeight()/2);

}


void LightPointApp::draw(){


    if(bRecord){
      //  ofClear(0);
        screen.begin();

       // fade out func from https://forum.openframeworks.cc/t/left-gray-trail-in-ofenableblendmode-of-blendmode-alpha-alpha-trail-alpha-blending-mac-openframeworks/17702/7
       // glEnable(GL_BLEND);
       // glBlendFunc(GL_ONE, GL_ONE);
        //glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
        
        
     
        
      /*  float fClearOpacity = 1.f;
        ofSetColor(0, 5);
        ofFill();
        ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight());
        */
        //glDisable(GL_BLEND);
        //glBlendEquation(GL_FUNC_ADD);
        //glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);

        ofPushStyle();
        ofSetColor(255);
        mover.draw();
        ofPopStyle();
        screen.end();
        screen.draw(0,0);

    }
    
    ofPushStyle();
    mover.draw();
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    ofSetLineWidth(8);
    ofPushMatrix();
 
   /* for(int i=0;i<mskel.size();i++){
        ofPushMatrix();
        mskel[i].drawSkeletton();
        ofPopMatrix();
    }*/
    ofPopMatrix();
    ofPopStyle();
    
    

    
    
}

void LightPointApp::exit(){
    cout<<"exit LightPointApp"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void LightPointApp::keyPressed(ofKeyEventArgs &e){
    cout<<e.key<<endl;
    if(e.key==OF_KEY_LEFT){
        mover.applyForce(ofVec2f(-1,0));
    }
    if(e.key==OF_KEY_RIGHT){
        mover.applyForce(ofVec2f(1,0));
    }
    
    if(e.key==OF_KEY_UP){
        mover.applyForce(ofVec2f(0,-1));
    }
    if(e.key==OF_KEY_DOWN){
        mover.applyForce(ofVec2f(0,1));
    }
    if(e.key=='t'){
        mover.bSeekTarget=!mover.bSeekTarget;
    }
    if(e.key=='z'){
        mover.bSeekMouse=!mover.bSeekMouse;
    }

    if(e.key=='l'){
        bSendOSCPosition=!bSendOSCPosition;
    }
    
    if(e.key=='8'){
        skelettonNodeId=8;
    }
 
    if(e.key=='6'){
        skelettonNodeId=6;
    }
    
    if(e.key=='4'){
        skelettonNodeId=4;
    }
    
    if(e.key=='5'){
        skelettonNodeId=5;
    }
    if(e.key=='2'){
        skelettonNodeId=2;
    }
    
    
    if(e.key=='+'){
        m8layer++;
        
    }
    
    if(e.key=='-'){
        m8layer--;
        
    }
        
    if(e.key=='0'){
        skelettId=0;
    }
    if(e.key=='1'){
        skelettId=1;
    }
    
    
    if(e.key=='s'){
        mover.scaleTo(150,2.f);
    }
    
    if(e.key=='a'){
        mover.scaleTo(600,20.f);
    }
    
    if(e.key=='d'){
        mover.scaleTo(50,0.5f);
    }
    
    if(e.key=='q'){
        mover.setSeekForce(5);
    }
    if(e.key=='w'){
        mover.setSeekForce(0.9);
    }
    
    if(e.key=='e'){
        mover.setSeekForce(0.5);
    }
  
    
    
    if(e.key=='r'){
        bRecord=!bRecord;
        screen.begin();
        ofClear(0,0,0,0);
        ofClearAlpha();
        
        screen.end();
    }
    
    if(e.key=='p'){
        toggleRepulsion();
    }
    
    
    if(e.key=='i'){
        mover.setSlowDown(true);
    }
    
    if(e.key=='o'){
        mover.setSlowDown(false);
    }
    
    if(e.key =='j'){
        homeposition.set(ofGetMouseX(),ofGetMouseY());
    }
    
    if(e.key=='u'){
        mover.setTarget(homeposition);
        skelettonNodeId=2;
        mover.setSeekForce(5);
        
    }
    
    
    if(e.key=='k'){
        mover.setReflectionPoint(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    }
    
    if(e.key=='i'){
        mover.setReflection(true);
    }
    
    if(e.key=='I'){
        mover.setReflection(false);
    }
    
}



void LightPointApp::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void LightPointApp::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}


void LightPointApp::toggleRepulsion(){
    bRepusion=!bRepusion;
}

//--------------------------------------------------------------
void LightPointApp::mouseMoved(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LightPointApp::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LightPointApp::mousePressed(ofMouseEventArgs &a){
    mover.setTarget(ofVec2f(a.x,a.y));
}

//--------------------------------------------------------------
void LightPointApp::mouseReleased(ofMouseEventArgs &a){
    
}

void LightPointApp::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LightPointApp::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LightPointApp::mouseExited(ofMouseEventArgs &a){
    
}

void LightPointApp::onMessageReceived(ofxOscMessage &msg){
    
   
    
    if(msg.getAddress() == "/Light/xy1")
    {
        float x=msg.getArgAsFloat(0);
        float y=msg.getArgAsFloat(1);
        x=ofMap(x,0,1,0,ofGetWidth());
        y=ofMap(y,0,1,0,ofGetHeight());
        cout<<x<<" "<<y<<endl;
        mover.setTarget(ofVec2f(x,y));
    }
    
    if(msg.getAddress() == "/Light/push2")
    {
        float f=msg.getArgAsFloat(0);
        mover.setSeekForce(5);
    }
    if(msg.getAddress() == "/Light/push3")
    {
        float f=msg.getArgAsFloat(0);
        mover.setSeekForce(1);
    }
    if(msg.getAddress() == "/Light/push4")
    {
        float f=msg.getArgAsFloat(0);
        mover.setSeekForce(0.6);
    }
    
    if(msg.getAddress() == "/Light2/xy2")
    {
        float x=msg.getArgAsFloat(0);
        float y=msg.getArgAsFloat(1);
        x=ofMap(x,0,1,0,ofGetWidth());
        y=ofMap(y,0,1,0,ofGetHeight());
        mover.setTarget(ofVec2f(x,y));
    }
    
    
    if(msg.getAddress() == "/Light/push6")
    {
        mover.scaleTo(50,0.5);
    }
    if(msg.getAddress() == "/Light/push7")
    {
        mover.scaleTo(150,2.f);
    }
    if(msg.getAddress() == "/Light/push8")
    {
        mover.scaleTo(600,20.f);
    }
    
}

