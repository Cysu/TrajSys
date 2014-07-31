#ifndef FRAMEIO_H
#define FRAMEIO_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class FrameIO
{
public:
    FrameIO(const std::string& source);
    ~FrameIO();
    void release();

    int getNrFrame();

    // Returns the frameIdx. Or -1 if no next frame.
    int readNextFrame(cv::Mat& frame);
    static bool writeFrame(const cv::Mat& frame, std::string& filePath);
    
private:
    std::vector<std::string> imgFiles;
    cv::VideoCapture* videoCapture;

    int frameIdx;
};

#endif // FRAMEIO_H
