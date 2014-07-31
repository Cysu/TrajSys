#ifndef APPMANAGER_H
#define APPMANAGER_H

#include "coherentfilter.h"
#include "utils/utils.h"
#include "utils/clusterio.h"
#include "utils/clusterdisplayer.h"
#include <opencv2/opencv.hpp>

class AppManager
{
public:
    AppManager(const std::string& type, const std::string& params,
               int nrFeature, const std::string& srcPath,
               const std::string& ofPath, bool needDisplay);

    void getResult(const cv::Mat& frame, TrackPoint* trackPoints);

    void release();

    ClusterDisplayer* clusterDisplayer;

private:
    ClusterIO* clusterIO;

    CoherentFilter* coherentFilter;
    ClusterPoint* clusterPoints;
};

#endif // APPMANAGER_H
