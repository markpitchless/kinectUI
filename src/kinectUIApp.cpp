#include "kinectUIApp.h"

//--------------------------------------------------------------
void kinectUIApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);

    setupUIApp();

    setupKinect();
    setupUIKinect();
}

void kinectUIApp::setupKinect() {
	// enable depth->video image calibration
	kinect.setRegistration(true);

	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)

	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);

	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);

	// start from the front
	bDrawPointCloud = false;
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

void kinectUIApp::setupUIKinect() {
    ui_kinect = new ofxUICanvas(326,0,320,320);
    ui_kinect->addWidgetDown(new ofxUILabel("Kinect", OFX_UI_FONT_LARGE));

    ui_kinect->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,"Foo"));
    ui_kinect->addWidgetDown(new ofxUIToggle(32, 32, false, "Bar"));

    ofAddListener(ui_kinect->newGUIEvent, this, &kinectUIApp::guiEvent);
    ui_kinect->loadSettings("GUI/guiSettingsKinect.xml");
}

//--------------------------------------------------------------
void kinectUIApp::update(){
    gui_fps_graph->addPoint(ofGetFrameRate());

    kinect.update();
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {

        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThreshold, true);
            grayThreshFar.threshold(farThreshold);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {

            // or we do it ourselves - show people how they can work with the pixels
            unsigned char * pix = grayImage.getPixels();

            int numPixels = grayImage.getWidth() * grayImage.getHeight();
            for(int i = 0; i < numPixels; i++) {
                if(pix[i] < nearThreshold && pix[i] > farThreshold) {
                    pix[i] = 255;
                } else {
                    pix[i] = 0;
                }
            }
        }

        // update the cv images
        grayImage.flagImageChanged();

        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
    }
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
    ui_kinect->saveSettings("GUI/guiSettingsKinect.xml");
    delete ui_app;

    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}
