#include "kinectUIApp.h"

//--------------------------------------------------------------
void kinectUIApp::setup(){
    ofSetWindowTitle("kinectUI");
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

    setupUIApp();

    flip_kinect = false;
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
	depthImage.allocate(kinect.width, kinect.height);
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
    int gw = 176;
    float max_fps = 1000;
    ui_app = new ofxUICanvas(10,10,gw+20,320);
    ui_app->addWidgetDown(new ofxUILabel("KinectUI", OFX_UI_FONT_LARGE));

    ui_app->addWidgetDown(new ofxUIFPSSlider(gw, 16, 0.0, max_fps, 0, "FPS"));
    vector<float> buffer;
    for(int i = 0; i < 256; i++) {
        buffer.push_back(0.0);
    }
    gui_fps_graph = (ofxUIMovingGraph*) ui_app->addWidgetDown(
            new ofxUIMovingGraph(gw, 64, buffer, buffer.size(), 0, max_fps, "FPS Graph"));

    ui_app->addWidgetDown(new ofxUISlider(gw,16,0.0,255.0,100.0,"BACKGROUND VALUE"));
    ui_app->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));

    ofAddListener(ui_app->newGUIEvent, this, &kinectUIApp::guiEvent);
    ui_app->loadSettings("GUI/guiSettings.xml");
}

void kinectUIApp::setupUIKinect() {
    int gw = 176;
    int gh = 20;
    ui_kinect = new ofxUICanvas(10+176+20+10,10,320,320);
    ui_kinect->addWidgetDown(new ofxUILabel("Kinect", OFX_UI_FONT_LARGE));

    string id_str = "ID: " + ofToString(kinect.getDeviceId()) + " " + kinect.getSerial();
    ui_device_id_label = (ofxUILabel*) ui_kinect->addWidgetDown(
            new ofxUILabel(id_str, OFX_UI_FONT_SMALL));

    ui_kinect->addWidgetDown(new ofxUISlider(gw, gh, -30, 30, angle, "Tilt"));
    // IDEA: Use getTargetCameraTitlAngle with a slider to show movement.
    ui_kinect->addWidgetDown(new ofxUIToggle(gh, gh, flip_kinect, "H Flip"));
    ui_kinect->addWidgetDown(new ofxUIToggle(gh, gh, kinect.isDepthNearValueWhite(), "Depth Near White"));

    ui_kinect->addWidgetDown(new ofxUIImage(160, 120, (ofImage*)&colorImg, "colorImg"));
    ui_kinect->addWidgetDown(new ofxUIImage(160, 120, (ofImage*)&depthImage, "depthImage"));
	ui_kinect->addWidgetDown(new ofxUIRangeSlider(gw, gh, 0.0, 255.0, nearThreshold, farThreshold, "Threshold"));
    ui_kinect->addWidgetDown(new ofxUIImage(160, 120, (ofImage*)&grayImage, "grayImage"));

    ui_kinect->autoSizeToFitWidgets();
    ofAddListener(ui_kinect->newGUIEvent, this, &kinectUIApp::guiEvent);
    ui_kinect->loadSettings("GUI/guiSettingsKinect.xml");
}


//--------------------------------------------------------------
void kinectUIApp::setKinectAngle(int n_angle) {
    if (n_angle>30)  n_angle=30;
    if (n_angle<-30) n_angle=-30;
    angle = n_angle;
    kinect.setCameraTiltAngle(angle);
}

void kinectUIApp::setNearThreshold(int n) {
    cout << "near: " << n << endl;
    if (n>255) n=255;
    if (n<0)   n=0;
    nearThreshold = n;
}

void kinectUIApp::setFarThreshold(int n) {
    cout << "far: " << n << endl;
    if (n>255) n=255;
    if (n<0)   n=0;
    farThreshold = n;
}


//--------------------------------------------------------------
void kinectUIApp::update(){
    gui_fps_graph->addPoint(ofGetFrameRate());

    kinect.update();
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        // load the rgb image
        colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        if (flip_kinect)
            colorImg.mirror(false,true);

        // load grayscale depth image from the kinect source
        depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        if (flip_kinect)
            depthImage.mirror(false,true);
        grayImage = depthImage;

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
    string name = ev.widget->getName();
    if (ev.widget->getName() == "BACKGROUND VALUE") {
        ofxUISlider *slider = (ofxUISlider *) ev.widget;
        ofBackground(slider->getScaledValue());
    }
    else if (ev.widget->getName() == "FULLSCREEN") {
        ofxUIToggle *toggle = (ofxUIToggle *) ev.widget;
        ofSetFullscreen(toggle->getValue());
    }
    else if (ev.widget->getName() == "Tilt") {
        ofxUISlider* slider = (ofxUISlider*) ev.widget;
        setKinectAngle(slider->getScaledValue());
    }
    else if (ev.widget->getName() == "Threshold") {
        ofxUIRangeSlider* slider = (ofxUIRangeSlider*) ev.widget;
        setFarThreshold(slider->getScaledValueLow());
        setNearThreshold(slider->getScaledValueHigh());
    }
    else if(name == "Depth Near White") {
        ofxUIButton *button = (ofxUIButton *) ev.widget;
        kinect.enableDepthNearValueWhite(button->getValue());
    }
    else if(name == "H Flip") {
        ofxUIButton *button = (ofxUIButton *) ev.widget;
        flip_kinect = button->getValue();
    }
}

//--------------------------------------------------------------
void kinectUIApp::draw(){
    easyCam.begin();
    drawPointCloud();
    easyCam.end();
}

void kinectUIApp::drawPointCloud() {
    int w = 640;
    int h = 480;
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 2;
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            if(kinect.getDistanceAt(x, y) > 0) {
                mesh.addColor(kinect.getColorAt(x,y));
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(3);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points a bit
    glEnable(GL_DEPTH_TEST);
    mesh.drawVertices();
    glDisable(GL_DEPTH_TEST);
    ofPopMatrix();
}


//--------------------------------------------------------------
void kinectUIApp::keyPressed(int key){
    switch(key) {
        case '\t':
            ui_app->toggleVisible();
            ui_kinect->toggleVisible();
        break;
    }
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


