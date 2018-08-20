//
//  LinieApp.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "LinieApp.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


LinieApp::LinieApp(){
    init();
}


LinieApp::~LinieApp(){
    
}

void LinieApp::init(){
    cout<<"init LinieApp"<<endl;
    bAddedListeners = false;
    
    ofAddListener(APPC->oscmanager.onMessageReceived, this, &LinieApp::onMessageReceived);

    
    
    box2d.init();
    box2d.setGravity(0, 0);
   // box2d.createGround();

    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    float xPos=(ofGetWidth()-250);

    anchorPositionTop.set(xPos, ofGetHeight()/3);
    anchorPositionBottom.set(xPos,ofGetHeight());
    
    anchorStartPositionTop=&Settings::getVec2("LinieApp/anchorStartPositionTop");
    anchorStartPositionBottom=&Settings::getVec2("LinieApp/anchorStartPositionBottom");

    
    finalCirclePosition=&Settings::getVec2("LinieApp/finalCirclePosition");

    
    //anchor.setPhysics(50, 0.5, 0.9);

    anchor.setup(box2d.getWorld(), anchorStartPositionTop->x, anchorStartPositionTop->y, 10);
    anchor2.setup(box2d.getWorld(), anchorStartPositionBottom->x, anchorStartPositionBottom->y, 10);
    
    waveInitPosition = *anchorStartPositionTop;
    
    int jointlength=3;
    
    // first we add just a few circles
    for(int i=0; i<numCircles; i++) {
        
        auto circle = std::make_shared<ofxBox2dCircle>();
        circle.get()->setPhysics(5, 0.53, 0.9);
      
        float offset=anchorStartPositionBottom->y-anchorStartPositionTop->y;
        offset/=numCircles;
        
        circle.get()->setup(box2d.getWorld(), anchorStartPositionTop->x, anchorStartPositionTop->y+(offset*i), 5);
        circles.push_back(circle);
        positions.push_back(ofVec2f(xPos, (ofGetHeight()/(numCircles+1))+(ofGetHeight()/(numCircles+1))*i));
    }
    
    // now connect each circle with a joint
    for(int i=0; i<circles.size(); i++) {
        auto joint = std::make_shared<ofxBox2dJoint>();
        // if this is the first point connect to the top anchor.
        if(i == 0) {
            joint.get()->setup(box2d.getWorld(), anchor.body, circles[i].get()->body,5,1,false);
        }
        else {
            joint.get()->setup(box2d.getWorld(), circles[i-1].get()->body, circles[i].get()->body,5,1,false);
        }
       joint.get()->setLength(jointlength);
        joints.push_back(joint);
    }
    
    auto joint = std::make_shared<ofxBox2dJoint>();
    joint.get()->setup(box2d.getWorld(), circles[circles.size()-1].get()->body, anchor2.body,5,1,false);
    joint.get()->setLength(jointlength);
    joints.push_back(joint);
    
    
    mover1.setup();
    
    mover1.actualRadius=0.2;
    mover1.radiusTarget=10;
    mover1.scaleDuration=20;
    
    moverCircleCenter.set(finalCirclePosition->x,finalCirclePosition->y);

    
    mover1.setPosition(moverCircleCenter);
    mover1.setTarget(ofVec2f(ofGetWidth()/3,ofGetHeight()/3));

    mover1.setSeekForce(10);
    mover1.setMaxSpeed(100);
    
    mover1.bSeekTarget=true;
    moverCircleRadius.set(150,0);
    
    damping=0.99;
    
}

