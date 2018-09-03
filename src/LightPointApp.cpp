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
    
    mover.setSlowDownDistance(500);
    
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
    scaredposition=&Settings::getVec2("LightPointApp/scaredposition");
    cabinposition=&Settings::getVec2("LightPointApp/cabinposition");
    cabindimension=&Settings::getVec2("LightPointApp/cabindimension");
    watchposition=&Settings::getVec2("LightPointApp/watchposition");
    insidepositon=&Settings::getVec2("LightPointApp/insidePoisiton");
    
    
    
    cabinpositionInside=&Settings::getVec2("LightPointApp/cabinpositioninside");
    cabindimensionInside=&Settings::getVec2("LightPointApp/cabindimensioninside");

    setMoverToStartPosition();
    mover.bSeekTarget=true;
    state=BOUNCE;
    mover.scaleTo(size2,0.1);
    
    
    //string bp = ofSystem("/usr/local/bin/SwitchAudioSource -s 'iRig PRO DUO'");
    

    // Load Bouncesounds
   /* char* files;
    for(int i=0;i<soundsLength;i++){
        sprintf(files,"BallBounces/Boing%i.wav",i);
        bouncesounds[i].load(files);
    }*/

    
    ofDirectory dir;
    dir.listDir("BallBounces");
    dir.sort();
    bouncesounds.resize(dir.size());
    
    for(int i=0; i<dir.size(); i++ ){
        bouncesounds[i].load(dir.getPath(i));
        ofLog(OF_LOG_NOTICE,"songs loaded")<<i<<" path "<<dir.getPath(i);
    }
    

}

void LightPointApp::update(){
   
    
    lastPosition=mover.getPosition();
    mover.update();
    
    ofSoundUpdate();

    
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
 
    
 // if(bSendOSCPosition)APPC->oscmanager.sendPositionToLayer(m8layer,mover.getPosition().x-ofGetWidth()/2,-mover.getPosition().y+ofGetHeight()/2);
    
    
    //  if(bSendOSCPosition)APPC->oscmanager.sendMusicMessage(100);

}



void LightPointApp::bounceFromWalls(){
    ofVec2f position=mover.getPosition();
    float radius=mover.getRadius();
    ofVec2f speed=mover.getSpeed();
    
    if(position.x+radius>=ofGetWidth() || position.x-radius <0){
        ofVec2f sReflected=speed;
        sReflected*=-1;
        mover.setSpeed(sReflected.x, speed.y);
        mover.setPosition(lastPosition.x,lastPosition.y);
    }
    
    if(position.y+radius>=ofGetHeight() || position.y-radius <0){
        ofVec2f sReflected=speed;
        sReflected*=-1;
        mover.setSpeed(speed.x, sReflected.y);
        mover.setPosition(lastPosition.x,lastPosition.y);
    }
    
}

