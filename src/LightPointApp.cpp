//
//  LightPointApp.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "LightPointApp.hpp"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"
#include "ofxOscMessage.h"


LightPointApp::LightPointApp(){
    init();
}


LightPointApp::~LightPointApp(){
    
}

void LightPointApp::init(){
    ofAddListener(APPC->oscmanager.onOSCSetup, this, &LightPointApp::onOSCSetup);

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
   // ofVec2f homeposition(0,0);
    
    ofAddListener(APPC->oscmanager.onMessageReceived, this, &LightPointApp::onMessageReceived);
   // mover.setPosition(ofVec2f(ofGetWidth()/3,ofGetHeight()/3));



    homeposition=&Settings::getVec2("LightPointApp/homeposition");
    startposition=&Settings::getVec2("LightPointApp/startposition");
    
    cabinposition=&Settings::getVec2("LightPointApp/cabinposition");
    cabindimension=&Settings::getVec2("LightPointApp/cabindimension");

    setMoverToStartPosition();
    
    state=BOUNCE;

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
    
    
    
    
    switch (state) {
        case BOUNCE:
            bounceFromCabin();
            break;
        
        case ENTER:
            // entering from left working?
            if(cabinRect.inside(mover.getPosition().x-mover.getRadius(), mover.getPosition().y-mover.getRadius())){
                switchState(BOUNCE);
            }
            break;
            
        case LEAVE:
            if(!cabinRect.inside(mover.getPosition().x+mover.getRadius(), mover.getPosition().y+mover.getRadius())){
                cout<<"LEAVE"<<endl;
                switchState(BOUNCE);
            }
            break;
            
            
        default:
            break;
    }
    
    bounceFromWalls();
    
    cabinRect=ofRectangle(cabinposition->x,cabinposition->y, cabindimension->x, cabindimension->y);
    
    if(cabinRect.intersects(mover.getPosition(), mover.getPosition()+mover.getSpeed())){
        cout<<"intersect"<<endl;
    };
    ofPolyline p;
    
    
    
 /*   if(bBounceFromCabin){
        bounceFromCabin();
    }
    
    //STATE CONTROLL FOR GETTING INSIDE
    // SHOULD IMPLEMET STATE MACHINE!!!
    // not working from left
    if(goInside && cabinRect.inside(mover.getPosition().x-mover.getRadius(), mover.getPosition().y-mover.getRadius())){
        cout<<"INSIDE"<<endl;
        goInside=false;
        bBounceFromCabin=true;
    }*/
    
    lastPosition=mover.getPosition();
    mover.update();
    
  if(bSendOSCPosition)APPC->oscmanager.sendPositionToLayer(m8layer,mover.getPosition().x-ofGetWidth()/2,-mover.getPosition().y+ofGetHeight()/2);

}



void LightPointApp::bounceFromWalls(){
    ofVec2f position=mover.getPosition();
    float radius=mover.getRadius();
    ofVec2f speed=mover.getSpeed();
    
    if(position.x+radius>=ofGetWidth() || position.x-radius <0){
        ofVec2f sReflected=speed;
        sReflected*=-1;
        mover.setSpeed(sReflected.x, speed.y);
    }
    
    if(position.y+radius>=ofGetHeight() || position.y-radius <0){
        ofVec2f sReflected=speed;
        sReflected*=-1;
        mover.setSpeed(speed.x, sReflected.y);
    }
    
    
    
}

bool LightPointApp::bounceFromCabin(){
    
    ofVec2f position=mover.getPosition();
    float radius=mover.getRadius();
    ofVec2f speed=mover.getSpeed();

    
    //check for top collision:
    if(position.x+radius>=
       cabinposition->x && position.x-radius<=cabinposition->x+cabindimension->x){
        float ball2topEdge = abs(position.y - cabinposition->y); //look up abs()
        if(ball2topEdge <= radius)
        {
            // anti-warp
            if(position.x+radius>=cabinposition->x){
            mover.setPosition(lastPosition.x,lastPosition.y);
            }
            ofVec2f sReflected=speed;
            sReflected*=-1;
            mover.setSpeed(speed.x, sReflected.y);
        }
        
    }
    
    //check for left collision:
    if(position.y+radius>=
       cabinposition->y && position.y+radius<=cabinposition->y+cabindimension->y){
        float ball2leftEdge = abs(position.x - cabinposition->x);
        //check for left collision:
        if(ball2leftEdge <= radius)
        {
            if(position.x+radius>=cabinposition->x){
                mover.setPosition(lastPosition.x,lastPosition.y);
            }
            ofVec2f sReflected=speed;
            sReflected*=-1;
            mover.setSpeed(sReflected.x, speed.y);
        }
        //check for right collision:
        float ball2rightEdge = abs(position.x - (cabinposition->x+cabindimension->x));
        if(ball2rightEdge <= radius)
        {
            if(position.x-radius<=cabinposition->x+cabindimension->x){
                mover.setPosition(lastPosition.x,lastPosition.y);
            }
            ofVec2f sReflected=speed;
            sReflected*=-1;
            mover.setSpeed(sReflected.x, speed.y);
        }
    }
 
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
    
    if(APPC->debug){
       // cout<<cabinposition<<endl;
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofPushMatrix();
        ofTranslate(cabinposition->x,cabinposition->y);
        ofDrawRectangle(0,0, cabindimension->x, cabindimension->y);
        ofPopMatrix();
        ofPopStyle();
    }
    
    
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
        homeposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");

    }
    
    if(e.key =='J'){
        startposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
    }
    
    
    if(e.key =='g'){
        cabinposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
        
    }
    if(e.key =='G'){
        ofVec2f cD=ofVec2f(ofGetMouseX(),ofGetMouseY());
        cD-=*cabinposition;
        cabindimension->set(cD);
        Settings::get().save("data.json");
    }
    
    
    if(e.key=='u'){
        goHome();
    }
    
    
    if(e.key=='U'){
        setMoverToStartPosition();
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
    
    if(e.key==OF_KEY_CONTROL){
        switchState(ENTER);
    }
    
    if(e.key==OF_KEY_ALT){
        switchState(LEAVE);
    }
    
}

void LightPointApp::switchState(int _newstate){
    stateBefore=state;
    state=_newstate;
    
    switch (_newstate) {

        
        case BOUNCE:
            mover.scaleTo(150,2.f);
            break;
            
        case ENTER:
            mover.scaleTo(50,2.f);
            break;
            
        default:
            break;
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



void LightPointApp::setMoverToStartPosition(){
    mover.setPosition(startposition->x, startposition->y);
    mover.setTarget(ofVec2f(startposition->x, startposition->y));
}


void LightPointApp::goHome(){
    mover.setTarget(*homeposition);
    mover.scaleTo(50,0.5);
    skelettonNodeId=2;
    mover.setSeekForce(5);
    ofxOscMessage m;
    m.addFloatArg(ofMap(homeposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(homeposition->x,0,ofGetWidth(),0,1));
    m.setAddress("/Light/xy1");
    APPC->oscmanager.touchOscSender.sendMessage(m);
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
    
    
    if(msg.getAddress() == "/Light/push9")
    {
        goHome();
        
    }
    
    
}

void LightPointApp::onOSCSetup(ofxOscMessage &msg){
    ofxOscMessage m;
    m.addFloatArg(ofMap(startposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(startposition->x,0,ofGetWidth(),0,1));
    m.setAddress("/Light/xy1");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
}
