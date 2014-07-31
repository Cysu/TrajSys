#include "appmanager.h"

using std::string;
using cv::Mat;

AppManager::AppManager(const string& type, const string& params,
                       int nrFeature, const string& srcPath,
                       const string& ofPath, bool needDisplay)
{
    coherentFilter = NULL;
    clusterPoints = NULL;
    if (type == "Coherent Filter") {
        coherentFilter = new CoherentFilter(params, nrFeature);
        clusterPoints = new ClusterPoint[nrFeature];
    }

    clusterIO = NULL;
    if (ofPath != "") {
        clusterIO = new ClusterIO;
        clusterIO->setOutput(ofPath);

        clusterIO->writeInfo(srcPath, nrFeature);
    }

    clusterDisplayer = NULL;
    if (needDisplay) {
        clusterDisplayer = new ClusterDisplayer(nrFeature);
    }
}

void AppManager::release()
{
    if (coherentFilter != NULL) {
        coherentFilter->release();
        delete coherentFilter;
        coherentFilter = NULL;
        delete[] clusterPoints;
        clusterPoints = NULL;
    }

    if (clusterIO != NULL) {
        clusterIO->closeInput();
        clusterIO->closeOutput();
        delete clusterIO;
        clusterIO = NULL;
    }

    if (clusterDisplayer != NULL) {
        clusterDisplayer->release();
        delete clusterDisplayer;
        clusterDisplayer = NULL;
    }
}

void AppManager::getResult(const Mat& frame, TrackPoint* trackPoints)
{
    if (coherentFilter != NULL) {
        coherentFilter->getClusterPoints(trackPoints, clusterPoints);
    }

    if (clusterIO != NULL) {
        clusterIO->writeFrame(clusterPoints);
    }

    if (clusterDisplayer != NULL) {
        clusterDisplayer->dispFrame(frame, clusterPoints);
    }
}
