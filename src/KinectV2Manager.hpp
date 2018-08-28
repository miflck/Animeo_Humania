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
    vector<UnMappedPoints> unMappedSkelettons;


    ofNode testnode;
    ofNode hand;
    ofNode righthandDebug;

    

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
    vector<UnMappedPoints> getUnMappedSkelettons();

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
