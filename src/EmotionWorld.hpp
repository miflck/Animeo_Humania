//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef EmotionWorld_hpp
#define EmotionWorld_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "ofxBox2d.h"
#include "MovingObject.hpp"
#include "Sun.hpp"
#include "Flash.hpp"

#include "Baloon.hpp"
#include "Kreis.hpp"
#include "Dreieck.hpp"
#include "Stern.hpp"



#include "ofxOsc.h"

#include "ofxEasing.h"


#include "ofxJsonSettings.h"

class SoundData {
public:
    int     soundID;
    bool bHit;
};


// ------------------------------------------------- a simple extended box2d circle
class Heart : public ofxBox2dCircle {
    
public:
    ofImage herz;
    ofColor col;
    ofRectangle screen;
    
    float actualRadius;
    float easingInitTime;
    float radiusTarget;
    float scaleDuration=5.f;

    Heart() {
        herz.loadImage("herz.png");
        col=ofColor(220+ofRandom(-30,30),37+ofRandom(-30,30),151+ofRandom(-30,30));
        screen.set(0,0,ofGetWidth(),ofGetHeight());
        actualRadius=0;
        easingInitTime = ofGetElapsedTimef();
        radiusTarget=ofRandom(10,40);
    }
    ofColor color;
    
   static bool shouldRemoveOffScreen(shared_ptr<Heart> shape) {
        return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
    }
    
    void update(){
        
        auto endTime = easingInitTime + scaleDuration;
        auto now = ofGetElapsedTimef();
        actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
        if(actualRadius!=radiusTarget)setRadius(actualRadius);
    }
    
    
    void draw() {
        float radius = getRadius();
        ofPushMatrix();
         ofTranslate(getPosition().x,getPosition().y);
         ofRotate(getRotation());
        ofSetColor(col);
        ofFill();
        herz.draw(-getRadius(),-getRadius(),getRadius()*2,getRadius()*2);
        ofPopMatrix();
    }
};



// ------------------------------------------------- a simple extended box2d circle
class Anchor : public ofxBox2dCircle {
    
public:
    ofImage anchor;
    ofColor col;
    ofRectangle screen;
    
    float actualRadius;
    float easingInitTime;
    float radiusTarget;
    float scaleDuration=8;
    
    Anchor() {
        anchor.loadImage("anker.png");
        col=ofColor(255);
        screen.set(0,0,ofGetWidth(),ofGetHeight());
        actualRadius=20;
        easingInitTime = ofGetElapsedTimef();
        radiusTarget=100;
    }
    ofColor color;
    
    static bool shouldRemoveOffScreen(shared_ptr<Heart> shape) {
        return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
    }
    
    void update(){
        
        auto endTime = easingInitTime + scaleDuration;
        auto now = ofGetElapsedTimef();
        actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
        if(actualRadius!=radiusTarget)setRadius(actualRadius);
    }
    
    
    void draw() {

       // ofxBox2dCircle::draw();
        float radius = getRadius();
        ofPushMatrix();
        ofTranslate(getPosition().x,getPosition().y);
        ofRotate(getRotation());
        ofSetColor(col);
        ofFill();
        anchor.draw(-getRadius(),-getRadius(),getRadius()*2,getRadius()*2);
        ofPopMatrix();
    }
};



// ------------------------------------------------- a simple extended box2d circle
class Shape : public ofxBox2dCircle {
    
public:
    ofColor col;
    ofRectangle screen;
    float actualRadius;
    float easingInitTime;
    float radiusTarget;
    float scaleDuration=2.f;

    
    Shape() {
        col=ofColor(220+ofRandom(-30,30),37+ofRandom(-30,30),151+ofRandom(-30,30));
        screen.set(0,0,ofGetWidth(),ofGetHeight());
        actualRadius=0;
        easingInitTime = ofGetElapsedTimef();
        radiusTarget=ofRandom(5,30);
    }
    ofColor color;
    
    static bool shouldRemoveOffScreen(shared_ptr<Shape> shape) {
        return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
    }
    

    void update(){
        auto endTime = easingInitTime + scaleDuration;
        auto now = ofGetElapsedTimef();
          actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
            if(actualRadius!=radiusTarget)setRadius(actualRadius);
    }
    
    void draw() {
        float radius = getRadius();
        ofPushMatrix();
        ofTranslate(getPosition().x,getPosition().y);
        ofRotate(getRotation());
        ofSetColor(col);
        ofFill();
        ofDrawCircle(0,0,radius);
        ofPopMatrix();
    }
};







