#include "kinectUIApp.h"

//--------------------------------------------------------------
void kinectUIApp::setup(){
    gui = new ofxUICanvas(0,0,320,320);
    gui->addWidgetDown(new ofxUILabel("OFXUI TUTORIAL", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUIFPSSlider(304, 16, 0.0, 1000.0 , 0, "FPS"));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,"BACKGROUND VALUE"));
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    ofAddListener(gui->newGUIEvent, this, &kinectUIApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------
void kinectUIApp::update(){

}

void kinectUIApp::guiEvent(ofxUIEventArgs& ev) {
    if (ev.widget->getName() == "BACKGROUND VALUE") {
        ofxUISlider *slider = (ofxUISlider *) ev.widget;
        ofBackground(slider->getScaledValue());
    }
    else if (ev.widget->getName() == "FULLSCREEN") {
        ofxUIToggle *toggle = (ofxUIToggle *) ev.widget;
        ofSetFullscreen(toggle->getValue());
    }
}

//--------------------------------------------------------------
void kinectUIApp::draw(){

}

//--------------------------------------------------------------
void kinectUIApp::keyPressed(int key){

}

//--------------------------------------------------------------
void kinectUIApp::keyReleased(int key){

}

//--------------------------------------------------------------
void kinectUIApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void kinectUIApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void kinectUIApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void kinectUIApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void kinectUIApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void kinectUIApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void kinectUIApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void kinectUIApp::exit() {
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}
