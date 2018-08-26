//
//  AvatarApp.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "AvatarApp.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"
#include "ofxOscMessage.h"



AvatarApp::AvatarApp(){
    init();
}


AvatarApp::~AvatarApp(){
    
}

void AvatarApp::init(){
    cout<<"init AvatarApp"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    screen.allocate(1920,1080, GL_RGB);
    screen.begin();
    ofClear(0,0,0,0);
    screen.end();
    
    
    savedAvatarAddPosition=&Settings::getVec2("Avatar/addPosition");
    avatarAddPosition.set(savedAvatarAddPosition->x,savedAvatarAddPosition->y);
    
    facePosition=ofVec2f(ofGetWidth()/3,ofGetHeight()/2);
    mouthCenterPosition=ofVec2f(0,150);
    leftMouth=ofVec2f(200,0);
    rightMouth=ofVec2f(-200,0);
    
    leftHandMover.bSeekTarget=true;
    leftHandMover.setSeekForce(2);
    
    
  
    humania.setup();
    humania.bSeekTarget=true;
    
    
    savedMainAvatarOffset=&Settings::getVec2("Avatar/mainAvatarOffset");

    mainAvatarOffset.set(*savedMainAvatarOffset);
    
    avatarOffset.set(250,0);
    
    ofAddListener(APPC->oscmanager.onMessageReceived, this, &AvatarApp::onMessageReceived);

    
    faceCircle.bSeekTarget=true;
    leftEye.bSeekTarget=true;
    rightEye.bSeekTarget=true;
    
    ofDirectory dir;
    dir.listDir("Sounds/Plopp");
    dir.sort();
    ploppsounds.resize(dir.size());
    
    for(int i=0; i<dir.size(); i++ ){
        ploppsounds[i].load(dir.getPath(i));
        ofLog(OF_LOG_NOTICE,"songs loaded")<<i<<" path "<<dir.getPath(i);
    }

}