// ------------------------------------------------- a simple extended box2d circle
class Triangle : public ofxBox2dPolygon {
    
public:
    ofColor col;
    ofRectangle screen;
    float actualRadius=0.1;
    float easingInitTime;
    float radiusTarget=1;
    float scaleDuration=2.f;
    ofVec2f a,b,c;
    ofVec2f center;
    b2World* world;
    
    Triangle(ofVec2f _a,ofVec2f _b,ofVec2f _c) {
        ofxBox2dPolygon::ofxBox2dPolygon();

        //col=ofColor(220+ofRandom(-30,30),37+ofRandom(-30,30),151+ofRandom(-30,30));
        col=ofColor(255);

        screen.set(0,0,ofGetWidth(),ofGetHeight());
        actualRadius=0.1;
        easingInitTime = ofGetElapsedTimef();
        //radiusTarget=ofRandom(5,30);
        
        a=_a;
        b=_b;
        c=_c;
     
        
        center= getTriangleCenter(a, b, c);

        addTriangle(a,b,c);
        
       
        
        //body->SetLinearVelocity()
        //setVelocity();
      // ofxBox2dPolygon:: setVelocity(ofRandom(50,100), ofRandom(-5,0));

    }
    ofColor color;
    
    void setWorld(b2World* w){
        world=w;
    }
    
    static bool shouldRemoveOffScreen(shared_ptr<Triangle> shape) {
        return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
    }
    
    
    void update(){
        
  
        
        /*clear();
        addTriangle(a*actualRadius,b*actualRadius,c*actualRadius);
        create(world);*/
        
        
        auto endTime = easingInitTime + scaleDuration;
        auto now = ofGetElapsedTimef();
        actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
      //  if(actualRadius!=radiusTarget)setRadius(actualRadius);
     
        
    }
    
    void draw() {
      //  float radius = getRadius();
     //  ofxBox2dPolygon::draw();
        ofPushStyle();
        ofPoint ct = getCentroid2D();
        ofPushMatrix();
        ofTranslate(getPosition().x,getPosition().y);
        ofRotate(getRotation());
        ofScale(actualRadius,actualRadius);
        ofTranslate(-ct.x,-ct.y);
        ofSetColor(col);
        ofFill();
        ofDrawTriangle(a, b,c);
    
        ofPopMatrix();
        ofNoFill();
        //ofSetColor(255);

        ofPopStyle();
    }
};



// ------------------------------------------------- a simple extended box2d circle
class AnchorTriangle : public ofxBox2dPolygon {
    
public:
    ofImage anchor;

    ofColor col;
    ofRectangle screen;
    float actualRadius=0.1;
    float easingInitTime;
    float radiusTarget=1;
    float scaleDuration=2.f;
    ofVec2f a,b,c;
    ofVec2f center;
    b2World* world;
    
    AnchorTriangle(ofVec2f _a,ofVec2f _b,ofVec2f _c) {
        ofxBox2dPolygon::ofxBox2dPolygon();
        anchor.loadImage("anker.png");

        
        //col=ofColor(220+ofRandom(-30,30),37+ofRandom(-30,30),151+ofRandom(-30,30));
        col=ofColor(255);
        
        screen.set(0,0,ofGetWidth(),ofGetHeight());
        actualRadius=0.2;
        easingInitTime = ofGetElapsedTimef();
        //radiusTarget=ofRandom(5,30);
        
        a=_a;
        b=_b;
        c=_c;
        
        
        center= getTriangleCenter(a, b, c);
        
        addTriangle(a,b,c);
        
       
        
    }
    ofColor color;
    
    void setWorld(b2World* w){
        world=w;
    }
    
    static bool shouldRemoveOffScreen(shared_ptr<AnchorTriangle> shape) {
        return !ofRectangle(0, 0, shape.get()->screen.getWidth(), shape.get()->screen.getHeight()).inside(shape.get()->getPosition());
    }
    
    
    void update(){
        
        
        
        /*clear();
         addTriangle(a*actualRadius,b*actualRadius,c*actualRadius);
         create(world);*/
        
        
        auto endTime = easingInitTime + scaleDuration;
        auto now = ofGetElapsedTimef();
        actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
        //  if(actualRadius!=radiusTarget)setRadius(actualRadius);
        
        
    }
    
