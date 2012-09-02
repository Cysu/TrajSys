#ifndef FRAMEIO_H
#define FRAMEIO_H

#include <QString>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>

#include "opencv2/opencv.hpp"

class FrameIO
{
public:
    FrameIO(const QString &source);
    ~FrameIO();
    void release();

    int getNrFrame();

    // Returns the frameIdx. Or -1 if no next frame.
    int readNextFrame(cv::Mat &frame);
    static bool writeFrame(const cv::Mat &frame, QString &filePath);
    
private:
    QFileInfoList *imgFiles;
    cv::VideoCapture *videoCapture;

    int frameIdx;
};

#endif // FRAMEIO_H
