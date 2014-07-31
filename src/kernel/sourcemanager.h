#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

#include "utils/frameio.h"
#include <string>
#include <opencv2/opencv.hpp>

class SourceManager
{
public:
    explicit SourceManager(const std::string& source);
    void release();

    int getNrFrame();

    int getNextFrame(cv::Mat& frame);
    
private:
    FrameIO* frameIO;
};

#endif // SOURCEMANAGER_H