void AvatarApp::update(){
    
    
    if(faceCircle.getState()==RELEASED && faceCircle.getSpeed().length()<0.1){
      
    }
    
    if(faceCircle.getState()==RELEASED && faceCircle.getReached()){
        humania.setState(FACE);
        faceCircle.setState(FADEOUT);
    }
    
    if(leftEye.getState()==RELEASED && leftEye.getReached()&& rightEye.getState()==RELEASED && rightEye.getReached()){
        humania.openEyes();
        leftEye.setState(FADEOUT);
        rightEye.setState(FADEOUT);
        
      //  humania.bEyesAreBound=true;
        
        
    }
    
    faceCircle.update();
    leftEye.update();
    rightEye.update();
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    if(mskel.size()-1<skelettonId){
        setSkelettonId(mskel.size()-1);
    }
    
    if(mskel.size()>0 && bindPositionToSkeletton){
       ofVec2f target=mskel[skelettonId].spineBase+mainAvatarOffset;
      //  target.y-=humania.actualHeadRadius;
        humania.setTarget(target);
    }
    
    // debug without kinect
    /*
    if(bindPositionToSkeletton){
        ofVec2f testPoint=ofVec2f(ofGetMouseX(),ofGetMouseY());
        ofVec2f target=testPoint+mainAvatarOffset;
        //target.y-=humania.actualHeadRadius;
        humania.setTarget(target);
    }*/
    
    
    if(mskel.size()>0){
        ofVec2f epos;
        ofVec2f lefthand=mskel[skelettonId].leftHand;
        ofVec2f righthand=mskel[skelettonId].rightHand;
        ofVec2f middle=(lefthand-righthand)/2;
        epos.set(righthand+middle);
        startposition.set(epos);
        if(faceCircle.getState()==MOVINGOBJECT){
            faceCircle.setTarget(startposition);
            faceCircle.updateRadiusTarget(middle.length());
        }
        if(leftEye.getState()==MOVINGOBJECT){
            leftEye.setTarget(startposition);
            leftEye.updateRadiusTarget(middle.length());
        }
        if(rightEye.getState()==MOVINGOBJECT){
            rightEye.setTarget(startposition);
            rightEye.updateRadiusTarget(middle.length());
            
        }
    }
    
    humania.update();
    box2d.update();
  
    if(avatars.size()>0){
        avatars[0]->setTarget(humania.getPosition()-avatarOffset);
        avatars[0]->setSiblingBones(humania.getBonesPositions());
        avatars[0]->update();

        for(int i=1;i<avatars.size();i++){
            avatars[i]->setTarget(avatars[i-1]->getPosition()-avatarOffset);
            avatars[i]->setSiblingBones(avatars[i-1]->getBonesPositions());//setSiblingBones(avatars[i-1]->getBonesPositions());
            avatars[i]->update();
        }
    }
  
 

    
    if(mskel.size()>0){
        
        head=mskel[skelettonId].head;
        
        leftHand=ofVec2f(mskel[skelettonId].leftHand.x,mskel[skelettonId].leftHand.y);
        leftEllbow=ofVec2f(mskel[skelettonId].leftEllbow.x,mskel[skelettonId].leftEllbow.y);
        leftShoulder=ofVec2f(mskel[skelettonId].leftShoulder.x,mskel[skelettonId].leftShoulder.y);
        leftHip=mskel[skelettonId].leftHip;
        leftKnee=mskel[skelettonId].leftKnee;
        leftAnkle=mskel[skelettonId].leftAnkle;
        leftFoot=mskel[skelettonId].leftFoot;
        leftWrist=mskel[skelettonId].leftWrist;

        
        rightHand=ofVec2f(mskel[skelettonId].rightHand.x,mskel[skelettonId].rightHand.y);
        rightEllbow=ofVec2f(mskel[skelettonId].rightEllbow.x,mskel[skelettonId].rightEllbow.y);

        rightShoulder=ofVec2f(mskel[skelettonId].rightShoulder.x,mskel[skelettonId].rightShoulder.y);
        rightHip=mskel[skelettonId].rightHip;
        rightKnee=mskel[skelettonId].rightKnee;
        rightAnkle=mskel[skelettonId].rightAnkle;
        rightFoot=mskel[skelettonId].rightFoot;
        rightWrist=mskel[skelettonId].rightWrist;

        
        neck=mskel[skelettonId].neck;
        spineBase=ofVec2f(mskel[skelettonId].spineBase.x,mskel[skelettonId].spineBase.y);
        spineMid=ofVec2f(mskel[skelettonId].spineMid.x,mskel[skelettonId].spineMid.y);
        
        
        leftHandMover.setTarget(leftHand);
        
     
    }
    
    if(bRemoveAvatar)removeAvatar();

    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
  
}


