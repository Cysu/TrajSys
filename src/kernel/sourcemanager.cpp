#include "sourcemanager.h"

using std::string;
using cv::Mat;

SourceManager::SourceManager(const string& source)
{
    frameIO = new FrameIO(source.c_str());
}

void SourceManager::release()
{
    delete frameIO;
}

int SourceManager::getNextFrame(Mat& frame)
{
    return frameIO->readNextFrame(frame);
}

int SourceManager::getNrFrame()
{
    return frameIO->getNrFrame();
}

