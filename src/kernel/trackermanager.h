#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include "klttracker.h"
#include "utils/utils.h"
#include "utils/trackio.h"
#include "utils/trackdisplayer.h"
#include <opencv2/opencv.hpp>


class TrackerManager
{
public:
    TrackerManager(const std::string& type, const std::string& params,
                   const std::string& srcPath, const std::string& ofPath,
                   bool needDisplay);

    void recordBgFrame(const cv::Mat& frame);
    void getTrackPoints(const cv::Mat& frame, TrackPoint* trackPoints);

    int getNrFeature();
    int getFgThres();

    void release();

    std::string srcPath;

private:
    TrackIO* trackIO;
    TrackDisplayer* trackDisplayer;

    int nrFeature, fgThres, stThres;

    KltTracker* kltTracker;
};

#endif // TRACKERMANAGER_H
