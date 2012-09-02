#ifndef TRACKDISPLAYER_H
#define TRACKDISPLAYER_H

#include <QObject>
#include <QString>

#include "utils.h"
#include "frameio.h"
#include "trackio.h"
#include "opencv2/opencv.hpp"

class TrackDisplayer : public QObject
{
    Q_OBJECT

public:
    explicit TrackDisplayer(const int &nrFeature, const int &trackLength, QObject *parent = 0);
    explicit TrackDisplayer(const QString &trkFile, QObject *parent = 0);

    void release();

    void dispFrame(const cv::Mat &frame, TrackPoint *trackPoints);
    void disp();
    
private:
    FrameIO *frameIO;
    TrackIO *trackIO;

    QString window;
    int trackLength;

    int nrFeature;
    int frameIdx;
    TrackPoint *bufTrackPoints;
};

#endif // TRACKDISPLAYER_H