void LinieApp::update(){
    box2d.update();
    if(bMakeCircle){
        moverCircleRadius.rotate(moverCircleSpeed);
        mover1.setTarget(moverCircleCenter+moverCircleRadius);
        mover1.update();
        //anchor.setPosition(mover1.getPosition());
        anchor.addAttractionPoint((moverCircleCenter+moverCircleRadius),circleAttractionForce);


        float ang=(360/(circles.size()+1));
        ofVec2f r=moverCircleRadius.getRotated(-ang);

        if(circleBoundindex>circles.size())circleBoundindex=circles.size();
        
        for(int i=0; i<circleBoundindex; i++) {
            /* float dis = mouse.distance(circles[i].get()->getPosition());
             if(dis < minDis && bIsMouseActive) circles[i].get()->addRepulsionForce(mouse,10);
             if(mskel.size()>0){
             float handDist = leftHand.distance(circles[i].get()->getPosition());
             if(dis < minDis) circles[i].get()->addRepulsionForce(leftHand,10);
             }*/
            // circles[i].get()->setDamping(generalDamping);
            ofVec2f r=moverCircleRadius.getRotated(-ang*(i+1));
         // circles[i].get()->setDamping(damping);
             circles[i]->addAttractionPoint((moverCircleCenter+r),circleAttractionForce);

            
        }
        if(circleBoundindex>=circles.size()){
            anchor2.addAttractionPoint((moverCircleCenter+moverCircleRadius),circleAttractionForce*5);
            cout<<"Close!"<<endl;
        }
       // if(ofGetFrameNum()%10==0)circleBoundindex++;
    }
    
    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
    float minDis = ofGetMousePressed() ? 200 : 100;
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    ofVec2f leftHand;
    if(mskel.size()>0){
         leftHand=mskel[0].leftHand;
    }
    
    for(int i=0; i<circles.size(); i++) {
       /* float dis = mouse.distance(circles[i].get()->getPosition());
        if(dis < minDis && bIsMouseActive) circles[i].get()->addRepulsionForce(mouse,10);
            if(mskel.size()>0){
                float handDist = leftHand.distance(circles[i].get()->getPosition());
                if(dis < minDis) circles[i].get()->addRepulsionForce(leftHand,10);
            }*/
      // circles[i].get()->setDamping(generalDamping);
    //   if(!bMakeCircle) circles[i].get()->setDamping(0.98);
       circles[i].get()->setDamping(damping);


    }
    
    if(!bMakeCircle){
        anchor.setDamping(0.98);
        anchor2.setDamping(0.98);

    }else{
        anchor.setDamping(1);
        anchor2.setDamping(1);


    }
    
    float mV=ofMap(APPC->audioInVolume,0,0.05,0,50);
    cout<<mV<<endl;
    int c=0;
 /* for(int i=0;i<circles.size();i+=5){
        if(c%2==0 || circles[i].get()->getPosition().y>ofGetHeight()-100)mV*=-1;
        circles[i].get()->setPosition(circles[i].get()->getPosition().x, circles[i].get()->getPosition().y+mV);
        c++;
    }*/
  
    
    
 /*  for(int i=0;i<4;i++){
     if(circles[i].get()->getPosition().y>ofGetHeight()-300)mV*=-1;
     circles[i].get()->setPosition(circles[i].get()->getPosition().x, circles[i].get()->getPosition().y+mV);
     }*/
    
   // circles[0].get()->addRepulsionForce(ofGetWidth()/2,ofGetHeight()/2,mV);
   // cout<<mV<<endl;
  
    if(!bMakeCircle){
    if(mV>10 && !bHasStartWave)startWave(mV,mV,10*PI);
   // if(bHasStartWave && mV<15){
    if( mV<10){
       endWave();
    bHasStartWave=false;
    }
        if(bMakeWave){
            waveAmplitude=maxWaveVol(mV);
            wave();
        }
    }

    
    if( bUseHand){
        if(mskel.size()>0 ){
            for(int i=0; i<circles.size(); i++) {
                float handDist = leftHand.distance(circles[i].get()->getPosition());
                if(repulse){
                    //anchor.addRepulsionForce(leftHand,10);
                    if(handDist < minDis) circles[i].get()->addRepulsionForce(leftHand,10);
                }else{
                   // anchor.addAttractionPoint(leftHand,1000);
                    if(handDist < minDis) circles[i].get()->addRepulsionForce(leftHand,-10);
                }
            }
        }
    }else{
        for(int i=0; i<circles.size(); i++) {
             float dis = mouse.distance(circles[i].get()->getPosition());

            if(repulse){
                if(dis < minDis) circles[i].get()->addRepulsionForce(mouse,30);

                //anchor.addRepulsionForce(mouse,50);
            }else{
                if(dis < minDis) circles[i].get()->addAttractionPoint(mouse,100);

                //anchor.addAttractionPoint(mouse,50);
            }
        }
    }
    
   // anchor2.setDamping(0.9);

    //ofVec2f distance=mouse-anchor.getPosition();
  //  anchor2.setVelocity(distance.normalize()*-1);
    ofPolyline cur;
    
    cur.addVertex(anchor.getPosition());
    
    for(int i=0; i<circles.size(); i++) {
        cur.addVertex( circles[i].get()->getPosition());
    }
    
    cur.addVertex(anchor2.getPosition());
    
    line=cur;
    line.getSmoothed(5);
}


