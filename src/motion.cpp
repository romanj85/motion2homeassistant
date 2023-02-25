/*
* To compile: g++ opencv_test2.cpp -o opencv_test2 $(pkg-config --cflags --libs opencv)

Dependencies: apt install libopencv-dev

g++ -g opencv_test.cpp -o opencv_test -I /usr/include/opencv4 \
    -L/usr/lib/aarch64-linux-gnu -lopencv_videoio -lopencv_core -lopencv_imgproc -lopencv_highgui 

g++ -std=c++17 -o app app.cpp /path/to/libredis++.a /path/to/libhiredis.a -pthread

*/

#include "motion.h"
#include "rest.h"

#include <thread>
#include <chrono>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

void MotionDetect::saveImage(cv::Mat toSave, std::string file) {
    std::time_t time = std::time({});
    char timeString[std::size("yyyy-mm-dd hh:mm:ss")+1];
    std::strftime(std::data(timeString), std::size(timeString), "%F %T", std::localtime(&time));
    putText(toSave, timeString, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
    imwrite(file, toSave);
    //imshow("saved", toSave);
}

int MotionDetect::run(MotionRestCall& rest) {
    Mat frame, gray, frameDelta, thresh, previousFrame;
    vector<vector<Point> > cnts;
    VideoCapture camera(0);

    camera.set(3, 1280);
    camera.set(4, 720);

    camera.read(frame);
    cvtColor(frame, previousFrame, COLOR_BGR2GRAY);
    GaussianBlur(previousFrame, previousFrame, Size(21, 21), 0);
    std::this_thread::sleep_for(1s);

    while(camera.read(frame)) {
        //saveImage(frame.clone(), "/tmp/last.jpg");

        // convert to grayscale and set the first frame
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(21, 21), 0);

        // compute difference between first frame and current frame
        absdiff(previousFrame, gray, frameDelta);
        previousFrame = gray.clone();
        //imshow("Delta", frameDelta);

        // find the contours
        threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
        dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
        findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        bool motionDetected{false};

        for(int i = 0; i< cnts.size(); i++) {
            if(contourArea(cnts[i]) < 500) {
                continue;
            }
            // may be several times / depends on contours
            motionDetected = true;
            //putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
            break;
        }
        //imshow("Camera", frame);

        if (!motionDetected || !rest.notify()) {
            std::this_thread::sleep_for(1s);
        }
        
        if(waitKey(1) == 27){ //exit if ESC is pressed
            break;
        }
    }

    return 0;
}
