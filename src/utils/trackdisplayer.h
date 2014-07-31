#ifndef TRACKDISPLAYER_H
#define TRACKDISPLAYER_H

#include "utils.h"
#include "frameio.h"
#include "trackio.h"
#include <opencv2/opencv.hpp>

class TrackDisplayer
{
public:
    TrackDisplayer(int nrFeature, int trackLength);
    explicit TrackDisplayer(const std::string& trkFile);

    void release();

    void dispFrame(const cv::Mat& frame, TrackPoint* trackPoints);
    void disp();
    
private:
    FrameIO* frameIO;
    TrackIO* trackIO;

    std::string window;
    int trackLength;

    int nrFeature;
    int frameIdx;
    TrackPoint* bufTrackPoints;
};

#endif // TRACKDISPLAYER_H
