#include "kinectUIApp.h"

//--------------------------------------------------------------
void kinectUIApp::setup(){
    setupUI();
}

//--------------------------------------------------------------
void kinectUIApp::update(){
    gui_fps_graph->addPoint(ofGetFrameRate());
}

void kinectUIApp::setupUI() {
    float max_fps = 1000;
    gui = new ofxUICanvas(0,0,320,320);
    gui->addWidgetDown(new ofxUILabel("KinectUI", OFX_UI_FONT_LARGE));

    gui->addWidgetDown(new ofxUIFPSSlider(304, 16, 0.0, max_fps, 0, "FPS"));
    vector<float> buffer;
    for(int i = 0; i < 256; i++) {
        buffer.push_back(0.0);
    }
    gui_fps_graph = (ofxUIMovingGraph*) gui->addWidgetDown(
            new ofxUIMovingGraph(304, 64, buffer, buffer.size(), 0, max_fps, "FPS Graph"));

    gui->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,"BACKGROUND VALUE"));
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    ofAddListener(gui->newGUIEvent, this, &kinectUIApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
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
