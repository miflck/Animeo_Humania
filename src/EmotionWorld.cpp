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
    box2d.enableEvents();   // <-- turn on the event listener
    box2d.setGravity(0, gravityY);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    // register the listener so that we get the events
    ofAddListener(box2d.contactStartEvents, this, &EmotionWorld::contactStart);
    ofAddListener(box2d.contactEndEvents, this, &EmotionWorld::contactEnd);
    
    anchorLeftHand.setPhysics(50, 0.5, 0.9);
    anchorLeftHand.setup(box2d.getWorld(), 0, 0, 70);
    
    anchorRightHand.setPhysics(50, 0.5, 0.9);
    anchorRightHand.setup(box2d.getWorld(), 0, 0, 70);
  
  /*  box.setup(box2d.getWorld(), ofGetWidth()/2, -20, ofGetWidth(), 20);
    leftbox.setup(box2d.getWorld(), 0, 150,20,300);
    rightbox.setup(box2d.getWorld(), ofGetWidth(),150, 20,300);
*/
    
    screen.allocate(1920,1080, GL_RGB);
    screen.begin();
    ofClear(0,0,0,0);
    screen.end();
    
    herz.loadImage("herz.png");
    sternImg.load("bilder/stern.png");

    sun.setup();
    sun.bSeekTarget=true;
    
    
    balloon.setup();
    bird.setup();

   // baloon.bSeekTarget=true;
    
    savedemitterposition=&Settings::getVec2("emotions/emitterposition");
    emitterposition.set(savedemitterposition->x,savedemitterposition->y);
    emitteroffset.set(400,0);
    
    
    savedeanchorposition=&Settings::getVec2("emotions/anchorposition");
   anchorposition.set(savedeanchorposition->x,savedeanchorposition->y);
   
    
    savedeanchorpositionBottom=&Settings::getVec2("emotions/anchorpositionBottom");
    anchorpositionBottom.set(savedeanchorpositionBottom->x,savedeanchorpositionBottom->y);
    
    
    ofDirectory dir;
    dir.listDir("Sounds/Multiplopp");
    dir.sort();
    multiploppsounds.resize(dir.size());
    
    for(int i=0; i<dir.size(); i++ ){
        multiploppsounds[i].load(dir.getPath(i));
        ofLog(OF_LOG_NOTICE,"songs loaded")<<i<<" path "<<dir.getPath(i);
    }
    
    
    dir.listDir("Sounds/Plopp");
    dir.sort();
    ploppsounds.resize(dir.size());
    
    for(int i=0; i<dir.size(); i++ ){
        ploppsounds[i].load(dir.getPath(i));
        ofLog(OF_LOG_NOTICE,"songs loaded")<<i<<" path "<<dir.getPath(i);
    }
    anchorSound.load("Sounds/Anker0.wav");
    anchorSound.setMultiPlay(true);
    
    ofAddListener(APPC->oscmanager.onMessageReceived, this, &EmotionWorld::onMessageReceived);
    
    
}

