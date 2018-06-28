//
//  EmotionWorld.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "EmotionWorld.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


EmotionWorld::EmotionWorld(){
    init();
}


EmotionWorld::~EmotionWorld(){
    
}

void EmotionWorld::init(){
    cout<<"init EmotionWorld"<<endl;
    bAddedListeners = false;
    
    gravityY=0;
    gravityX=0;

    
    box2d.init();
    box2d.setGravity(0, gravityY);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    anchor.setPhysics(50, 0.5, 0.9);
    anchor.setup(box2d.getWorld(), 0, 0, 50);
  
  /*  box.setup(box2d.getWorld(), ofGetWidth()/2, -20, ofGetWidth(), 20);
    leftbox.setup(box2d.getWorld(), 0, 150,20,300);
    rightbox.setup(box2d.getWorld(), ofGetWidth(),150, 20,300);
*/
    
    screen.allocate(1920,1080, GL_RGB);
    screen.begin();
    ofClear(0,0,0,0);
    screen.end();
    
    herz.loadImage("herz.png");
    
    sun.setup();
    sun.bSeekTarget=true;
    
    ofAddListener(APPC->oscmanager.onMessageReceived, this, &EmotionWorld::onMessageReceived);

    
}

void EmotionWorld::update(){
    //box2d.setGravity(0, APPC->gui->emotionsgravity);
    
    for(int i=0;i<shapes.size();i++){
        shapes[i]->update();
    }
    
    for(int i=0;i<triangles.size();i++){
        triangles[i]->update();
    }
    
    for(int i=0;i<hearts.size();i++){
        hearts[i]->update();
    }
    
    box2d.update();
    
    headposition=ofVec2f(ofGetMouseX(),ofGetMouseY());
    ofVec2f hand;
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()<=0){
    sun.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    }else{
        hand=mskel[0].leftHand;
        hand=ofVec2f(hand.x, hand.y);
        sun.setTarget(hand);

    }
    sun.update();
    
    for(int i=0;i<movingObjects.size();i++){
        movingObjects[i].setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        movingObjects[i].update();
    }
    
  for(int i=0;i<flashes.size();i++){
        flashes[i]->update();
    }
    
    if(mskel.size()>0){
        headposition=mskel[0].head;
        headposition=ofVec2f(headposition.x, headposition.y);
        anchor.setPosition(mskel[0].leftHand.x, mskel[0].leftHand.y);
    }
       // box2d.setGravity(0, (mskel[0].head.y-mskel[0].rightHand.y)/10);

    

    
    
        float rAdd=ofRandom(1);
        if(rAdd>emitFrequency && bEmitHearts){
           float r = ofRandom(10, 40);        // a random radius 4px - 20px
            hearts.push_back(shared_ptr<Heart>(new Heart));
            hearts.back().get()->setPhysics(3.0, 0.53, 0.5);
            hearts.back().get()->setup(box2d.getWorld(), headposition.x, headposition.y, 0);
            hearts.back().get()->setVelocity(ofRandom(-10,10), ofRandom(0,-10));
            hearts.back().get()->setAngularVelocity(ofRandom(1));
        }
    
    
    
     rAdd=ofRandom(1);
    if(rAdd>emitShapeFrequency && bEmitShapes){
        float r =0;        // a random radius 4px - 20px
        shapes.push_back(shared_ptr<Shape>(new Shape));
        shapes.back().get()->setPhysics(3.0, 0.2, 0.01);
        shapes.back().get()->setup(box2d.getWorld(), headposition.x, headposition.y,r);
        shapes.back().get()->setVelocity(ofRandom(20,30), ofRandom(-10,-30));
    }
   // cout<<shapes.size()<<endl;
    
    // remove shapes offscreen
    //ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    //ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(shapes, Shape::shouldRemoveOffScreen);
    ofRemove(hearts, Heart::shouldRemoveOffScreen);
    ofRemove(flashes, MovingObject::shouldRemoveOffScreen);

  
}