bool LightPointApp::bounceFromCabin(){
    
    ofVec2f position=mover.getPosition();
    float radius=mover.getRadius();
    ofVec2f speed=mover.getSpeed();
    
    
   /* bIsInside=false;
    if(lastPosition.x+radius>=cabinposition->x && lastPosition.x-radius<=cabinposition->x+cabindimension->x){
        if(lastPosition.y+radius>=cabinposition->y && lastPosition.y+radius<=cabinposition->y+cabindimension->y){
            bIsInside=true;
        }
    }*/

        
    
  
    
    ofVec2f cabin(cabinposition->x,cabinposition->y);
    if(bIsInside)cabin.set(*cabinpositionInside);
    
    ofVec2f cabindim(cabindimension->x,cabindimension->y);
    if(bIsInside)cabindim.set(*cabindimensionInside);
    
   // cout<<speed.length()<<endl;
    //check for top collision:
    if(position.x+radius>=
       cabin.x && position.x-radius<=cabin.x+cabindim.x){
        float ball2topEdge = abs(position.y - cabin.y); //look up abs()
        float dist=ball2topEdge-radius;


        if(ball2topEdge <= radius)
        {
            
           // cout<<" dist "<<ball2topEdge-radius<<endl;

            // anti-warp
            if(position.x+radius>=cabin.x){
            mover.setPosition(lastPosition.x,lastPosition.y);
            }
          
            ofVec2f sReflected=speed;
            sReflected*=-1;
             mover.setSpeed(speed.x, sReflected.y);
          playRandomSound();
            
        }
    }
    
 
    
    //check for left collision:
    if(position.y+radius>=cabin.y && position.y+radius<=cabin.y+cabindim.y){
        
        float ball2leftEdge = abs(position.x - cabin.x);
        //check for left collision:
        if(ball2leftEdge <= radius)
        {

                if(position.x+radius>=cabin.x){
                    mover.setPosition(lastPosition.x,lastPosition.y);
                    
                }
                ofVec2f sReflected=speed;
                sReflected*=-1;
                mover.setSpeed(sReflected.x, speed.y);
            playRandomSound();
            
           
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
               playRandomSound();
            
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
        
        ofSetColor(255, 255, 0);
        ofPushMatrix();
        ofTranslate(cabinpositionInside->x,cabinpositionInside->y);
        ofDrawRectangle(0,0, cabindimensionInside->x, cabindimensionInside->y);
        ofPopMatrix();
        
        
        
        if(bIsInside){
            
            ofSetColor(0,0,255);
            
        }else{
            ofSetColor(0, 255, 0);
        }
        mover.draw();
        ofPopStyle();
        
    }
    
   
    
    
}

void LightPointApp::exit(){
    cout<<"exit LightPointApp"<<endl;
}



void LightPointApp::playRandomSound(){
    int randNum;
    randNum=(int)(ofRandom(0,bouncesounds.size()));
    if(!bIsMute)bouncesounds[randNum].play();
}


void LightPointApp::setInside(bool b){
    bIsInside=b;
    
    ofxOscMessage mM;
    mM.addBoolArg(bIsInside);
    mM.setAddress("/Light/toggle35");
    APPC->oscmanager.touchOscSender.sendMessage(mM);
}

//KEY LISTENER
//--------------------------------------------------------------




void LightPointApp::keyPressed(ofKeyEventArgs &e){
    cout<<"Light point key"<<e.key<<endl;
    //keyIsDown[e.key]=true;
    
  /*  cout<<"pressed"<<e.key<<endl;

    
    if(keyIsDown['a']&&keyIsDown['b']){
        cout<<"COMBO!"<<endl;
    }*/
    
    

    
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
   

    if(e.key=='l'){
       // bSendOSCPosition=!bSendOSCPosition;
        //setMoverToStartPosition();
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
       // skelettonNodeId=2;
    }
    
    
  
        
    if(e.key=='0'){
        skelettId=0;
    }
    if(e.key=='1'){
      //  skelettId=1;
    }
    
    
  /*  if(e.key=='s'){
        mover.scaleTo(150,2.f);
    }
    
    if(e.key=='a'){
        mover.scaleTo(600,20.f);
    }
    
    if(e.key=='d'){
        mover.scaleTo(50,0.5f);
    }
    */
    if(e.key=='q'){
        mover.setSeekForce(5);
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
      //  mover.setSlowDown(true);
        insidepositon->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
    }
    
 
    
   
    if(e.key =='S'){
        scaredposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
    }
    
    
    if(e.key=='P'){
        getScared();
    }
    
    if(e.key =='j'){
        homeposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");

    }
    
    if(e.key =='J'){
        startposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
    }
    
    if(e.key =='w'){
        watchposition->set(ofGetMouseX(),ofGetMouseY());
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
    
    
    if(e.key=='i'){
        //  mover.setSlowDown(true);
        insidepositon->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
    }
    
    if(e.key=='o'){
        cabinpositionInside->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");    }
    
    if(e.key=='O'){
        ofVec2f cD=ofVec2f(ofGetMouseX(),ofGetMouseY());
        cD-=*cabinpositionInside;
        cabindimensionInside->set(cD);
        Settings::get().save("data.json");
    }
    
    
    if(e.key=='U'){
        goHome();
    }
    
    
    if(e.key=='u'){
        setMoverToStartPosition();
    }
    
    
    if(e.key=='k'){
        mover.setReflectionPoint(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    }
    
    if(e.key=='i'){
        //mover.setReflection(true);
    }
    
    if(e.key=='I'){
        //mover.setReflection(false);
    }
    
    if(e.key==OF_KEY_CONTROL){
        cout<<"Enter"<<endl;
        switchState(ENTER);
        
    }
    
    if(e.key==OF_KEY_ALT){
        switchState(LEAVE);
    }
    
    if(e.key==OF_KEY_COMMAND){
        setMute(!bIsMute);
    }
    
}

void LightPointApp::switchState(int _newstate){
    stateBefore=state;
    state=_newstate;
    
    switch (_newstate) {

        
        case BOUNCE:
            mover.scaleTo(size2,0.1);
            break;
            
        case ENTER:
            mover.scaleTo(size1,2.f);
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
    
    setInside(false);
    setMute(false);
    
    mover.setPosition(startposition->x, startposition->y);
    mover.setTarget(ofVec2f(startposition->x, startposition->y));
    
    ofxOscMessage m;
    m.addFloatArg(ofMap(startposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(startposition->x,0,ofGetWidth(),0,1));
    m.setAddress("/Light/xy1");
   if (APPC->oscmanager.bIsInitialized) APPC->oscmanager.touchOscSender.sendMessage(m);
    mover.setRadius(size2);
        
    
}


void LightPointApp::goHome(){
    mover.setTarget(*homeposition);
    mover.setState(HOME);

    //mover.scaleTo(size2,0.5);
    skelettonNodeId=2;
    //mover.setSeekForce(5);
    ofxOscMessage m;
    m.addFloatArg(ofMap(homeposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(homeposition->x,0,ofGetWidth(),0,1));
    m.setAddress("/Light/xy1");
    APPC->oscmanager.touchOscSender.sendMessage(m);
}

void LightPointApp::getScared(){
    mover.setTarget(*scaredposition);
    mover.setState(SCARED);
    //mover.scaleTo(size1,0.5);
    skelettonNodeId=2;
    //mover.setSeekForce(50);
    ofxOscMessage m;
    m.addFloatArg(ofMap(scaredposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(scaredposition->x,0,ofGetWidth(),0,1));
    m.setAddress("/Light/xy1");
    APPC->oscmanager.touchOscSender.sendMessage(m);
}

void LightPointApp::getWatch(){
    if(mover.getState()==WATCH){
        mover.setState(FREE);
    }else{
    mover.setTarget(*watchposition);
    mover.setState(WATCH);
    skelettonNodeId=2;
    }
    ofxOscMessage m;
    m.addFloatArg(ofMap(watchposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(watchposition->x,0,ofGetWidth(),0,1));
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
        mover.setSlowDownDistance(500);

    }
    if(msg.getAddress() == "/Light/push3")
    {
        float f=msg.getArgAsFloat(0);
        mover.setSeekForce(0.9);
        mover.setSlowDownDistance(1000);
    }
    if(msg.getAddress() == "/Light/push4")
    {
  //      float f=msg.getArgAsFloat(0);
//        mover.setSeekForce(0.9);
        switchState(ENTER);
        mover.setSeekForce(10);
        mover.setSlowDownDistance(500);
        mover.setTarget(*insidepositon);
        
        ofxOscMessage m;
        m.addFloatArg(ofMap(insidepositon->y,0,ofGetHeight(),0,1));
        m.addFloatArg(ofMap(insidepositon->x,0,ofGetWidth(),0,1));
        m.setAddress("/Light/xy1");
        APPC->oscmanager.touchOscSender.sendMessage(m);

    }
    

    
    
    if(msg.getAddress() == "/Light/push6")
    {
        mover.scaleTo(mover.size1,0.5);
    }
    if(msg.getAddress() == "/Light/push7")
    {
        mover.scaleTo(mover.size2,0.6f);
    }
    if(msg.getAddress() == "/Light/push8")
    {
        mover.scaleTo(mover.size3,1.f);
    }
    
    
    if(msg.getAddress() == "/Light/push9")
    {
        goHome();
        
    }
    
    if(msg.getAddress() == "/Light/push14")
    {
        getScared();
        
    }
    
    if(msg.getAddress() == "/Light/push34")
    {
      //  mover.setState(WATCH);
        getWatch();
    }
    
    
    if(msg.getAddress() == "/Light/toggle33")
    {
        //  mover.setState(WATCH);
       // setMute(!bIsMute);
        float f=msg.getArgAsBool(0);
        setMute(f);
    }
    
    if(msg.getAddress() == "/Light/toggle35")
    {
        float f=msg.getArgAsBool(0);
        setInside(f);
    }
    
    
}


void LightPointApp::setMute(bool m){
    bIsMute=m;
    
    ofxOscMessage mM;
    mM.addBoolArg(m);
    mM.setAddress("/Light/toggle33");
    APPC->oscmanager.touchOscSender.sendMessage(mM);
}

bool LightPointApp::getIsMute(){
    return bIsMute;
}


void LightPointApp::onOSCSetup(ofxOscMessage &msg){
    ofxOscMessage m;
    m.addFloatArg(ofMap(startposition->y,0,ofGetHeight(),0,1));
    m.addFloatArg(ofMap(startposition->x,0,ofGetWidth(),0,1));
    m.setAddress("/Light/xy1");
    APPC->oscmanager.touchOscSender.sendMessage(m);
    
}