    void draw() {
        //  float radius = getRadius();
         // ofxBox2dPolygon::draw();
        ofPushStyle();
        ofSetColor(col);
        
       // SoundData * data = (SoundData*) getData();
       // if(data && data->bHit) ofSetHexColor(0xff0000);
       // else
       //     ofSetColor(255);

        ofPoint ct = getCentroid2D();
        ofPushMatrix();
        ofTranslate(getPosition().x,getPosition().y);
        ofRotate(getRotation());
        ofScale(actualRadius,actualRadius);
        anchor.draw(-anchor.getWidth()/2,-anchor.getHeight()/2-20);
        ofTranslate(-ct.x,-ct.y);
        ofFill();
       // ofDrawTriangle(a, b,c);
        // ofSetColor(255,0,0);
        //  ofDrawCircle(center,5);
        // ofDrawCircle(b,5);
        
        //  ofDrawCircle(0,0,radius);
        ofPopMatrix();
        ofNoFill();
        //ofSetColor(255);
        
        ofPopStyle();
    }
};



class EmotionWorld:public ApplicationBase{
public:
   EmotionWorld();
    virtual ~EmotionWorld();
    
    

    
    void turnOn();
    void turnOff();
    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();
    void keyPressed(ofKeyEventArgs &e);
    
    void mouseMoved(ofMouseEventArgs &a );

     void mouseDragged(ofMouseEventArgs &a);
     void mousePressed(ofMouseEventArgs &a);
     void mouseReleased(ofMouseEventArgs &a);
     void mouseScrolled(ofMouseEventArgs &a);
     void mouseEntered(ofMouseEventArgs &a);
     void mouseExited(ofMouseEventArgs &a);

    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    
    void toggleMouseActive();
    ofFbo screen;
    
    
    Sun sun;
    
    
    Baloon baloon;
    bool bShowBaloon=false;
    void showBaloon(bool _s);

    
    void toggleHearts();
    void toggleSun();
    void showSun(bool _s);

    ofImage herz;
    
    
    Kreis kreis;
    
    vector      <shared_ptr<Kreis> > kreise; // default box2d circles
    vector      <shared_ptr<Stern> > sterne; // default box2d circles
    vector      <shared_ptr<Dreieck> > dreiecke; // default box2d circles




private:
    ofxBox2d                                  box2d;   // the box2d world
    ofxBox2dCircle                            anchor;  // fixed anchor
    vector      <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles
    vector      <shared_ptr<ofxBox2dJoint> >  joints;  // joints
    vector    <shared_ptr<ofxBox2dRect> >   boxes;           // default box2d rects
    
    vector    <shared_ptr<Heart> > hearts; // this is a custom particle the extends a cirlce
    vector    <shared_ptr<Shape> > shapes; // this is a custom particle the extends a cirlce
    vector      <shared_ptr<Triangle> > triangles;

    vector      <shared_ptr<AnchorTriangle> > anchors;
    


    
    
    ofxBox2dRect box;
    ofxBox2dRect leftbox;
    ofxBox2dRect rightbox;

    ofxBox2dCircle                            anchor2;  // fixed anchor
    vector      <ofVec2f>   positions;  // joints

    ofPolyline line;
    
    ofxBox2dJoint mousejoint;
    bool bIsMouseActive=false;
  
   vector<MovingObject> movingObjects;
    
    vector      <shared_ptr<Flash> > flashes;

    
    //vector<shared_ptr<Agent> > movingObjects;

    
    
    bool bEmitShapes=false;
    float emitShapeFrequency=0.7;
    
    void emitShapes();
    
    void emitMultiShapes(int n);

    
    
    bool bEmitHearts=false;
    float emitFrequency=0.7;
    
    
    bool bShowSun=false;
    void onMessageReceived(ofxOscMessage &msg);
    
    
    bool bShowFeeling=true;
    int feelingIndex=1;
    void drawFeeling();
    
    
    ofVec2f headposition;
    
    float gravityX;
    float gravityY;
    
    bool bCircleFollowMouse=false;
  
    
    
    
    // Settings
    ofVec2f *savedemitterposition;
    void saveEmitterposition(ofVec2f _p);
    ofVec2f emitterposition;
    bool bBindToHead=false;
    
    void bindToSkeletton(bool _b);
    
    
    
    
    // Sound
    vector<ofSoundPlayer>ploppsounds;
    void playRandomPlopp();
    vector<ofSoundPlayer>multiploppsounds;
    void playRandomMultiplopp();

    ofSoundPlayer anchorSound;

    // this is the function for contacts
    void contactStart(ofxBox2dContactArgs &e);
    void contactEnd(ofxBox2dContactArgs &e);
    
    
};


#endif /* Physics_hpp */