void EmotionWorld::draw(){

    if(APPC->debug){
        ofPushStyle();
        for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
            circles[i].get()->draw();
        }
        ofSetColor(255,0,0);
        anchor.draw();
        leftbox.draw();
        rightbox.draw();
        ofPopStyle();
    }
    
    ofPushStyle();
   if(bShowSun)sun.draw();
    
    for(int i=0; i<circles.size(); i++) {
      //  ofSetColor(220+ofRandom(-20,20),37+ofRandom(-20,20),151+ofRandom(-20,20));
        circles[i]->draw();
        ofPushMatrix();
       // ofTranslate(circles[i]->getPosition().x,circles[i]->getPosition().y);
       // ofRotate(circles[i]->getRotation());
        //herz.draw(-circles[i]->getRadius(),-circles[i]->getRadius(),circles[i]->getRadius()*2,circles[i]->getRadius()*2);
        ofPopMatrix();
    }
    
    for(int i=0; i<hearts.size(); i++) {
        hearts[i]->draw();
    }
    
    for(int i=0; i<shapes.size(); i++) {
        shapes[i]->draw();
    }
    
    for(int i=0;i<triangles.size();i++){
        triangles[i]->draw();
    }
    
    
    
    ofPopStyle();
    
    for(int i=0; i<circles.size(); i++) {
    }

 /*   for(int i=0;i<movingObjects.size();i++){
        movingObjects[i].draw();
    }*/
    
   if(bShowFeeling)drawFeeling();
    
   /*   screen.begin();

  ofPushStyle();
    ofSetColor(255,100);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 10);
    screen.end();
   
    screen.draw(0,0);
   */
    
}

void EmotionWorld::exit(){
    cout<<"exit EmotionWorld"<<endl;
}


void EmotionWorld::toggleHearts(){
    bEmitHearts=!bEmitHearts;
}
void EmotionWorld::toggleSun(){
    bShowSun=!bShowSun;
}

void EmotionWorld::showSun(bool _s){
    bShowSun=_s;
}



void EmotionWorld::drawFeeling(){
    switch (feelingIndex) {
        case 1:
            ofPushStyle();
            ofPushMatrix();
            for(int i=0;i<flashes.size();i++){
                flashes[i]->draw();
            }
            ofPopMatrix();
            ofPopStyle();
            break;
            
        default:
            break;
    }
}



//KEY LISTENER
//--------------------------------------------------------------
void EmotionWorld::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'j') {
        
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
        ofVec2f head=ofVec2f(ofGetMouseX(), ofGetMouseY());
        if(mskel.size()>0){
            head=mskel[0].head;
            head=ofVec2f(head.x, head.y);
        }
        float r = ofRandom(10, 40);        // a random radius 4px - 20px

      /*  cout<<"Add Circles"<<endl;
        float r = ofRandom(10, 40);        // a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));

        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        circles.back().get()->setVelocity(ofRandom(-5,5), ofRandom(-1,-5));
        */

        r=ofRandom(20,80);
        ofVec2f a =ofVec2f(ofGetMouseX()-r/2,ofGetMouseY());
        ofVec2f b =ofVec2f(ofGetMouseX()+r/2,ofGetMouseY());
        ofVec2f c =ofVec2f(ofGetMouseX(),ofGetMouseY()+r);
        
        triangles.push_back(shared_ptr<Triangle>(new Triangle(a,b,c)));
        triangles.back().get()->setWorld(box2d.getWorld());

        triangles.back().get()->setPhysics(3.0, 0.3, 0.3);
        triangles.back().get()->create(box2d.getWorld());
        triangles.back().get()->setVelocity(ofVec2f(10,5));
        triangles.back().get()->setAngularVelocity(2);



        
        
        

    }
    
    
    if(e.key == 'o') {
        MovingObject m;
        m.bSeekTarget=true;
        m.setRadius(10);
        m.setPosition(ofGetWidth()/2, 0);
        m.setTarget(ofVec2f(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight())));
        movingObjects.push_back(m);
    }
    
    if(e.key == 'p') {
 
        for(int i=0;i<movingObjects.size();i++){
            ofVec2f r=ofVec2f(ofRandom(50),ofRandom(50));
            r.rotate(ofRandom(360));
            movingObjects[i].applyForce(r);
        }
    }
    
    if(e.key=='h'){
        toggleHearts();
    }
    
    
    if(e.key=='s'){
       // toggleSun();
        
        bEmitShapes=!bEmitShapes;
    }
    
    if(e.key=='q'){
        sun.scaleTo(100,20.f);
    }
    
    if(e.key=='w'){
        sun.scaleTo(500,20.f);
    }
    
    if(e.key=='e'){
        sun.scaleTo(2000,20.f);
    }
    
    if(e.key=='g'){

        for(int i=0;i<10;i++){
            ofVec2f r=ofVec2f(-1,0);
            r.rotate(180/10*(i+1));
            r*=ofGetWidth()*3;
            cout<<"Add Flashes"<<endl;
            flashes.push_back(shared_ptr<Flash>(new Flash));
            flashes.back().get()->setup();
            flashes.back().get()->setPosition(headposition.x, headposition.y);
            flashes.back().get()->setTarget(r);
           flashes.back().get()->bSeekTarget=true;
        }

   // circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
   // circles.back().get()->setVelocity(ofRandom(-5,5), ofRandom(-1,-5));
    }
    
}


