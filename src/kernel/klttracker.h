#ifndef KLTTRACKER_H
#define KLTTRACKER_H

#include <QObject>
#include <QString>
#include <QStringList>

#include <opencv2/opencv.hpp>

#include "utils/utils.h"
#include "utils/klt/klt.h"

class KltTracker : public QObject
{
    Q_OBJECT

public:
    explicit KltTracker(const QString &params, QObject *parent = 0);

    void recordBgFrame(const cv::Mat &frame);
    void getTrackPoints(const cv::Mat &frame, TrackPoint *trackPoints);

    void release();

    int nrFeature, fgThres, stThres;
    
private:
    KLT_TrackingContext tc;
    KLT_FeatureList fl;

    cv::Mat bgFrame, prevFrame, cntFrame;

    int frameIdx;

    TrackPoint *bufTrackPoints;

    void parseParams(const QString &params);
    bool isStableTrack(int idx);
};

#endif // KLTTRACKER_H
