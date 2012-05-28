#include "kinectUIApp.h"

//--------------------------------------------------------------
void kinectUIApp::setup(){
    setupUIApp();
}

//--------------------------------------------------------------
void kinectUIApp::update(){
    gui_fps_graph->addPoint(ofGetFrameRate());
}

void kinectUIApp::setupUIApp() {
    float max_fps = 1000;
    ui_app = new ofxUICanvas(0,0,320,320);
    ui_app->addWidgetDown(new ofxUILabel("KinectUI", OFX_UI_FONT_LARGE));

    ui_app->addWidgetDown(new ofxUIFPSSlider(304, 16, 0.0, max_fps, 0, "FPS"));
    vector<float> buffer;
    for(int i = 0; i < 256; i++) {
        buffer.push_back(0.0);
    }
    gui_fps_graph = (ofxUIMovingGraph*) ui_app->addWidgetDown(
            new ofxUIMovingGraph(304, 64, buffer, buffer.size(), 0, max_fps, "FPS Graph"));

    ui_app->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,"BACKGROUND VALUE"));
    ui_app->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    ofAddListener(ui_app->newGUIEvent, this, &kinectUIApp::guiEvent);
    ui_app->loadSettings("GUI/guiSettings.xml");
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
    ui_app->saveSettings("GUI/guiSettings.xml");
    delete ui_app;
}