void EmotionWorld::update(){
    
    ofVec2f hand;
    ofVec2f knee;

    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    repulsionPosition.set(ofGetMouseX(),ofGetMouseY());
    
    if(mskel.size()>0){
        repulsionPosition.set(mskel[0].rightKnee);
    }
    
    
    
    
    
    for(int i=0;i<shapes.size();i++){
        float dis = repulsionPosition.distance(shapes[i].get()->getPosition());
        if(dis < minDis && bIsRepulsionActive){
            shapes[i].get()->addRepulsionForce(repulsionPosition,repulsionForce);
        }
        shapes[i]->update();
    }
    
    for(int i=0;i<triangles.size();i++){
        float dis = repulsionPosition.distance(triangles[i].get()->getPosition());
        if(dis < minDis && bIsRepulsionActive){
            triangles[i].get()->addRepulsionForce(repulsionPosition,repulsionForce);
        }
        triangles[i]->update();
    }
    
    for(int i=0;i<hearts.size();i++){
        float dis = repulsionPosition.distance(hearts[i].get()->getPosition());
        if(dis < minDis && bIsRepulsionActive){
            hearts[i].get()->addRepulsionForce(repulsionPosition,repulsionForce);
        }
        hearts[i]->update();
    }
    
    for(int i=0;i<anchors.size();i++){
        anchors[i]->setRotationFriction(0.8);
        anchors[i]->update();
    }
    
    for(int i=0;i<kreise.size();i++){

        float dis = repulsionPosition.distance(kreise[i].get()->getPosition());
        if(dis < minDis && bIsRepulsionActive){
            kreise[i].get()->anchor.addRepulsionForce(repulsionPosition,repulsionForce);
        }
        
        if(bCircleFollowMouse)kreise[i]->setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        kreise[i]->update();
    }
    
    for(int i=0;i<dreiecke.size();i++){
        if(bCircleFollowMouse)dreiecke[i]->setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        dreiecke[i]->update();
    }
    
    for(int i=0;i<sterne.size();i++){
        float dis = repulsionPosition.distance(sterne[i].get()->getPosition());
        if(dis < minDis && bIsRepulsionActive){
            sterne[i].get()->anchor.addRepulsionForce(repulsionPosition,repulsionForce);
        }
        sterne[i]->update();
    }
    
    
    for(int i=0;i<ellipsen.size();i++){
        ellipsen[i]->update();
        if(ellipsen[i]->bShouldRemove)emitMultiShapes(20,ofVec2f(ellipsen[i]->getPosition().x,50));
    }
    

    
    box2d.update();
    
    //headposition=ofVec2f(ofGetMouseX(),ofGetMouseY());
    
   // emitterposition=ofVec2f(ofGetMouseX(),ofGetMouseY());
    

   
    
    //baloon.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
   // baloon.setTarget(emitterposition.x,emitterposition.y);

    
    
    
    // automatic add ellipse?
    if(mskel.size()>0 && bIsEllipseAutomated){
        Hand hl = KINECTMANAGER->skeletons->at(0).getLeftHand();
        Joint handJointleft =KINECTMANAGER->skeletons->at(0).getHandLeft();
        
        Hand hr = KINECTMANAGER->skeletons->at(0).getRightHand();
        Joint handJointright =KINECTMANAGER->skeletons->at(0).getHandRight();
        
        ofVec2f lefthand=mskel[0].leftHand;
        ofVec2f righthand=mskel[0].rightHand;
        ofVec2f spineBase=mskel[0].spineBase;

        ofVec2f dist=lefthand-righthand;
        ofVec2f ldist=lefthand-spineBase;

        
        
        if(hl.isConfidentlyDetected() && hr.isConfidentlyDetected()) {
            if(hl.isClosed() && hr.isClosed() && bIsEllipseAdded){
                releaseEllipse();
            }
        }
        
        if( !bIsEllipseAdded && abs(ldist.length())<50 && abs(dist.length())<50){
            addEllipse();
        }
        
        
    }
    
   
    
    
    
    
    if(mskel.size()>0){
        ofVec2f epos;
        
        ofVec2f lefthand=mskel[0].leftHand;
        ofVec2f righthand=mskel[0].rightHand;
        ofVec2f middle=(lefthand-righthand)/2;
        epos.set(righthand+middle);
        
        
       

        
        
       if(ellipsen.size()>0 && ellipsen.back()->getState()==MOVINGOBJECT)ellipsen.back()->setTarget(epos);
        if(ellipsen.size()>0 && ellipsen.back()->getState()==MOVINGOBJECT)ellipsen.back()->setRadiusTarget(middle.length()+20);
    }
    

    
    
    
    if(mskel.size()<=0){
    sun.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        
    }else{
        hand=mskel[0].leftHand;
        hand=ofVec2f(hand.x, hand.y);
        sun.setTarget(hand);

    }
    sun.update();
    balloon.update();
    bird.update();
    
    for(int i=0;i<movingObjects.size();i++){
        movingObjects[i].setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
        movingObjects[i].update();
    }
    
  for(int i=0;i<flashes.size();i++){
        flashes[i]->update();
    }
    
    //emitterposition=savedemitterposition;
    

    
    if(mskel.size()>0 && bBindToHead){
        headposition=mskel[0].head;
        headposition=ofVec2f(headposition.x, headposition.y);
        emitterposition=headposition;
       // anchorLeftHand.setPosition(mskel[0].leftHand.x, mskel[0].leftHand.y);
       // anchorRightHand.setPosition(mskel[0].rightHand.x, mskel[0].rightHand.y);

    }
    
    if(mskel.size()>0 && bBindHands){
        anchorLeftHand.setPosition(mskel[0].leftHand.x, mskel[0].leftHand.y);
        anchorRightHand.setPosition(mskel[0].rightHand.x, mskel[0].rightHand.y);
    }
    
    
    

        float rAdd=ofRandom(1);
        if(rAdd>emitFrequency && bEmitHearts){
           float r = ofRandom(10, 40);        // a random radius 4px - 20px
            hearts.push_back(shared_ptr<Heart>(new Heart));
            hearts.back().get()->setPhysics(3.0, 0.53, 0.5);
            hearts.back().get()->setup(box2d.getWorld(), emitterposition.x, emitterposition.y, 0);
            hearts.back().get()->setVelocity(ofRandom(-10,10), ofRandom(0,-10));
            hearts.back().get()->setAngularVelocity(ofRandom(1));
            playRandomPlopp();

        }
    

    
    if(bEmitShapes)emitShapes();

    
   // cout<<shapes.size()<<endl;
    
    // remove shapes offscreen
    //ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    //ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(shapes, Shape::shouldRemoveOffScreen);
    ofRemove(hearts, Heart::shouldRemoveOffScreen);
    ofRemove(flashes, MovingObject::shouldRemoveOffScreen);
    ofRemove(triangles, Triangle::shouldRemoveOffScreen);
    ofRemove(anchors, AnchorTriangle::shouldRemoveOffScreen);    
    ofRemove(kreise, Kreis::shouldRemoveFromScreen);
    ofRemove(dreiecke, Dreieck::shouldRemoveFromScreen);

    ofRemove(ellipsen, IAKreis::shouldRemoveFromScreen);

   
    ofRemove(sterne, Stern::shouldRemoveFromScreen);

  
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
        anchorLeftHand.draw();
        anchorRightHand.draw();

        leftbox.draw();
        rightbox.draw();
        ofPopStyle();
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
        vector<UnMappedPoints> uskel=KINECTMANAGER->getUnMappedSkelettons();

            // hand open?
      if(mskel.size()>0){
          Hand hl = KINECTMANAGER->skeletons->at(0).getLeftHand();
          Joint handJointleft =KINECTMANAGER->skeletons->at(0).getHandLeft();
          
          Hand hr = KINECTMANAGER->skeletons->at(0).getRightHand();
          Joint handJointright =KINECTMANAGER->skeletons->at(0).getHandRight();
            if(hl.isConfidentlyDetected()) {
                ofFill();
                if(hl.isOpen()) ofSetColor(ofColor::blue);
                else ofSetColor(ofColor::red);
                ofDrawCircle(mskel[0].leftHand, 25);
            }
          
          if(hr.isConfidentlyDetected()) {
              ofFill();
              if(hr.isOpen()) ofSetColor(ofColor::blue);
              else ofSetColor(ofColor::red);
              ofDrawCircle(mskel[0].rightHand, 25);
          }
      }
        
        
        for(int i=0;i<mskel.size();i++){
            ofSetColor(255, 0, 0);
            mskel[i].drawSkeletton();
        }
        
        for(int i=0;i<uskel.size();i++){
            ofSetColor(0, 255, 0);
           // uskel[i].drawSkeletton();
        }
        
        
    }
    
    ofPushStyle();
    if(bShowSun)sun.draw();
    if(bShowBalloon)balloon.draw();
    if(bShowBird)bird.draw();

    
    
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
    for(int i=0;i<anchors.size();i++){
        anchors[i]->draw();
    }
 
    for(int i=0;i<kreise.size();i++){
        kreise[i]->draw();
    }
    
    for(int i=0;i<dreiecke.size();i++){
        dreiecke[i]->draw();
    }
    
    for(int i=0;i<sterne.size();i++){
        sterne[i]->draw();
    }
   for(int i=0;i<ellipsen.size();i++){
        ellipsen[i]->draw();
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



void EmotionWorld::bindToSkeletton(bool _b){
    bBindToHead=_b;
    if(!bBindToHead) emitterposition.set(savedemitterposition->x,savedemitterposition->y);

}


void EmotionWorld::bindToHands(bool _b){
    bBindHands=_b;
    if(!bBindHands){
        anchorLeftHand.setPosition(0,0);
        anchorRightHand.setPosition(0,0);
    }
    
}

void EmotionWorld::addEllipse(){
    bIsEllipseAdded=true;
    ofVec2f epos(emitterposition);
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

    if(mskel.size()>0){
        ofVec2f lefthand=mskel[0].leftHand;
        ofVec2f righthand=mskel[0].rightHand;
        ofVec2f middle=(lefthand-righthand)/2;
        epos.set(righthand+middle);
    }
    ellipsen.push_back(shared_ptr<IAKreis>(new IAKreis));
    ellipsen.back().get()->setWorld(box2d.getWorld());
    ellipsen.back().get()->bSeekTarget=true;
    ellipsen.back().get()->setRadius(ofRandom(200,500));
    ellipsen.back().get()->setPosition(epos.x,epos.y);
    ellipsen.back().get()->setTarget(ofVec2f(epos.x,epos.y));
    ellipsen.back().get()->setup();
    playRandomPlopp();

}

void EmotionWorld::releaseEllipse(){
    for(int i=0;i<ellipsen.size();i++){
        if(ellipsen[i]->getState()==MOVINGOBJECT){
            ellipsen[i]->setState(PHYSICS);
            break;
        }
    }
    bIsEllipseAdded=false;
}


void EmotionWorld::emitShapes(){
    float rAdd=ofRandom(-1,1);
    if(rAdd>emitShapeFrequency){
        float r =0;        // a random radius 4px - 20px
        shapes.push_back(shared_ptr<Shape>(new Shape));
        shapes.back().get()->setPhysics(5, 0.6, 0.3);
        shapes.back().get()->setup(box2d.getWorld(), emitterposition.x, emitterposition.y,r);
        shapes.back().get()->setVelocity(ofRandom(5,10), ofRandom(-5,10));
        playRandomPlopp();
    }
    
    rAdd=ofRandom(-1,1);
    if(rAdd<-emitShapeFrequency){
        float r=ofRandom(20,30);
        ofVec2f a =ofVec2f(emitterposition.x-r/2,emitterposition.y);
        ofVec2f b =ofVec2f(emitterposition.x+r/2,emitterposition.y);
        ofVec2f c =ofVec2f(emitterposition.x,emitterposition.y+r);
        triangles.push_back(shared_ptr<Triangle>(new Triangle(a,b,c)));
        triangles.back().get()->setWorld(box2d.getWorld());
        triangles.back().get()->setPhysics(5.0, 0.6, 0.3);
        triangles.back().get()->create(box2d.getWorld());
        triangles.back().get()->setVelocity(ofRandom(5,10), ofRandom(-5,10));
        triangles.back().get()->setAngularVelocity(2);
        playRandomPlopp();
    }
}


void EmotionWorld::emitMultiShapes(int _n){
    
    
    for(int i=0;i<_n;i++){
        float rAdd=ofRandom(-1,1);
        if(rAdd>emitShapeFrequency){
            float r =0;        // a random radius 4px - 20px
            shapes.push_back(shared_ptr<Shape>(new Shape));
            shapes.back().get()->setPhysics(5, 0.6, 0.3);
            shapes.back().get()->setup(box2d.getWorld(), emitterposition.x, emitterposition.y,r);
            shapes.back().get()->setVelocity(ofRandom(5,10), ofRandom(-5,10));
        }
        
        rAdd=ofRandom(-1,1);
        if(rAdd<-emitShapeFrequency){
            float r=ofRandom(20,30);
            ofVec2f a =ofVec2f(emitterposition.x-r/2,emitterposition.y);
            ofVec2f b =ofVec2f(emitterposition.x+r/2,emitterposition.y);
            ofVec2f c =ofVec2f(emitterposition.x,emitterposition.y+r);
            triangles.push_back(shared_ptr<Triangle>(new Triangle(a,b,c)));
            triangles.back().get()->setWorld(box2d.getWorld());
            triangles.back().get()->setPhysics(5.0, 0.6, 0.3);
            triangles.back().get()->create(box2d.getWorld());
            triangles.back().get()->setVelocity(ofRandom(5,10), ofRandom(-5,10));
            triangles.back().get()->setAngularVelocity(2);
        }
    }
    playRandomMultiplopp();
}

void EmotionWorld::emitMultiShapes(int _n,ofVec2f pos){
    
    
    for(int i=0;i<_n;i++){
        float rAdd=ofRandom(-1,1);
        if(rAdd>emitShapeFrequency){
            float r =0;        // a random radius 4px - 20px
            shapes.push_back(shared_ptr<Shape>(new Shape));
            shapes.back().get()->setPhysics(5, 0.6, 0.3);
            shapes.back().get()->setup(box2d.getWorld(), pos.x, pos.y,r);
            shapes.back().get()->setVelocity(ofRandom(-10,10), ofRandom(0,10));
        }
        
        rAdd=ofRandom(-1,1);
        if(rAdd<-emitShapeFrequency){
            float r=ofRandom(30,70);
            ofVec2f a =ofVec2f(pos.x-r/2,pos.y);
            ofVec2f b =ofVec2f(pos.x+r/2,pos.y);
            ofVec2f c =ofVec2f(pos.x,pos.y+r);
            triangles.push_back(shared_ptr<Triangle>(new Triangle(a,b,c)));
            triangles.back().get()->setWorld(box2d.getWorld());
            triangles.back().get()->setPhysics(5.0, 0.6, 0.3);
            triangles.back().get()->create(box2d.getWorld());
            triangles.back().get()->setVelocity(ofRandom(-10,10), ofRandom(0,10));
            triangles.back().get()->setAngularVelocity(2);
        }
    }
    playRandomMultiplopp();
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


void EmotionWorld::showBalloon(bool _s){
    bShowBalloon=_s;
}

void EmotionWorld::showBird(bool _s){
    bShowBird=_s;
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


void EmotionWorld::saveEmitterposition(ofVec2f _p){
    savedemitterposition->set(_p.x,_p.y);
    Settings::get().save("data.json");
    emitterposition.set(savedemitterposition->x,savedemitterposition->y);
}


void EmotionWorld::playRandomMultiplopp(){
    int randNum;
    randNum=(int)(ofRandom(0,multiploppsounds.size()));
    cout<<randNum<<endl;
    multiploppsounds[randNum].play();
}

void EmotionWorld::playRandomPlopp(){
    int randNum;
    randNum=(int)(ofRandom(0,ploppsounds.size()));
    ploppsounds[randNum].play();
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
    
    
   /* if(e.key == 'o') {
       /* MovingObject m;
        m.bSeekTarget=true;
        m.setRadius(10);
        m.setPosition(ofGetWidth()/2, 0);
        m.setTarget(ofVec2f(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight())));
        movingObjects.push_back(m);
        */
    /*    kreise.push_back(shared_ptr<Kreis>(new Kreis));
        kreise.back().get()->setWorld(box2d.getWorld());
        kreise.back().get()->bSeekTarget=true;
        kreise.back().get()->setRadius(ofRandom(200,500));
        kreise.back().get()->setPosition(headposition.x,headposition.y);
        kreise.back().get()->setTarget(ofVec2f(headposition.x+400,headposition.y));
        kreise.back().get()->setup();
*/
  //y  }
    
    if(e.key=='o'){
       /* for(int i=0;i<kreise.size();i++){
            if(kreise[i]->getState()==MOVINGOBJECT){
                kreise[i]->setState(PHYSICS);
                break;
            }
        }*/
        bCircleFollowMouse=!bCircleFollowMouse;
        
    }
    
    if(e.key=='F'){
        for(int i=0;i<kreise.size();i++){
                kreise[i]->setState(FADEOUT);
        }
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
        
       // bEmitShapes=!bEmitShapes;
        
        saveEmitterposition(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    }
    
    if(e.key=='q'){
        sun.scaleTo(100,20.f);
    }
    
    if(e.key=='w'){
        sun.scaleTo(500,20.f);
    }
    
    if(e.key=='e'){
        //sun.scaleTo(2000,20.f);
        addEllipse();
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
    
    
    if(e.key=='a'){
        savedeanchorposition->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
        anchorposition.set(savedeanchorposition->x,savedeanchorposition->y);
    }

    if(e.key=='A'){
        savedeanchorpositionBottom->set(ofGetMouseX(),ofGetMouseY());
        Settings::get().save("data.json");
        anchorpositionBottom.set(savedeanchorpositionBottom->x,savedeanchorpositionBottom->y);
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




//--------------------------------------------------------------
void EmotionWorld::contactStart(ofxBox2dContactArgs &e) {

    if(e.a != NULL && e.b != NULL) {
        
        // if we collide with the ground we do not
        // want to play a sound. this is how you do that
        if(e.a->GetType() == b2Shape::e_edge && e.b->GetType() == b2Shape::e_polygon) {
            
            SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
            SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
            
            if(aData) {
              aData->bHit = true;
                //sound[aData->soundID].play();
                //playRandomPlopp();
            }
            
            if(bData && !bData->bHit) {
               bData->bHit = true;
                //sound[bData->soundID].play();
                //playRandomPlopp();
                anchorSound.play();

            }
        }
    }
}

//--------------------------------------------------------------
void EmotionWorld::contactEnd(ofxBox2dContactArgs &e) {

    if(e.a != NULL && e.b != NULL) {
        
        SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
        SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
        
        if(aData) {
            //aData->bHit = false;
        }
        
        if(bData) {
           // bData->bHit = false;
        }
    }
}




void EmotionWorld::makeStars(int num){
    for(int i=0;i<num;i++){
     sterne.push_back(shared_ptr<Stern>(new Stern));
     sterne.back().get()->setWorld(box2d.getWorld());
     sterne.back().get()->bSeekTarget=true;
     sterne.back().get()->setPosition(emitterposition.x,emitterposition.y);
    // sterne.back().get()->setTarget(ofVec2f(ofRandom(0,ofGetWidth()),ofRandom(0,emitterposition.y)));
    sterne.back().get()->setTarget(ofVec2f(ofRandom(0,ofGetWidth()),ofRandom(0,(ofGetHeight()/3)*2)));

     sterne.back().get()->setup(sternImg);
     sterne.back().get()->setTargetRadius(ofRandom(15,25));
     }
    
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
    
    
    if(msg.getAddress() == "/EmotionWorld/toggle6")
    {
        float f=msg.getArgAsBool(0);

        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
        ofVec2f pos=ofVec2f(ofGetWidth()/2,ofGetHeight()/2);
        
        if(mskel.size()>0 && f){
           pos=mskel[0].head;
        }
        balloon.setPosition(pos.x,-30);
        showBalloon(f);
    }
    
    if(msg.getAddress() == "/Balloon/toggle24")
    {
        float f=msg.getArgAsBool(0);
       // balloon.setPosition(emitterposition.x,emitterposition.y);
       // balloon.setTarget(ofVec2f(emitterposition.x+emitteroffset.x,emitterposition.y+emitteroffset.y));
        
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
        ofVec2f pos=ofVec2f(ofGetWidth()/2,ofGetHeight()/2);
        if(mskel.size()>0 && f){
            pos=mskel[0].head;
        }
        balloon.setPosition(pos.x,-30);
        balloon.setTarget(ofVec2f(pos.x,pos.y));
        

        if(f)balloon.startEasingIn();
        showBalloon(f);
        playRandomPlopp();

        
        ofxOscMessage m;
        m.addFloatArg(ofMap(emitterposition.y+emitteroffset.y,0,ofGetHeight(),0,1));
        m.addFloatArg(ofMap(emitterposition.x+emitteroffset.x,0,ofGetWidth(),0,1));
        m.setAddress("/Balloon/xy3");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    
    
    if(msg.getAddress() == "/Balloon/toggle25")
    {
        float f=msg.getArgAsBool(0);
        bird.setPosition(emitterposition.x,emitterposition.y);
        bird.setTarget(ofVec2f(emitterposition.x+emitteroffset.x,emitterposition.y+emitteroffset.y));
        
        showBird(f);
        
        playRandomPlopp();

        
        ofxOscMessage m;
        m.addFloatArg(ofMap(emitterposition.y+emitteroffset.y,0,ofGetHeight(),0,1));
        m.addFloatArg(ofMap(emitterposition.x+emitteroffset.x,0,ofGetWidth(),0,1));
        m.setAddress("/Balloon/xy3");
        APPC->oscmanager.touchOscSender.sendMessage(m);
    }
    
    
    if(msg.getAddress() == "/Balloon/xy3")
    {
        float x=msg.getArgAsFloat(0);
        float y=msg.getArgAsFloat(1);
        x=ofMap(x,0,1,-150,ofGetWidth()+150);
        y=ofMap(y,0,1,-150,ofGetHeight()+150);
        balloon.setTarget(ofVec2f(x,y));
        bird.setTarget(ofVec2f(x,y));
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
        f=ofMap(f, -1.f, 1.f, -40, 40);
        gravityY=f;
        box2d.setGravity(gravityX, gravityY);
    }
    
    if(msg.getAddress() == "/EmotionWorld/fader10")
    {
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, -1.f, 1.f, -40, 40);
        shapeDir=f;
    }

    
    // repulsionforce
    if(msg.getAddress() == "/EmotionWorld/fader13")
    {
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, 0, 1.f, 0, 100);
        repulsionForce=f;
    }
    
    
    // GRAVITY
    if(msg.getAddress() == "/EmotionWorld/fader4")
    {
    
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, -1.f, 1.f, -10, 10);
        gravityX=f;
        box2d.setGravity(gravityX, gravityY);
    }
    
    
    if(msg.getAddress() == "/EmotionWorld/fader14")
    {
        float f=msg.getArgAsFloat(0);
        f=ofMap(f, -1.f, 1.f, -3, 3);
        gravityY=f;
        box2d.setGravity(gravityX, gravityY);
        
        float mG=ofMap(gravityY,-3,3,-1,1);
        
        ofxOscMessage m;
        m.addFloatArg(mG);
        m.setAddress("/EmotionWorld/fader2");
        APPC->oscmanager.touchOscSender.sendMessage(m);
        
        
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
        hearts.back().get()->setup(box2d.getWorld(), anchorposition.x, anchorposition.y, r);
        hearts.back().get()->setVelocity(ofRandom(5,20), ofRandom(0,-20));
        playRandomPlopp();

    }
    
    if(msg.getAddress() == "/EmotionWorld/push5")
    {
        float r = ofRandom(80, 100);        // a random radius 4px - 20px
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
            leftbox.setup(box2d.getWorld(), 0, ofGetHeight()/2,20,ofGetHeight());
            rightbox.setup(box2d.getWorld(), ofGetWidth(),ofGetHeight()/2, 20,ofGetHeight());
            
        }else{
            box.destroy();
            leftbox.destroy();
            rightbox.destroy();
        }
    }
    
    if(msg.getAddress() == "/EmotionWorld/toggle5")
    {
        float m=msg.getArgAsBool(0);
        bEmitShapes=m;
        cout<<bEmitShapes<<endl;
        
    }
    if(msg.getAddress() == "/EmotionWorld/fader5")
    {
        float f=msg.getArgAsFloat(0);
        emitShapeFrequency=f;
    }
    
    
    if(msg.getAddress() == "/EmotionWorld/push17")
    {
       /* for(int i=0;i<20;i++){
            emitShapes();
            playRandomMultiplopp();
        }*/
        emitMultiShapes(20);
        //playRandomMultiplopp();
    }
    
    
    if(msg.getAddress() == "/EmotionWorld/push18")
    {
        float r = 400;        // a random radius 4px - 20px
    

       // anchors.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        ofVec2f dir = ofVec2f(-200,-100);
        dir.normalize();
        dir*=10;
 
        r=150;
        ofVec2f a =ofVec2f(anchorposition.x-r/2,anchorposition.y);
        ofVec2f b =ofVec2f(anchorposition.x+r/2,anchorposition.y);
        ofVec2f c =ofVec2f(anchorposition.x,anchorposition.y-r);
        anchors.push_back(shared_ptr<AnchorTriangle>(new AnchorTriangle(a,b,c)));
        anchors.back().get()->setWorld(box2d.getWorld());
        anchors.back().get()->setPhysics(800.0, 0.2, 0.5);
        anchors.back().get()->create(box2d.getWorld());
        anchors.back().get()->setVelocity(dir);
        anchors.back().get()->setAngularVelocity(0);
        playRandomPlopp();
        
        
        anchors.back().get()->setData(new SoundData());
        SoundData * sd = (SoundData*) anchors.back().get()->getData();
        sd->soundID = ofRandom(0, ploppsounds.size());
        sd->bHit    = false;

        
    }
    
    if(msg.getAddress() == "/EmotionWorld/push43")
    {
        float r = 400;        // a random radius 4px - 20px
        
        
        // anchors.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        ofVec2f dir = ofVec2f(ofRandom(-200,-500),-100);
        dir.normalize();
        dir*=ofRandom(10,50);
        
        r=150;
        ofVec2f a =ofVec2f(anchorpositionBottom.x-r/2,anchorpositionBottom.y);
        ofVec2f b =ofVec2f(anchorpositionBottom.x+r/2,anchorpositionBottom.y);
        ofVec2f c =ofVec2f(anchorpositionBottom.x,anchorpositionBottom.y-r);
        anchors.push_back(shared_ptr<AnchorTriangle>(new AnchorTriangle(a,b,c)));
        anchors.back().get()->setWorld(box2d.getWorld());
        anchors.back().get()->setPhysics(800.0, 0.2, 0.5);
        anchors.back().get()->create(box2d.getWorld());
        anchors.back().get()->setVelocity(dir);
        anchors.back().get()->setAngularVelocity(0);
        playRandomPlopp();
        
        
        anchors.back().get()->setData(new SoundData());
        SoundData * sd = (SoundData*) anchors.back().get()->getData();
        sd->soundID = ofRandom(0, ploppsounds.size());
        sd->bHit    = false;
        
        
    }
    
    
    
    // KREIS
    if(msg.getAddress() == "/EmotionWorld/push27")
    {
        kreise.push_back(shared_ptr<Kreis>(new Kreis));
        kreise.back().get()->setWorld(box2d.getWorld());
        kreise.back().get()->bSeekTarget=true;
        kreise.back().get()->setRadius(ofRandom(200,500));
        kreise.back().get()->setPosition(emitterposition.x,emitterposition.y);
        kreise.back().get()->setSpeed(0,-5);

        kreise.back().get()->setTarget(ofVec2f(emitterposition.x+emitteroffset.x,emitterposition.y+emitteroffset.y));
        kreise.back().get()->setup();
        playRandomPlopp();

    }
    if(msg.getAddress() == "/EmotionWorld/push28")
    {
        for(int i=0;i<kreise.size();i++){
            if(kreise[i]->getState()==MOVINGOBJECT){
                kreise[i]->setState(PHYSICS);
                break;
            }
        }
    }
    if(msg.getAddress() == "/EmotionWorld/push29")
    {
        for(int i=0;i<kreise.size();i++){
            kreise[i]->setState(FADEOUT);
        }
    }
    
    
    
    if(msg.getAddress() == "/EmotionWorld/push40")
    {
        addEllipse();
        
    }
    if(msg.getAddress() == "/EmotionWorld/push41")
    {
        
        releaseEllipse();
    
    }
    if(msg.getAddress() == "/EmotionWorld/push42")
    {
        for(int i=0;i<ellipsen.size();i++){
            ellipsen[i]->setState(FADEOUT);
        }
    }
    
    
    
    // Dreieck
    if(msg.getAddress() == "/EmotionWorld/push31")
    {
        
        float r=ofRandom(100,200);
      
        ofVec2f a =ofVec2f(-r/2,0);
        ofVec2f b =ofVec2f(+r/2,0);
        ofVec2f c =ofVec2f(0,+r);
    
        dreiecke.push_back(shared_ptr<Dreieck>(new Dreieck (a,b,c)));
        dreiecke.back().get()->setWorld(box2d.getWorld());
        dreiecke.back().get()->bSeekTarget=true;
        dreiecke.back().get()->setRadius(r);
        dreiecke.back().get()->setPosition(emitterposition.x,emitterposition.y);
        dreiecke.back().get()->setSpeed(0,-5);
        
        dreiecke.back().get()->setTarget(ofVec2f(emitterposition.x+emitteroffset.x,emitterposition.y+emitteroffset.y));
        dreiecke.back().get()->setup();
        playRandomPlopp();

    }
    if(msg.getAddress() == "/EmotionWorld/push32")
    {
        for(int i=0;i<dreiecke.size();i++){
            if(dreiecke[i]->getState()==MOVINGOBJECT){
                dreiecke[i]->setState(PHYSICS);
                break;
            }
        }
    }
    if(msg.getAddress() == "/EmotionWorld/push33")
    {
        for(int i=0;i<dreiecke.size();i++){
            dreiecke[i]->setState(FADEOUT);
        }
    }

    
    // Stern
    if(msg.getAddress() == "/EmotionWorld/push24")
    {
        /*for(int i=0;i<60;i++){
        sterne.push_back(shared_ptr<Stern>(new Stern));
        sterne.back().get()->setWorld(box2d.getWorld());
        sterne.back().get()->bSeekTarget=true;
        sterne.back().get()->setPosition(emitterposition.x,emitterposition.y);
        sterne.back().get()->setTarget(ofVec2f(ofRandom(0,ofGetWidth()),ofRandom(0,emitterposition.y)));
        sterne.back().get()->setup(sternImg);
        sterne.back().get()->setTargetRadius(ofRandom(5,20));

        }*/
        makeStars(60);
    }
    
    if(msg.getAddress() == "/EmotionWorld/push25")
        
    {
        for(int i=0;i<sterne.size();i++){
            sterne[i]->setState(PHYSICS);
        }
    }
    
    if(msg.getAddress() == "/EmotionWorld/push30")
    {
        for(int i=0;i<sterne.size();i++){
            sterne[i]->setGravity(true);
        }
    }

    if(msg.getAddress() == "/EmotionWorld/toggle28")
    {
        float m=msg.getArgAsBool(0);

        for(int i=0;i<sterne.size();i++){
            sterne[i]->setGravity(m);
        }
    }
    
    
    if(msg.getAddress() == "/EmotionWorld/push26")
    {
        for(int i=0;i<sterne.size();i++){
            sterne[i]->setState(FADEOUT);
        }
    }
    
    if(msg.getAddress() == "/EmotionWorld/toggle23")
    {
        float m=msg.getArgAsBool(0);
        bindToSkeletton(m);
    }
    
    
    if(msg.getAddress() == "/EmotionWorld/toggle29")
    {
        float m=msg.getArgAsBool(0);
        bindToHands(m);
    }
    
    

    if(msg.getAddress() == "/EmotionWorld/toggle26")
    {
        float m=msg.getArgAsBool(0);
        bIsRepulsionActive=m;
    }

    if(msg.getAddress() == "/EmotionWorld/toggle27")
    {
        float m=msg.getArgAsBool(0);
        bIsEllipseAutomated=m;
    }
    
}



