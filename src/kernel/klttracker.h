#ifndef KLTTRACKER_H
#define KLTTRACKER_H

#include "utils/utils.h"
#include "utils/klt/klt.h"
#include <opencv2/opencv.hpp>


class KltTracker
{
public:
    explicit KltTracker(const std::string& params);

    void recordBgFrame(const cv::Mat& frame);
    void getTrackPoints(const cv::Mat& frame, TrackPoint* trackPoints);

    void release();

    int nrFeature, fgThres, stThres;
    
private:
    KLT_TrackingContext tc;
    KLT_FeatureList fl;

    cv::Mat bgFrame, prevFrame, cntFrame;

    int frameIdx;

    TrackPoint* bufTrackPoints;

    void parseParams(const std::string& params);
    bool isStableTrack(int idx);
};

#endif // KLTTRACKER_H
