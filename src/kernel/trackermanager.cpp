#include "trackermanager.h"

TrackerManager::TrackerManager(const QString &type, const QString &params,
                               const QString &srcPath, const QString &ofPath,
                               QObject *parent) :
    QObject(parent)
{
    kltTracker = NULL;
    if (type == "KLT") {
        kltTracker = new KltTracker(params);
    }

    trackIO = NULL;
    if (ofPath != "") {
        trackIO = new TrackIO;
        trackIO->setOutput(ofPath);

        trackIO->writeInfo(srcPath, kltTracker->nrFeature);
    }
}

TrackerManager::TrackerManager(const QString &ifPath,
                               QObject *parent) :
    QObject(parent)
{
    trackIO = new TrackIO;
    trackIO->setInput(ifPath);
}

void TrackerManager::recordBgFrame(const cv::Mat &frame)
{
    if (kltTracker != NULL) {
        kltTracker->recordBgFrame(frame);
    }
}

void TrackerManager::getTrackPoints(const cv::Mat &frame, TrackPoint *trackPoints)
{
    if (kltTracker != NULL) {
        kltTracker->getTrackPoints(frame, trackPoints);
    }

    if (trackIO != NULL) {
        trackIO->writeFrame(trackPoints);
    }
}

int TrackerManager::getNrFeature()
{
    if (kltTracker != NULL) return kltTracker->nrFeature;
    return -1;
}

int TrackerManager::getFgThres()
{
    if (kltTracker != NULL) return kltTracker->fgThres;
    return -1;
}


void TrackerManager::release()
{
    if (kltTracker != NULL) {
        kltTracker->release();
        delete kltTracker;
        kltTracker = NULL;
    }

    if (trackIO != NULL) {
        trackIO->closeInput();
        trackIO->closeOutput();
        delete trackIO;
        trackIO = NULL;
    }
}