void AvatarApp::draw(){
    
    if(avatars.size()>0){
        for(int i=avatars.size()-1;i>=0;i--){
            avatars[i]->draw();
        }
    }
    
    humania.draw();
    
    
    faceCircle.draw();
    

    leftEye.draw();
    rightEye.draw();
   
    

    
    /*for(int i=0;i<avatars.size();i++){
        avatars[i]->draw();
    }*/
    
  
   /* ofPushMatrix();
    ofTranslate(-300,0);
    ofSetLineWidth(10);
    ofDrawCircle(leftHandMover.getPosition(),10);
    ofDrawLine(leftHand,leftEllbow);
    ofDrawCircle(leftEllbow,5);

    ofDrawLine(leftEllbow,leftShoulder);
    
    
    ofDrawLine(rightShoulder,leftShoulder);

    
    ofDrawCircle(leftShoulder,5);

    ofSetColor(255);
    ofVec2f headCenter=ofVec2f(head.x,head.y-40);

    ofDrawCircle(headCenter,80);
    ofSetColor(0);

    

    ofDrawCircle(head.x-20,head.y-60,10);
    ofDrawCircle(head.x+20,head.y-60,10);
    
    ofVec2f dist=ofVec2f(80,0);
    dist.rotate(-20);

    for(int i=0;i<6;i++){
        ofVec2f pos=headCenter;
        pos+=dist;
        ofSetColor(0);
        ofDrawCircle(pos,20);
        ofSetColor(255);
        ofDrawCircle(pos,17);
        dist.rotate(-180/6);
    }
    ofSetColor(255,0,0);

    ofDrawCircle(headCenter.x-40,headCenter.y+10,20);
    ofDrawCircle(headCenter.x+40,headCenter.y+10,20);

    
    ofSetColor(255);

    ofDrawLine(leftShoulder,neck);
    ofDrawCircle(neck,5);

    
    ofDrawCircle(rightHand,10);
    ofDrawLine(rightHand,rightEllbow);
    ofDrawLine(rightEllbow,rightShoulder);
    ofDrawCircle(rightShoulder,5);
    ofDrawCircle(rightEllbow,5);

    
    ofDrawLine(rightShoulder,neck);
    ofDrawLine(neck,spineMid);
    
    ofDrawLine(spineMid,spineBase);

    
    //ofDrawTriangle(neck,leftHip,rightHip);
    
    ofDrawLine(spineBase,leftHip);
    ofDrawLine(spineBase,rightHip);
    
    ofDrawLine(rightHip,rightKnee);
    ofDrawLine(rightKnee,rightFoot);
    
    ofDrawLine(leftHip,leftKnee);
    ofDrawLine(leftKnee,leftFoot);
    ofPopMatrix();
*/
    if(APPC->debug){
        ofPushStyle();
        ofSetColor(255,0,0,150);
        ofDrawCircle(startposition,20);
        ofPopStyle();
    }
    
    
  /*  vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()>0){
        ofPushMatrix();
        ofTranslate(ofGetWidth(), ofGetHeight());
        ofDrawCircle(mskel[0].localZero,10);
        ofDrawCircle(mskel[0].leftHandLocal,10);
        ofPopMatrix();
        
        ofDrawCircle(mskel[0].head,10);
        ofDrawCircle(mskel[0].leftHand,10);
        
    }
*/
   
    

    

    
    
}

void AvatarApp::exit(){
    cout<<"exit AvatarApp"<<endl;
}

void AvatarApp::addAvatar(){
    Avatar * a =new Avatar();
    a->setup();
    a->setTarget(ofVec2f(ofRandom(ofGetWidth()),ofRandom(ofGetHeight())));
    //a->setPosition(humania.getPosition().x,humania.getPosition().y);
    a->setPosition(avatarAddPosition);

    
    a->setLeftEyePosition(humania.getLeftEyePosition());
    a->setRightEyePosition(humania.getRightEyePosition());
    a->setLeftMouthPosition(humania.getLeftMouthPosition());
    a->setRightMouthPosition(humania.getRightMouthPosition());
    a->setMouthCenterPosition(humania.getMouthCenterPosition());

    a->bSeekTarget=true;
    a->setSeekForce(50);
    a->setMaxSpeed(100);
    a->bindSkeletton(true);

    if(bIsImitating)a->startImitate();
    avatars.push_back(a);
}


void AvatarApp::removeAvatar(){
    
  
    
    if(avatars.size()>0 ){
        delete avatars[0];
        avatars.erase( avatars.begin());
    }
}


void AvatarApp::setSkelettonId(int id){
    skelettonId= id;
    humania.setSkelettonId(skelettonId);
    
    for(int i=0;i<avatars.size();i++){
        avatars[i]->setSkelettonId(skelettonId);
    }
    
    ofxOscMessage m;
    m.addIntArg(skelettonId);
    m.setAddress("/avatar/label21");
    APPC->oscmanager.touchOscSender.sendMessage(m);
}

int AvatarApp::cycleSkelettonId(){
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

    int actId=skelettonId;
    int tempId=0;
    if(mskel.size()>actId+1){
        tempId=actId+1;
    }
    setSkelettonId(tempId);
}


void AvatarApp::startImitate(){
    bIsImitating=true;
        for(int i=0;i<avatars.size();i++){
            avatars[i]->startImitate();
        }
    setAvatarReactionSpeed(60);
}
void AvatarApp::stopImitate(){
    bIsImitating=false;
    
    for(int i=0;i<avatars.size();i++){
        avatars[i]->stopImitate();
    }
    setAvatarReactionSpeed(150);
}