void LinieApp::draw(){
    ofSetColor(0);
   // ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    
    
    
    ofPushMatrix();
    ofDrawCircle(moverCircleCenter+moverCircleRadius, 10);
    ofPopMatrix();
    
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(8);
    line.draw();
    
   // ofDrawCircle(anchor.getPosition().x,anchor.getPosition().y,50);
    ofPopStyle();
    
    
    ofSetHexColor(0xf2ab01);


    if(APPC->debug){
        ofPushStyle();
        
        mover1.draw();
        
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

        ofVec2f leftHand;
        if(mskel.size()>0){
            leftHand=mskel[0].leftHand;
        }
        anchor.draw();
        anchor2.draw();
        ofDrawEllipse(leftHand,20,20);
        
            for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
             circles[i].get()->draw();
        }
        
        for(int i=0; i<joints.size(); i++) {
            ofSetHexColor(0x444342);
              joints[i].get()->draw();
        }
        ofPopStyle();

    }
    
   // line=line.getResampledBySpacing(20);
  // line=line.getSmoothed(10);
    


    
    
    string info = "";
    info += "Press [n] to add a new joint\n";
    info += "click and pull the chain around\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
}

void LinieApp::exit(){
    cout<<"exit LinieApp"<<endl;
}

void LinieApp::makeCircle(bool _b){
    bMakeCircle=_b;
}



//KEY LISTENER
//--------------------------------------------------------------
void LinieApp::keyPressed(ofKeyEventArgs &e){
   if(e.key=='R'){
        anchor.setPhysics(1, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
        
    }
    
    
    if(e.key=='r'){
        anchor2.setPhysics(1, 1, 10);
        anchor2.body->SetType(b2_dynamicBody);
        
    }
    
    if(e.key=='l'){
        bUseHand=!bUseHand;
        cout<<bUseHand<<endl;
    }
    
    if(e.key=='h'){
        anchor.setPosition(0, ofGetHeight()/2);
        anchor.setPhysics(0, 0.5, 0.9);
        anchor.body->SetType(b2_staticBody);

        anchor2.setPosition(ofGetWidth(), ofGetHeight()/2);
        anchor2.setPhysics(0, 0.5, 0.9);
        anchor2.body->SetType(b2_staticBody);
        
    }
    
    
    if(e.key=='a'){
        anchor2.setPosition(ofGetMouseX(), ofGetMouseY());
    }
    
    
    if(e.key=='s'){
        anchor.setPosition(ofGetMouseX(), ofGetMouseY());
    }
    
    if(e.key=='w'){
        repulse=!repulse;
    }
    
    
    if(e.key=='-'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(joints[i].get()->getFrequency()-0.1);
        }
        cout<<joints[0].get()->getFrequency()<<endl;
    }
    
    if(e.key=='+'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(joints[i].get()->getFrequency()+0.1);
        }
    }
    
   /* if(e.key=='d'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping( joints[i].get()->getDamping()+0.01);
        }
        cout<<joints[0].get()->getDamping()<<endl;
    }
    if(e.key=='D'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping( joints[i].get()->getDamping()-0.01);
        }
        cout<<joints[0].get()->getDamping()<<endl;
    }
    */
    
    if(e.key=='m'){
        toggleMouseActive();
    }
    
    if(e.key=='u'){
        anchorStartPositionTop->set(ofGetMouseX(),ofGetMouseY());
        anchor.setPosition(ofGetMouseX(), ofGetMouseY());

        Settings::get().save("data.json");
    }
    
    
    
    
    
    
    if(e.key=='i'){
        anchorStartPositionBottom->set(ofGetMouseX(),ofGetMouseY());
        anchor2.setPosition(ofGetMouseX(), ofGetMouseY());

        Settings::get().save("data.json");
    }
    
    
    if(e.key=='C'){
        finalCirclePosition->set(ofGetMouseX(),ofGetMouseY());
        
        Settings::get().save("data.json");
        moverCircleCenter.set(finalCirclePosition->x,finalCirclePosition->y);

    }
    
    
    if(e.key=='9'){
        startWave(10,500,PI);
      /*  bMakeWave=!bMakeWave;
        if(bMakeWave){
            waveInittime=ofGetElapsedTimef();
            waveInitPosition=anchor.getPosition();
        }else{
            anchor.setPosition(waveInitPosition);
        }
       */
    }
    
    if(e.key=='8'){
        startWave(20,200,PI);
       
    }
    if(e.key=='0'){
        startWave(5,200,6*PI);
    }
    
}


