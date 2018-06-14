//
//  KinectV2.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "KinectV2Manager.hpp"
#include "ApplicationController.h"



KinectV2Manager* KinectV2Manager::instance = 0;

KinectV2Manager* KinectV2Manager::getInstance() {
    if (!instance) {
        instance = new KinectV2Manager();
    }
    return instance;
}

KinectV2Manager::KinectV2Manager() {
    
}
void KinectV2Manager::initialize() {
        initialized=true;
        cout<<"init KinectV2Manager"<<endl;
    bAddedListeners = false;
    bAddedInputListeners =false;
    
    turnOn();
    
    smallFont.loadFont("selena.otf", 16); //http://openfontlibrary.org/en/font/selena
    largeFont.loadFont("selena.otf", 48);
    
    //The Kinect here is just an OSC receiver and parser
    //It just needs a port number and font for the debug text
    kinect.setup(12345, smallFont);
    
    //Here we get a pointer to the list of skeletons it has parsed
    //from OSC
    skeletons = kinect.getSkeletons();
    
    //We could inspect the skeletons and draw them here in ofApp
    //but for now let's pass the list to a default renderer class
    renderer.setup(skeletons, largeFont);
    
    
    
    beamerposition.set(0,0,500);
    kinectposition.set(100,100,100);
    
     iMainCamera = 0;
    
    // observer camera
    
    beamerCam.setPosition(beamerposition);
    beamerCam.lookAt(sweetspot);
    beamerCam.setFov(60);
    beamerCam.setFarClip(20000);
    
    camEasyCam.setTarget(ofVec3f(0,0,0));
    camEasyCam.tilt(15);
    camEasyCam.setDistance(1000);
    camEasyCam.disableMouseInput();

    
    kinectCam.setPosition(kinectposition);
    
    cameras[0] = &camEasyCam;
    
    
    // front
    camFront.scale = 300;
    cameras[1] = &camFront;
    
    // top
    camTop.scale =300;
    camTop.tilt(-90);
    cameras[2] = &camTop;
    
    // left
    camLeft.scale = 300;
    camLeft.pan(-90);
    cameras[3] = &camLeft;
    
    cameras[4] = &kinectCam;
    cameras[5] = &beamerCam;
    
    setupViewports();
    
    plane.set(1280, 800);   ///dimensions for width and height in pixels
    plane.setPosition(0, 0, 0); /// position in x y z
    plane.setResolution(10, 10); /// this resolution (as columns and rows) is enough

}


void KinectV2Manager::update(){
    //Each frame check for new Kinect OSC messages
    kinect.update();
    
    beamerposition.set(APPC->gui->beamerPosition);
    beamerposition*=(APPC->gui->kinectscalefact);
    testnode.setPosition(APPC->gui->testPosition);
    beamerCam.setPosition(beamerposition);
    beamerCam.setFov(APPC->gui->beamerFov);
    
    kinectposition.set(APPC->gui->kinectPosition);
    kinectposition*=(APPC->gui->kinectscalefact);
    kinectCam.setPosition(kinectposition);
    

}


void KinectV2Manager::draw(){
    if(APPC->gui->kinectManagerDebug){
//    kinect.drawDebug();
    ofPushStyle();
    // draw main viewport
    cameras[iMainCamera]->begin(viewMain);
    drawScene(iMainCamera);
    cameras[iMainCamera]->end();
    ofPopStyle();

    
    if(draw_sidecameras){
        // draw outlines on views
        ofPushStyle();
        ofSetLineWidth(5);
        ofNoFill();
        ofSetColor(255, 255, 255);
        //
        for(int i = 0; i < N_CAMERAS; i++){
            ofRect(viewGrid[i]);
        }
        //
        //ofRect(viewMain);
        ofPopStyle();
    }
    }
}

void KinectV2Manager::exit(){
}



//KEY LISTENER
//--------------------------------------------------------------
void KinectV2Manager::keyPressed(ofKeyEventArgs &e){
    if(e.key >= '1' && e.key <= '6'){
        iMainCamera = e.key - '1';
    }
}


//--------------------------------------------------------------
void KinectV2Manager::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void KinectV2Manager::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2Manager::mousePressed(ofMouseEventArgs &a){
  

}

//--------------------------------------------------------------
void KinectV2Manager::mouseReleased(ofMouseEventArgs &a){
    
}

void KinectV2Manager::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2Manager::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2Manager::mouseExited(ofMouseEventArgs &a){
    
}