void AvatarApp::setImitate(bool _b){
    if(_b){
        bIsImitating=true;
        for(int i=0;i<avatars.size();i++){
            avatars[i]->startImitate();
        }
        setAvatarReactionSpeed(60);
    }else{
        bIsImitating=false;
        for(int i=0;i<avatars.size();i++){
            avatars[i]->stopImitate();
        }
        setAvatarReactionSpeed(150);
    }
}
    

void AvatarApp::setIsRecording(bool _b){
    if(_b){
        bIsRecodig=true;
        humania.startRecording();
        
        
    }else{
        bIsRecodig=false;
        humania.stopRecording();
    }
    
    ofxOscMessage m;
    m.addBoolArg(bIsRecodig);
    m.setAddress("/avatar/toggle21");
    APPC->oscmanager.touchOscSender.sendMessage(m);
}


void AvatarApp::setIsPlayback(bool _b){
    if(_b){
        bIsPlayback=true;
        setIsRecording(false);
        humania.startPlayback();
    }else{
        humania.stopPlayback();
        bIsPlayback=false;
    }
    
    ofxOscMessage m;
    m.addBoolArg(bIsPlayback);
    m.setAddress("/avatar/toggle22");
    APPC->oscmanager.touchOscSender.sendMessage(m);
}



void AvatarApp::setAvatarReactionSpeed(float _speed){
    for(int i=0;i<avatars.size();i++){
        avatars[i]->setBoneMoverSpeed(_speed);
    }
}



//KEY LISTENER
//--------------------------------------------------------------
void AvatarApp::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'e') {
      //  bHasEyes=!bHasEyes;
    }
    
    
    if(e.key == 'f') {
        bHasFace=!bHasFace;

    }
    
    if(e.key == 'g') {
        bHasMouth=!bHasMouth;

    }
    
    /*if(e.key=='a'){
        addAvatar();
    }*/
    
    if(e.key=='R'){
        humania.startRecording();
    }
    
    if(e.key=='r'){
        humania.stopRecording();
    }
    
    
    if(e.key=='p'){
        humania.startPlayback();
    }
    
    if(e.key=='P'){
        humania.stopPlayback();
    }
    
    if(e.key=='i'){
        startImitate();
    }
    if(e.key=='I'){
        stopImitate();
    }
    
    
    if(e.key=='A'){
        humania.setState(AVATAR);
    }
    
    if(e.key=='a'){
        humania.setState(FACE);
    }
    
    if(e.key=='S'){
        humania.saveStartposition(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    }
    
   
    if(e.key=='s'){
        savedAvatarAddPosition->set(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        Settings::get().save("data.json");
        avatarAddPosition.set(savedAvatarAddPosition->x,savedAvatarAddPosition->y);
        
    }
    
    if(e.key=='o'){
        savedMainAvatarOffset->set(mainAvatarOffset);
        Settings::get().save("data.json");
    }
    
    if(e.key=='e'){
        //humania.startEyes();
        
        humania.toggleCheeks();
        humania.toggleNose();
        humania.toggleBody();
    }
    
    
}



void AvatarApp::playRandomPlopp(){
    int randNum;
    randNum=(int)(ofRandom(0,ploppsounds.size()));
    ploppsounds[randNum].play();
}

//--------------------------------------------------------------
void AvatarApp::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void AvatarApp::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void AvatarApp::mousePressed(ofMouseEventArgs &a){
   

}

//--------------------------------------------------------------
void AvatarApp::mouseReleased(ofMouseEventArgs &a){
    
}

void AvatarApp::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void AvatarApp::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void AvatarApp::mouseExited(ofMouseEventArgs &a){
    
}


void AvatarApp::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void AvatarApp::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void AvatarApp::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}



