#include "sourcemanager.h"

SourceManager::SourceManager(const QString &source, QObject *parent) :
    QObject(parent)
{
    frameIO = new FrameIO(source);
}

void SourceManager::release()
{
    delete frameIO;
}

int SourceManager::getNextFrame(cv::Mat &frame)
{
    return frameIO->readNextFrame(frame);
}

int SourceManager::getNrFrame()
{
    return frameIO->getNrFrame();
}

