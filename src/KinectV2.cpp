//
//  KinectV2.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "KinectV2.hpp"
#include "ApplicationController.h"

KinectV2::KinectV2(){
    init();
}


KinectV2::~KinectV2(){
    
}

void KinectV2::init(){
    cout<<"init KinectV2"<<endl;
    bAddedListeners = false;
    
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
    
  
    
    beamerheight=0;
    beamerlookat=beamerheight;
    
    
    beamerposition.set(0,0,500);
    kinectposition.set(100,100,100);

    
    
    iMainCamera = 0;
    bCamParent = false;
    
    myfov=60;
    sweetspot.setPosition(0, 0, 0);
    hand.setPosition(0,0,0);
    
    // observer camera

    beamerCam.setPosition(beamerposition);
    beamerCam.lookAt(sweetspot);
    beamerCam.setFov(60);
    beamerCam.setFarClip(20000);
    
    // virtual beamer camera
   // camEasyCam.setPosition(beamerposition.x,beamerposition.y-50,beamerposition.z);
    camEasyCam.setTarget(ofVec3f(0,0,0));
    camEasyCam.tilt(15);
    camEasyCam.setDistance(1000);
    
    

    int kinectfov=60;
    
    // kinect camera
    kinectCam.setPosition(kinectposition);
    //kinectEasyCam.setTarget(ofVec3f(kinectxoffset, kinectheight, 0));
    
    
    
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
    
    //defining the real world coordinates of the window which is being headtracked is important for visual accuracy
    windowWidth = 0.3f;
    windowHeight = 0.2f;
    
    windowTopLeft = ofVec3f(-windowWidth / 2.0f,
                            +windowHeight / 2.0f,
                            0.0f);
    windowBottomLeft = ofVec3f(-windowWidth / 2.0f,
                               - windowHeight / 2.0f,
                               0.0f);
    windowBottomRight = ofVec3f(+windowWidth / 2.0f,
                                -windowHeight / 2.0f,
                                0.0f);
    
    //we use this constant since we're using a really hacky headtracking in this example
    //if you use something that can properly locate the head in 3d (like a kinect), you don't need this fudge factor
    viewerDistance = 0.4f;
    
    
}

void KinectV2::update(){
    //Each frame check for new Kinect OSC messages
    kinect.update();
    
    beamerposition.set(APPC->gui->beamerPosition);
    //beamerposition*=(APPC->gui->kinectscalefact);
    testnode.setPosition(APPC->gui->testPosition);
    beamerCam.setPosition(beamerposition);
    beamerCam.setFov(APPC->gui->beamerFov);

    kinectposition.set(APPC->gui->kinectPosition);
    //kinectposition*=(APPC->gui->kinectscalefact);
    kinectCam.setPosition(kinectposition);

    
    for(int i = 0; i < skeletons->size(); i++) {
        ofVec3f userHandLeftPoint = skeletons->at(i).getHandLeft().getPoint();
        hand.setPosition(kinectToWorld(userHandLeftPoint));
        cout<<kinectToWorld(userHandLeftPoint)<<endl;
    }

    
}


void KinectV2::draw(){
    
   // ofBackground(ofColor::darkGray);
    
    //Print out strings with the values from the network
    kinect.drawDebug();
    
    
    
    
    //We passed the skeleton list pointer to the renderer earlier,
    //now we tell it to draw them
    //renderer.draw();
    
    //If you want to stop using the default renderer and start
    //drawing your own graphics, uncomment this for a starting point:
    for(int i = 0; i < skeletons->size(); i++) {
        
        ofVec3f userHandLeftPoint = skeletons->at(i).getHandLeft().getPoint();
        ofCircle(userHandLeftPoint.x, userHandLeftPoint.y, 60);
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofPopStyle();
        cout<<userHandLeftPoint.x<<endl;
        
        ofSetColor(ofColor::fromHsb(ofGetFrameNum() % 255, 255, 255));
        Joint handLeft = skeletons->at(i).getHandLeft();
        ofCircle(ofMap(handLeft.x(),-1,1,0,ofGetWidth()), handLeft.y(), 60);
        Joint handRight = skeletons->at(i).getHandRight();
        ofCircle(handRight.x(), handRight.y(), 60);
    }
    
    //Print out commands and text
    string commands = "COMMANDS\n\n";
    commands.append("d = debug\n");
    commands.append("j = joints\n");
    commands.append("b = bones\n");
    commands.append("h = hands\n");
    commands.append("r = ranges\n");
    
    ofSetColor(ofColor::white);
    smallFont.drawString(commands, 20, 40);
    largeFont.drawString("fps:\n" + ofToString(ofGetFrameRate()), 20, ofGetHeight() - 100);
 
    
    // draw main viewport
    cameras[iMainCamera]->begin(viewMain);
    drawScene(iMainCamera);
    
    // calculate mouse ray whilst this camera is active
   // updateMouseRay();
    
    cameras[iMainCamera]->end();
    
    if(draw_sidecameras){
        // draw side viewports
        for(int i = 0; i < N_CAMERAS; i++){
            cameras[i]->begin(viewGrid[i]);
            drawScene(i);
            cameras[i]->end();
        }
    }
    
    
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
    
    
 /*   ofVec3f intersection;
    intersection.set(intersectLine(beamerposition,hand.getPosition(),ofVec3f(0,0,1),0)); // we'll get to this later
    ofDrawCircle(intersection.x, intersection.y, 20);
   screenpos=cameras[5]->worldToScreen(intersection, viewMain);
    ofDrawCircle(screenpos,10);
    
    
    
    
    
    ofVec3f intersection2;
    intersection2.set(intersectLine(APPC->gui->rayPosition,hand.getPosition(),ofVec3f(0,0,1),0)); // we'll get to this later
    ofSetColor(0, 0, 255);
    ofDrawCircle(intersection2.x, intersection2.y, 20);
    screenpos=cameras[5]->worldToScreen(intersection2, viewMain);
    ofSetColor(0, 255, 255);

    ofDrawCircle(screenpos,10);
    
    ofVec3f intersection3;
    intersection3.set(intersectLine(beamerposition,testnode.getPosition(),ofVec3f(0,0,1),0)); // we'll get to this later
    ofDrawCircle(intersection3.x, intersection3.y, 20);
    screenpos=cameras[5]->worldToScreen(intersection3, viewMain);
    
    ofSetColor(255, 255, 0);

    ofDrawCircle(screenpos,10);
    */
    
}

