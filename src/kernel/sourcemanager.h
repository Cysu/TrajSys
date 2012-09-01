#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

#include <QObject>
#include <QString>

#include "utils/frameio.h"
#include "opencv2/opencv.hpp"

class SourceManager : public QObject
{
    Q_OBJECT

public:
    explicit SourceManager(const QString &source, QObject *parent = 0);
    void release();

    int getNextFrame(cv::Mat &frame);
    
private:
    FrameIO *frameIO;
};

#endif // SOURCEMANAGER_H