//--------------------------------------------------------------
void EmotionWorld::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void EmotionWorld::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void EmotionWorld::mousePressed(ofMouseEventArgs &a){
   

}

//--------------------------------------------------------------
void EmotionWorld::mouseReleased(ofMouseEventArgs &a){
    
}

void EmotionWorld::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void EmotionWorld::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void EmotionWorld::mouseExited(ofMouseEventArgs &a){
    
}


void EmotionWorld::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void EmotionWorld::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void EmotionWorld::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}



void EmotionWorld::onMessageReceived(ofxOscMessage &msg){
    
    if(msg.getAddress() == "/EmotionWorld/toggle1")
    {
        float f=msg.getArgAsBool(0);
        showSun(f);
        cout<<f<<endl;


    }
    
    
    if(msg.getAddress() == "/EmotionWorld/toggle3")
    {
        float m=msg.getArgAsBool(0);
        bEmitHearts=m;
        cout<<"Hearts"<<bEmitHearts<<endl;
    }
    
    if(msg.getAddress() == "/EmotionWorld/fader3")
    {
        float f=msg.getArgAsFloat(0);
        emitFrequency=f;
    }

    if(msg.getAddress() == "/EmotionWorld/fader2")
    {
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, -1.f, 1.f, -10, 10);
        gravityY=f;
        box2d.setGravity(gravityX, gravityY);
    }
    
    if(msg.getAddress() == "/EmotionWorld/fader4")
    {
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, -1.f, 1.f, -10, 10);
        gravityX=f;
        box2d.setGravity(gravityX, gravityY);
    }
    
    if(msg.getAddress() == "/EmotionWorld/push15")
    {
        gravityX=0;
        box2d.setGravity(gravityX, gravityY);
        ofxOscMessage m;
        m.addFloatArg(0);
        m.setAddress("/EmotionWorld/fader4");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    if(msg.getAddress() == "/EmotionWorld/push16")
    {
        gravityY=0;
        box2d.setGravity(gravityX, gravityY);
        
        ofxOscMessage m;
        m.addFloatArg(0);
        m.setAddress("/EmotionWorld/fader2");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }

    
    
    if(msg.getAddress() == "/EmotionWorld/push1")
    {
        float f=msg.getArgAsBool(0);
        float r = ofRandom(10, 40);        // a random radius 4px - 20px
        hearts.push_back(shared_ptr<Heart>(new Heart));
        hearts.back().get()->setPhysics(3.0, 0.53, 0.1);
        hearts.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        hearts.back().get()->setVelocity(ofRandom(-10,10), ofRandom(0,-10));
    }
    
    if(msg.getAddress() == "/EmotionWorld/push5")
    {
        float r = ofRandom(10, 40);        // a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(ofRandom(5.0,1), ofRandom(0,1), ofRandom(0,1));
        circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        circles.back().get()->setVelocity(ofRandom(-5,5), ofRandom(-1,-5));
    }
    
    if(msg.getAddress() == "/EmotionWorld/toggle4")
    {
        float m=msg.getArgAsBool(0);
        if(m){
            box.setup(box2d.getWorld(), ofGetWidth()/2, -20, ofGetWidth(), 20);
            leftbox.setup(box2d.getWorld(), 0, 150,20,300);
            rightbox.setup(box2d.getWorld(), ofGetWidth(),150, 20,300);
            
        }else{
            box.destroy();
            leftbox.destroy();
            rightbox.destroy();
        }
    }
    
    
    

    
}



