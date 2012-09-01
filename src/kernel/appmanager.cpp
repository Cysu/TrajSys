#include "appmanager.h"

AppManager::AppManager(const QString &type, const QString &params,
                       const int &nrFeature,
                       const QString &srcPath, const QString &ofPath,
                       QObject *parent) :
    QObject(parent)
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
}

void AppManager::getResult(const cv::Mat &frame, TrackPoint *trackPoints)
{
    bool ready = false;
    if (coherentFilter != NULL) {
        ready = coherentFilter->getClusterPoints(trackPoints, clusterPoints);
    }

    if (clusterIO != NULL) {
        if (ready) clusterIO->writeFrame(clusterPoints);
        eles clusterIO->writeFrame(NULL);
    }
}