void KinectV2::exit(){
}



//KEY LISTENER
//--------------------------------------------------------------
void KinectV2::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'c') {
     
    }
    
    
    if(e.key == 'd') kinect.toggleDebug();
    if(e.key == 'j') renderer.toggleJoints();
    if(e.key == 'b') renderer.toggleBones();
    if(e.key == 'h') renderer.toggleHands();
    if(e.key == 'r') renderer.toggleRanges();
    
    
    
    if(e.key >= '1' && e.key <= '6'){
        iMainCamera = e.key - '1';
    }
    
    
}


//--------------------------------------------------------------
void KinectV2::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void KinectV2::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2::mousePressed(ofMouseEventArgs &a){
  

}

//--------------------------------------------------------------
void KinectV2::mouseReleased(ofMouseEventArgs &a){
    
}

void KinectV2::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void KinectV2::mouseExited(ofMouseEventArgs &a){
    
}


void KinectV2::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void KinectV2::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void KinectV2::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}


//--------------------------------------------------------------
void KinectV2::setupViewports(){
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
void KinectV2::drawScene(int iCameraDraw){
        ofDrawAxis(100);
        //nodeGrid.draw();
        plane.drawWireframe();
  // cameras[4]->drawFrustum();
   // cameras[5]->draw();

    
    hand.draw();
    testnode.draw();
    ofDrawBox(-200,-100,0,100);
    
   /* ofVec3f intersection;
    intersection.set(intersectLine(beamerposition,hand.getPosition(),ofVec3f(0,0,1),50)); // we'll get to this later
    ofDrawCircle(intersection.x, intersection.y, 20);
    */
    
    
    ofVec3f intersection3;
    intersection3.set(intersectLine(cameras[5]->getGlobalPosition(),testnode.getPosition(),ofVec3f(0,0,1),0)); // we'll get to this later
    ofSetColor(255, 255, 0);
    ofDrawCircle(intersection3.x, intersection3.y, 20);
    
   // screenpos=cameras[5]->worldToScreen(intersection3, viewMain);
    
   // ofDrawCircle(screenpos,10);
 
    ofVec3f v1 = cameras[5]->getGlobalPosition();
    ofVec3f v2 = testnode.getGlobalPosition();
    ofSetColor(0, 255, 0);
   // ofDrawLine(v1,v2);
    
     v1 = beamerposition;
     v2 = testnode.getPosition();
    ofSetColor(0, 255, 255);
    ofDrawLine(v1,v2);

    
     v1 = cameras[5]->getGlobalPosition();
     v2 = sweetspot.getGlobalPosition();
    ofDrawLine(v1,v2);
        // draw cameras
        //for(int i=0; i<N_CAMERAS; i++) {
           // ofSetColor(255, 255, 0);
         //   cameras[i]->draw();
            //cameras[i]->drawFrustum();
        //}

    ofPushStyle();
    ofSetColor(150, 100, 100);
    ofDrawGrid(1.0f, 5.0f, true);
    //--
    //draw window preview
    //
    window.clear();
    window.addVertex(windowTopLeft);
    window.addVertex(windowBottomLeft);
    window.addVertex(windowBottomRight);
    window.setMode(OF_PRIMITIVE_LINE_STRIP);
    window.draw();
    glPointSize(3.0f);
    window.drawVertices();
    //
    //--
    ofPopStyle();

}

ofVec3f KinectV2::   intersectLine(ofVec3f a, ofVec3f b, ofVec3f n,float d) {
    ofVec3f ba = b-a;
    float nDotA = n.dot(a);
    float nDotBA = n.dot(ba);
    return a + (((d - nDotA)/nDotBA) * ba);
}

ofVec3f KinectV2:: kinectToWorld (ofVec3f _pos){
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