void KinectV2Manager::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void KinectV2Manager::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
    cout<<"turn on KinectManager"<<endl;
}

void KinectV2Manager::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}


//--------------------------------------------------------------
void KinectV2Manager::setupViewports(){
    //call here whenever we resize the window
    
    //--
    // Define viewports
    
    //float xOffset = ofGetWidth() / 3;
    float xOffset = ofGetWidth() / 6;
    
    float yOffset = ofGetHeight() / 6;
    
    viewMain.x = 0;
    viewMain.y = 0;
    viewMain.width = 1280;
    viewMain.height = 800;
    
    for(int i = 0; i < N_CAMERAS; i++){
        viewGrid[i].x = 0;
        viewGrid[i].y = yOffset * i;
        viewGrid[i].width = xOffset;
        viewGrid[i].height = yOffset;
    }
    
    //
    //--

}

//--------------------------------------------------------------
void KinectV2Manager::drawScene(int iCameraDraw){
    ofDrawAxis(100);
    plane.drawWireframe();
    
    hand.draw();
    testnode.draw();
    
    ofVec3f intersection;
    intersection.set(intersectLine(cameras[5]->getGlobalPosition(),testnode.getPosition(),ofVec3f(0,0,1),0)); // we'll get to this later
    ofSetColor(255, 255, 0);
    ofDrawCircle(intersection.x, intersection.y, 20);
    
    
    ofVec3f v1 = cameras[5]->getGlobalPosition();
    ofVec3f v2 = testnode.getGlobalPosition();
    ofSetColor(0, 255, 0);
    ofDrawLine(v1,v2);
    
    v1 = intersection;
    v2 = testnode.getPosition();
    ofSetColor(255, 0, 0);
    ofDrawLine(v1,v2);
    
    
    v1 = cameras[5]->getGlobalPosition();
    v2 = sweetspot.getGlobalPosition();
    ofDrawLine(v1,v2);
    
    
}

ofVec3f KinectV2Manager::   intersectLine(ofVec3f a, ofVec3f b, ofVec3f n,float d) {
    ofVec3f ba = b-a;
    float nDotA = n.dot(a);
    float nDotBA = n.dot(ba);
    return a + (((d - nDotA)/nDotBA) * ba);
}

ofVec3f KinectV2Manager:: kinectToWorld (ofVec3f _pos){
    ofVec3f p;
    ofVec3f kpos;
    kpos.set(_pos);
    kpos*=100;
    kpos*=APPC->gui->kinectscalefact;
    p.set(kinectposition);
    p.x-=kpos.x;
    p.y+=kpos.y;
    p.z-=kpos.z;
    return p;
}


//EVENTS
//------------------------------------------------------

void KinectV2Manager::addListeners() {
    if(!bAddedListeners){
        ofAddListener(ofEvents().update, this, &KinectV2Manager::_update);
        ofAddListener(ofEvents().draw, this, &KinectV2Manager::_draw);
        ofAddListener(ofEvents().exit, this, &KinectV2Manager::_exit);
    }
    bAddedListeners = true;
}


void KinectV2Manager::removeListeners() {
    ofRemoveListener(ofEvents().update, this, &KinectV2Manager::_update);
    ofRemoveListener(ofEvents().draw, this, &KinectV2Manager::_draw);
    ofRemoveListener(ofEvents().exit, this, &KinectV2Manager::_exit);
    bAddedListeners = false;
}



void KinectV2Manager::addInputListeners() {
    if(!bAddedInputListeners){
        ofAddListener(ofEvents().keyPressed, this, &KinectV2Manager::keyPressed);
        ofRegisterMouseEvents(this);
        camEasyCam.enableMouseInput();
    }
    bAddedInputListeners = true;
}


void KinectV2Manager::removeInputListeners() {
    ofRemoveListener(ofEvents().keyPressed, this, &KinectV2Manager::keyPressed);
    ofUnregisterMouseEvents(this);
    bAddedInputListeners = false;
    camEasyCam.disableMouseInput();

}

void KinectV2Manager::KinectV2ManagerDebugToggle(){
    if(APPC->gui->kinectManagerDebug==true){
        addInputListeners();
    }else{
        removeInputListeners();
    }
}


//EVENT HANDLER
//------------------------------------------------------

void KinectV2Manager :: _update( ofEventArgs &e )
{
    update();
}

void KinectV2Manager :: _draw( ofEventArgs &e )
{
    draw();
}


void KinectV2Manager :: _exit( ofEventArgs &e )
{
    exit();
}