void LinieApp::startWave(float _speed,float _amplitude, float _howmany){
    waveInittime=ofGetElapsedTimef();
    if(!bMakeWave)waveInitPosition=anchor.getPosition(); // damit die ursprungsposition auch bei mehreren wellen bleibt
    bMakeWave=true;
    waveSpeed=_speed;
    waveAmplitude=_amplitude;
    howmany=_howmany;
    bHasStartWave=true;
}
void LinieApp::wave(){
    if((ofGetElapsedTimef()-waveInittime)*waveSpeed<howmany){
        float r=0;
        float v=0;
        r=waveAmplitude*sin((ofGetElapsedTimef()-waveInittime)*waveSpeed);
    
        v=2.f*cos(ofGetElapsedTimef()*2);
        ofVec2f ap=waveInitPosition;
        ofVec2f d=anchor2.getPosition()-ap;
        d.normalize();
        d.rotate(-90);
        d*=r;
        anchor.setPosition(ap+d);
    }else{
        endWave();
    }
}
void LinieApp::endWave(){
    bMakeWave=false;
    anchor.setPosition(waveInitPosition);
}

float LinieApp::maxWaveVol(float v){
    if((ofGetElapsedTimef()-waveInittime)<waveMaxVolTime){
        if(v>maxVol)maxVol=v;
    }
    return maxVol;
}

//--------------------------------------------------------------
void LinieApp::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void LinieApp::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LinieApp::mousePressed(ofMouseEventArgs &a){
  //  cout<<"Mouse"<< anchor.isFixed()<<endl;
    //anchor.setPhysics(0, 0.5, 0.9);
    //anchor.body->SetType(b2_staticBody);
    //anchor.setup(box2d.getWorld(), a.x, 0, 10);
    

}

//--------------------------------------------------------------
void LinieApp::mouseReleased(ofMouseEventArgs &a){
    
}

void LinieApp::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LinieApp::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void LinieApp::mouseExited(ofMouseEventArgs &a){
    
}