void AvatarApp::onMessageReceived(ofxOscMessage &msg){
    if(msg.getAddress() == "/avatar/xy2")
    {
        
        float x=msg.getArgAsFloat(0);
        float y=msg.getArgAsFloat(1);
        x=ofMap(x,0,1,0,ofGetWidth());
        y=ofMap(y,0,1,0,ofGetHeight());
        humania.setTarget(ofVec2f(x,y));
    }
    
    if(msg.getAddress() == "/avatar/push19")
    {
        addAvatar();
    }
    if(msg.getAddress() == "/avatar/fader6")
    {
        float x=msg.getArgAsFloat(0);
        x=ofMap(x,0,1,0,500);
        avatarOffset.set(x,avatarOffset.y);
    }
    

    
    
    if(msg.getAddress() == "/avatar/push20")
    {
        removeAvatar();
    }
    
    if(msg.getAddress() == "/avatar/push21")
    {
        cycleSkelettonId();
        ofxOscMessage m;
        m.addIntArg(skelettonId);
        m.setAddress("/avatar/label21");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    
    if(msg.getAddress() == "/avatar/toggle7")
    {
        float m=msg.getArgAsBool(0);
        bindPositionToSkeletton=m;
    }
    
    
    // Main Offset
    if(msg.getAddress() == "/avatar/fader7")
    {
        float x=msg.getArgAsFloat(0);
        x=ofMap(x,-1,1,-500,500);
        mainAvatarOffset.set(x,mainAvatarOffset.y);
    }
    
    if(msg.getAddress() == "/avatar/fader10")
    {
        float y=msg.getArgAsFloat(0);
        y=ofMap(y,-1,1,-500,500);
        mainAvatarOffset.set(mainAvatarOffset.x,y);
    }
    
    
   
    // reaction speed
    if(msg.getAddress() == "/avatar/fader8")
    {
        float x=msg.getArgAsFloat(0);
        x=ofMap(x,0,1,20,150);
        setAvatarReactionSpeed(x);
    }
    
    //--------------------------
    //Humania FAce
    if(msg.getAddress() == "/Face/toggle8")
    {
        float m=msg.getArgAsBool(0);
        if(m){
            cout<<"set Big Eyes"<<endl;
            //humania.setBigEyes();
            humania.openEyes();
            
        }else{
            //humania.setSmallEyes();
            humania.closeEyes();
            cout<<"set Small Eyes"<<endl;
        }
    
    }
    
    if(msg.getAddress() == "/Face/toggle10")
    {
        float m=msg.getArgAsBool(0);
        humania.bEyesAreBound=m;
        
    }
    
    
    if(msg.getAddress() == "/Face/toggle9")
    {
        float m=msg.getArgAsBool(0);
        humania.bHasMouth=m;
    }
    
    if(msg.getAddress() == "/Face/toggle11")
    {
        float m=msg.getArgAsBool(0);
        humania.bMouthIsBound=m;
    }
    
    
    
    if(msg.getAddress() == "/Face/toggle14")
    {
        float m=msg.getArgAsBool(0);
        humania.showCheeks(m);
    }
    
    if(msg.getAddress() == "/Face/toggle16")
    {
        float m=msg.getArgAsBool(0);
        humania.showNose(m);
    }
    
    if(msg.getAddress() == "/Face/toggle12")
    {
        float m=msg.getArgAsBool(0);
        humania.showBody(m);
       
        //make body reactive
        bindPositionToSkeletton=m;
        //humania.bindSkeletton(m);


    }
    
    if(msg.getAddress() == "/Face/toggle13")
    {
        float m=msg.getArgAsBool(0);
        humania.bindSkeletton(m);

    }
    
    if(msg.getAddress() == "/avatar/toggle18")
    {
        float m=msg.getArgAsBool(0);
        humania.bindPosition(m);
        
    }
    
    if(msg.getAddress() == "/Face/toggle19")
    {
        cout<<"HAIR"<<endl;
        float m=msg.getArgAsBool(0);
        humania.showHair(m);
    }
    
    if(msg.getAddress() == "/Face/push22")
    {
        humania.setState(FACE);

    }
    
    if(msg.getAddress() == "/Face/push23")
    {
        cout<<"Push23"<<endl;
        humania.setState(AVATAR);

    }
    
    
    if(msg.getAddress() == "/avatar/toggle20")
    {
        cout<<"Imitate"<<endl;
        float m=msg.getArgAsBool(0);
        setImitate(m);
    }
    
    if(msg.getAddress() == "/avatar/toggle21")
    {
        cout<<"Recording"<<endl;
        float m=msg.getArgAsBool(0);
        setIsRecording(m);
    }
    
    if(msg.getAddress() == "/avatar/toggle22")
    {
        cout<<"Playback"<<endl;
        float m=msg.getArgAsBool(0);
        setIsPlayback(m);
    }
    
    if(msg.getAddress() == "/Face/push44")
    {
        faceCircle.setPosition(startposition);
        faceCircle.easingInitTime=ofGetElapsedTimef();
        faceCircle.setRadiusTarget(300);
        faceCircle.setState(RELEASED);
        faceCircle.setTarget(humania.getPosition());
        faceCircle.setSlowDownDistance(800);
        faceCircle.setScaleDuration(1);
        faceCircle.color=ofColor(255);

        
    }
    
    if(msg.getAddress() == "/Face/push45")
    {
        leftEye.setPosition(startposition);
        leftEye.easingInitTime=ofGetElapsedTimef();
        leftEye.setRadiusTarget(40);
        leftEye.setState(RELEASED);
        leftEye.setTarget(ofVec2f(humania.getPosition().x-80,humania.getPosition().y-40));
        leftEye.setSlowDownDistance(800);
        leftEye.color=ofColor(50);
        leftEye.setScaleDuration(1);

        
    }
    
    if(msg.getAddress() == "/Face/push46")
    {
        rightEye.setPosition(startposition);
        rightEye.easingInitTime=ofGetElapsedTimef();
        rightEye.setRadiusTarget(40);
        rightEye.setState(RELEASED);
        rightEye.setTarget(ofVec2f(humania.getPosition().x+150,humania.getPosition().y+40));
        rightEye.setSlowDownDistance(800);
        rightEye.color=ofColor(50);
        leftEye.setScaleDuration(1);

    }
    
    
    if(msg.getAddress() == "/Face/push47")
    {
        faceCircle.setPosition(startposition);
        faceCircle.easingInitTime=ofGetElapsedTimef();
        faceCircle.startRadius=0;
        faceCircle.setRadiusTargetWithStartRadius(10,0);
        faceCircle.setState(MOVINGOBJECT);
        faceCircle.setTarget(startposition);
        faceCircle.setScaleDuration(0.3);
        faceCircle.color=ofColor(255);
        playRandomPlopp();

        
    }
    
    if(msg.getAddress() == "/Face/push48")
    {
        leftEye.setPosition(startposition);
        leftEye.easingInitTime=ofGetElapsedTimef();
        leftEye.setRadiusTargetWithStartRadius(10,0);
        leftEye.setState(MOVINGOBJECT);
        leftEye.setTarget(startposition);
        leftEye.setSlowDownDistance(800);
        leftEye.color=ofColor(200);
        leftEye.setScaleDuration(0.3);
        playRandomPlopp();

        
    }
    
    if(msg.getAddress() == "/Face/push49")
    {
        rightEye.setPosition(startposition);
        rightEye.easingInitTime=ofGetElapsedTimef();
        rightEye.setRadiusTargetWithStartRadius(10,0);
        rightEye.setState(MOVINGOBJECT);
        rightEye.setTarget(startposition);
        rightEye.setSlowDownDistance(800);
        rightEye.color=ofColor(200);
        rightEye.setScaleDuration(0.3);
        playRandomPlopp();

    }
    
    
    if(msg.getAddress() == "/Face/push50")
    {
        faceCircle.setPosition(startposition);
        rightEye.setPosition(startposition);
        leftEye.setPosition(startposition);

       
        humania.setState(IDLE);
        humania.closeEyes();
        
        leftEye.setState(IDLE);
        rightEye.setState(IDLE);
        faceCircle.setState(IDLE);

        
    }
    
    
    
    
    
}

