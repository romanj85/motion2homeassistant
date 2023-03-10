#pragma once

#include <string>

class MotionRestCall;
namespace cv {
    class Mat;
}
class MotionDetect {
    MotionDetect(MotionDetect&) = delete;
    MotionDetect(MotionDetect&&) = delete;

    void saveImage(cv::Mat frame, std::string file);
public:
    MotionDetect() = default;
    
    int run(MotionRestCall& rest);
};
