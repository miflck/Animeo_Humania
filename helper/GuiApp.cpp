/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

void GuiApp::setup(){
	parameters.setName("parameters");
	parameters.add(radius.set("radius",50,1,100));
	parameters.add(color.set("color",100,ofColor(0,0),255));
    
    parameters.add(testPosition.set("testPosition", ofVec3f(50),ofVec3f(0),ofVec3f(500) )); // this will create a slider group for your vec3 in the gui.

    parameters.add(kinectPosition.set("kinectPosition", ofVec3f(50),ofVec3f(-800),ofVec3f(800) )); // this will create a slider group for your vec3 in the gui.
    parameters.add(beamerPosition.set("beamerPosition", ofVec3f(100),ofVec3f(0),ofVec3f(1100) )); // this will create a slider group for your vec3 in the gui.
    parameters.add(beamerFov.set("beamerFov",60,1,100));

    parameters.add(kinectscalefact.set("kinectscalefact",0.4,0.4,2));

    parameters.add(rayPosition.set("rayPosition", ofVec3f(100),ofVec3f(0),ofVec3f(50000) )); // this will create a slider group for your vec3 in the gui.


	gui.setup(parameters);
	ofBackground(0);
	ofSetVerticalSync(false);
}

void GuiApp::update(){

}

void GuiApp::draw(){
	gui.draw();
}
