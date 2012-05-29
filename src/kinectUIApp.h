#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"


class kinectUIApp : public ofBaseApp {

    public:
        void setup();
        void update();
        void draw();

        void keyPressed  (int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void exit();

        void setupKinect();
        void setupUIApp();
        void setupUIKinect();
        void guiEvent(ofxUIEventArgs &ev);

        ofxUICanvas *ui_app;
        ofxUICanvas *ui_kinect;
        ofxUIMovingGraph* gui_fps_graph;
        ofxUILabel* ui_device_id_label;

        void setKinectAngle( int n_angle );
        void setNearThreshold( int n );
        void setFarThreshold( int n);
        void drawPointCloud();
        ofxKinect kinect;
        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage depthImage;// grayscale depth image
        ofxCvGrayscaleImage grayImage; // grayscale depth image after threshold
        ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
        ofxCvGrayscaleImage grayThreshFar;  // the far thresholded image
        ofxCvContourFinder contourFinder;
        bool bThreshWithOpenCV;
        bool bDrawPointCloud;
        int nearThreshold;
        int farThreshold;
        int angle;
        // used for viewing the point cloud
        ofEasyCam easyCam;
};
