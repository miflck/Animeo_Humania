//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef KinectV2_hpp
#define KinectV2_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"

#include "ofxKinectV2OSC.h"


#include "Grid.h"
#include "OrthoCamera.h"


#define N_CAMERAS 6

struct Plane {
    ofVec3f n; // normal
    float d; // distance from origin
    
    Plane(); // default constructor
    Plane(ofVec3f a, ofVec3f b, ofVec3f c); // plane from 3 points
};


class KinectV2:public ApplicationBase{
public:
    KinectV2();
    virtual ~KinectV2();
    
    

    
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
    
    
    
    
    //cameras have parent?
    bool bCamParent;
    
    //camera pointers
    ofCamera * cameras[N_CAMERAS];
    int iMainCamera;
    
    //viewports
    ofRectangle viewMain;
    ofRectangle viewGrid[N_CAMERAS];
    ofRectangle stage;
    
    bool draw_sidecameras=true;
    int myfov;
    int beamerlookat;
    ofVec3f beamerposition;
    int beamerdistance;
    int beamerheight;
    
    

    int kinectfov;
    ofVec3f kinectposition;

    

    void setupViewports();
    void drawScene(int iCameraDraw);
    grid nodeGrid;
    
    
    ofNode sweetspot;
    
    ofNode hand;
    ofNode testnode;



    
    
    ofPlanePrimitive plane;
    
    ofVboMesh window;

    
    ofVec3f windowTopLeft;
    ofVec3f windowBottomLeft;
    ofVec3f windowBottomRight;

    
    bool usePreview;
    float windowWidth;
    float windowHeight;
    float viewerDistance;
    
    
    ofVec3f intersectLine(ofVec3f a, ofVec3f b, ofVec3f n, float d); // we'll get to this later

    ofVec3f screenpos;
    
    ofVec3f kinectToWorld(ofVec3f _pos);
    
private:
   
    
    bool bIsMouseActive=false;
    
};


#endif /* Physics_hpp */