void LinieApp::onMessageReceived(ofxOscMessage &msg){
    if(msg.getAddress() == "/4/push10")
    {
        cout<<OSCwaveSpeed<<" "<<OSCwaveAmplitude<<" "<<OSChowmany<<endl;
        startWave(OSCwaveSpeed, OSCwaveAmplitude, OSChowmany);
    }
    
    if(msg.getAddress() == "/4/rotary2")
    {
        OSCwaveSpeed=msg.getArgAsFloat(0);
        ofxOscMessage m;
        m.addFloatArg(OSCwaveSpeed);
        m.setAddress("/4/label8");
        APPC->oscmanager.touchOscSender.sendMessage(m);

    }
    if(msg.getAddress() == "/4/rotary3")
    {
        OSCwaveAmplitude=msg.getArgAsFloat(0);
        ofxOscMessage m;
        m.addFloatArg(OSCwaveAmplitude);
        m.setAddress("/4/label9");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    if(msg.getAddress() == "/4/rotary4")
    {
        OSChowmany=msg.getArgAsFloat(0);
        ofxOscMessage m;
        m.addFloatArg(OSChowmany);
        m.setAddress("/4/label10");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        
        
        
        
    }
    
    if(msg.getAddress() == "/4/rotary5")
    {
        
        float freq=msg.getArgAsFloat(0);
        
        freq=ofMap(freq, 0, 1, 0.2, 20);

        
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(freq);
        }
        ofxOscMessage m;
        m.addFloatArg(freq);
        m.setAddress("/4/label11");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    
    if(msg.getAddress() == "/4/rotary6")
    {
       /* float dmp=msg.getArgAsFloat(0);
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping(dmp);
        }
        ofxOscMessage m;
        m.addFloatArg(dmp);
        m.setAddress("/4/label12");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        */
        float dmp=msg.getArgAsFloat(0);
        moverCircleSpeed=ofMap(dmp, 0, 1, 0, 5);
    }
    
    if(msg.getAddress() == "/4/rotary7")
    {
        float len=msg.getArgAsFloat(0);
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setLength(len);
        }
        ofxOscMessage m;
        m.addFloatArg(len);
        m.setAddress("/4/label13");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    
    
    if(msg.getAddress() == "/4/rotary8")
    {
       /* float damp=msg.getArgAsFloat(0);
        generalDamping=damp;
        ofxOscMessage m;
        m.addFloatArg(damp);
        m.setAddress("/4/label14");
        APPC->oscmanager.touchOscSender.sendMessage(m);*/
        
        float den=msg.getArgAsFloat(0);
        den=ofMap(den, 0, 1, 0, 10);
        for(int i=0; i<circles.size(); i++) {
            circles[i].get()->setPhysics(den, 0.5, 0.5);
        }
        
    }
    
    if(msg.getAddress() == "/4/rotary9")
    {
        /* float damp=msg.getArgAsFloat(0);
         generalDamping=damp;
         ofxOscMessage m;
         m.addFloatArg(damp);
         m.setAddress("/4/label14");
         APPC->oscmanager.touchOscSender.sendMessage(m);*/
        
        float damp=msg.getArgAsFloat(0);
        damp=ofMap(damp, 0, 1, 0.8, 1);
        damping=damp;
        ofxOscMessage m;
        m.addFloatArg(damp);
        m.setAddress("/4/label59");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        
    }
    
    
    
    if(msg.getAddress() == "/4/push11")
    {
        startWave(10,500,PI);

    }
    
    if(msg.getAddress() == "/4/push12")
    {
        cout<<"start wave"<<endl;
        startWave(10,500,PI);

    }
    
    if(msg.getAddress() == "/4/push13")
    {
        startWave(10,500,PI);

    }
    
    
    if(msg.getAddress() == "/4/push35")
    {
        bMakeCircle=!bMakeCircle;
        
        anchor.setPhysics(1, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
        
    }
    
    if(msg.getAddress() == "/4/push36")
    {
       // anchor.setup(box2d.getWorld(), anchorStartPositionTop->x, anchorStartPositionTop->y, 10);
       // anchor2.setup(box2d.getWorld(), anchorStartPositionBottom->x, anchorStartPositionBottom->y, 10);
        
        anchor.setPosition( anchorStartPositionTop->x, anchorStartPositionTop->y);
        anchor.setPhysics(0, 0.5, 0.9);
        anchor.body->SetType(b2_staticBody);
        
        anchor2.setPosition(anchorStartPositionBottom->x, anchorStartPositionBottom->y);
        anchor2.setPhysics(0, 0.5, 0.9);
        anchor2.body->SetType(b2_staticBody);
        
        circleBoundindex=0;
        
        bMakeCircle=false;
        
        
    }
    if(msg.getAddress() == "/4/push37")
    {
        //circleBoundindex++;
        circleBoundindex+=5;

    }

    // Release and make Circle
    if(msg.getAddress() == "/4/push38")
    {
        bMakeCircle=!bMakeCircle;

        anchor.setPhysics(1, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
        
        anchor2.setPhysics(1, 0.5, 0.9);
        anchor2.body->SetType(b2_dynamicBody);
        
        circleBoundindex+=20;

        
        float freq=12;
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(freq);
        }
        ofxOscMessage m;
        m.addFloatArg(freq);
        m.setAddress("/4/label11");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        
        
       float mfreq=ofMap(freq, 0.2, 20, 0, 1);
        ofxOscMessage mr;
        mr.addFloatArg(mfreq);
        mr.setAddress("/4/rotary5");
        APPC->oscmanager.touchOscSender.sendMessage(mr);
        
        
        float len=20;
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setLength(len);
        }
        ofxOscMessage m2;
        m2.addFloatArg(len);
        m2.setAddress("/4/label13");
        APPC->oscmanager.touchOscSender.sendMessage(m2);
        
        ofxOscMessage m2r;
        m2r.addFloatArg(len);
        m2r.setAddress("/4/rotary7");
        APPC->oscmanager.touchOscSender.sendMessage(m2r);
        
        
    }
    
    if(msg.getAddress() == "/4/push39")
    {
        anchor2.setPhysics(1, 0.5, 0.9);
        anchor2.body->SetType(b2_dynamicBody);
        
    }
    
    
    if(msg.getAddress() == "/4/rotary11")
    {
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, 0, 1, 0, 50);
        circleAttractionForce=f;
        
        ofxOscMessage m;
        m.addFloatArg(f);
        m.setAddress("/4/label64");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        
    }
    
    
}


void LinieApp::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void LinieApp::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void LinieApp::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
