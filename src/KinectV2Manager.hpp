//
//  KinectV2Manager.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef KinectV2Manager_hpp
#define KinectV2Manager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"

#include "ofxKinectV2OSC.h"


#include "OrthoCamera.h"
#include "helper.hpp"


#define KINECTMANAGER KinectV2Manager::getInstance()


#define N_CAMERAS 6



/*
struct MappedPoints{
    ofVec3f leftHand;
    ofVec3f leftShoulder;
    ofVec3f leftEllbow;
    ofVec3f leftWrist;
    ofVec3f leftHip;
    ofVec3f leftKnee;
    ofVec3f leftAnkle;
    ofVec3f leftFoot;
    
    ofVec3f rightHand;
    ofVec3f rightShoulder;
    ofVec3f rightEllbow;
    ofVec3f rightWrist;
    ofVec3f rightHip;
    ofVec3f rightKnee;
    ofVec3f rightAnkle;
    ofVec3f rightFoot;
    
    ofVec3f head;
    ofVec3f neck;
    ofVec3f spineBase;
    ofVec3f spineMid;
    
    
    
    ofVec3f localZero;
    ofVec3f leftHandLocal;
    ofVec3f leftShoulderLocal;
    ofVec3f leftEllbowLocal;
    ofVec3f leftWristLocal;
    ofVec3f leftHipLocal;
    ofVec3f leftKneeLocal;
    ofVec3f leftAnkleLocal;
    ofVec3f leftFootLocal;
    
    ofVec3f rightHandLocal;
    ofVec3f rightShoulderLocal;
    ofVec3f rightEllbowLocal;
    ofVec3f rightWristLocal;
    ofVec3f rightHipLocal;
    ofVec3f rightKneeLocal;
    ofVec3f rightAnkleLocal;
    ofVec3f rightFootLocal;
    
    ofVec3f headLocal;
    ofVec3f neckLocal;
    ofVec3f spineBaseLocal;
    ofVec3f spineMidLocal;
    
    void drawSkeletton(){
        ofDrawCircle(leftHand,20);
        ofDrawLine(leftHand,leftEllbow);
        ofDrawLine(leftEllbow,leftShoulder);
        ofDrawCircle(head,20);
        ofDrawLine(leftShoulder,neck);
        ofDrawCircle(rightHand,20);
        ofDrawLine(rightHand,rightEllbow);
        ofDrawLine(rightEllbow,rightShoulder);
        ofDrawLine(rightShoulder,neck);
        ofDrawLine(neck,spineMid);
        ofDrawLine(spineMid,leftHip);
        ofDrawLine(spineMid,rightHip);
        ofDrawLine(rightHip,rightKnee);
        ofDrawLine(leftHip,leftKnee);
        
    }
};
 */



class KinectV2Manager {
public:
    
    static KinectV2Manager* getInstance();
    void initialize();
    bool isInitialized();
    
    
    //events
    virtual void addListeners        ();
    virtual void removeListeners    ();
    
    //events
    virtual void addInputListeners        ();
    virtual void removeInputListeners    ();
    
    //KinectV2Manager();
    //virtual ~KinectV2Manager();
    
    

    
    void turnOn();
    void turnOff();
    
    
    virtual void _update(ofEventArgs &e);
    virtual void _draw(ofEventArgs &e);
    virtual void _exit(ofEventArgs &e);
    
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
    bool bIsRunning;
    
    void toggleMouseActive();
    
    
    ofxKinectV2OSC kinect;
    Skeleton* skeleton;
    vector<Skeleton>* skeletons;
    ofTrueTypeFont smallFont, largeFont;
    
    BodyRenderer renderer;
    
    ofEasyCam camEasyCam;
    ofCamera kinectCam;
    ofCamera beamerCam;
    orthoCamera camFront;
    orthoCamera camTop;
    orthoCamera camLeft;
    
    
    
    //camera pointers
    ofCamera * cameras[N_CAMERAS];
    int iMainCamera;
    
    //viewports
    ofRectangle viewMain;
    ofRectangle viewGrid[N_CAMERAS];
    
    bool draw_sidecameras=true;

    ofVec3f kinectposition;
    ofVec3f beamerposition;

    

    void setupViewports();
    void drawScene(int iCameraDraw);
    
    
    ofNode sweetspot;
    
    vector<ofVec3f>lefthands;
    vector<ofVec3f>righthands;
    vector<ofVec3f>heads;


    vector<MappedPoints> mappedSkelettons;

    ofNode testnode;
    ofNode hand;
    ofNode righthand;

    

    ofPlanePrimitive plane;
    
    ofVec3f intersectLine(ofVec3f a, ofVec3f b, ofVec3f n, float d); // we'll get to this later
    ofVec3f screenpos;
    ofVec3f kinectToWorld(ofVec3f _pos);
    static ofVec3f kinectToWorld(ofVec3f _kinectpos, ofVec3f _pos);

    void KinectV2ManagerDebugToggle(const void * sender, bool & pressed);
    
    vector<Skeleton>* getSkelettons();
  //  ofVec3f getScaledKinectPosition();
    
    vector<ofVec3f> getLeftHands();
    
    vector<MappedPoints> getMappedSkelettons();
    
    ofVec3f getLocal( ofVec3f p, ofVec3f sm);

    
private:
    KinectV2Manager();
    static KinectV2Manager* instance;
    bool initialized;
    bool bIsMouseActive=false;
    
    bool bAddedInputListeners;
    bool bAddedListeners;

};


#endif /* Physics_hpp */
