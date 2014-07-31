#ifndef CLUSTERDISPLAYER_H
#define CLUSTERDISPLAYER_H

#include "utils.h"
#include "frameio.h"
#include "clusterio.h"
#include <opencv2/opencv.hpp>

class ClusterDisplayer
{
public:
    explicit ClusterDisplayer(int nrFeature);
    explicit ClusterDisplayer(const std::string& clsFile);

    void release();

    void dispFrame(const cv::Mat& frame, ClusterPoint* clusterPoints);
    void disp();

    bool ignoreSmallCluster;
    int mode;
    
private:
    FrameIO* frameIO;
    ClusterIO* clusterIO;

    std::string window;
    int nrFeature;

    int* tot;
};

#endif // CLUSTERDISPLAYER_H
