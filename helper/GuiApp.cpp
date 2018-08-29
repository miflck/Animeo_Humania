/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"

void GuiApp::setup(){
	parameters.setName("parameters");
	parameters.add(radius.set("radius",50,1,100));
	parameters.add(color.set("color",100,ofColor(0,0),255));
    parameters.add(testPosition.set("testPosition", ofVec3f(50),ofVec3f(-1000),ofVec3f(1000) )); // this will create a slider group for your vec3 in the gui.
    parameters.add(kinectPosition.set("kinectPosition", ofVec3f(50),ofVec3f(-800),ofVec3f(7000) )); // this will create a slider group for your vec3 in the gui.
    parameters.add(beamerPosition.set("beamerPosition", ofVec3f(-800),ofVec3f(-1000),ofVec3f(10000) )); // this will create a slider group for your vec3 in the gui.
    parameters.add(beamerFov.set("beamerFov",60,1,100));
    parameters.add(kinectscalefact.set("kinectscalefact",0.4,0.4,2));
    parameters.add(rayPosition.set("rayPosition", ofVec3f(100),ofVec3f(0),ofVec3f(50000) )); // this will create a slider group for your vec3 in the gui.
    parameters.add(kinectManagerDebug.set("kinectManagerDebug",false));

    box2dPanel.setup("EMOTIONS","emotionssettings.xml",250,0);
    box2dPanel.setName("Box2d");
    box2dbasics.add(emotionsgravity.set("gravity",-3,-10,10));
    box2dbasics.add(drag.set("drag",0.95,0.5,1));

    
    box2dPanel.add(box2dbasics);

	gui.setup(parameters);

	ofBackground(0);
	ofSetVerticalSync(false);
    
    gui.loadFromFile("settings.xml");
    
    ofBackground(0);


}

void GuiApp::update(){

}

void GuiApp::draw(){
   vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    vector<UnMappedPoints> uskel=KINECTMANAGER->getUnMappedSkelettons();

    ofPushMatrix();
    ofScale(0.3,0.3);
    for(int i=0;i<mskel.size();i++){
        ofSetColor(255, 0, 0);
        mskel[i].drawSkeletton();
    }
    
    for(int i=0;i<uskel.size();i++){
        ofSetColor(0, 255, 0);
        uskel[i].drawSkeletton();
    }
    ofPopMatrix();

    
	gui.draw();
    box2dPanel.draw();
    
    string info = "";
    info += "FPS: "+ofToString(ofGetFrameRate())+"\n";

    ofSetColor(255);
    ofDrawBitmapString(info, 10, 10);
    
}



