#include "trackermanager.h"

TrackerManager::TrackerManager(const QString &type, const QString &params,
                               const QString &srcPath, const QString &ofPath,
                               const bool &needDisplay,
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
        trackIO->writeInfo(srcPath,
                           kltTracker->nrFeature, kltTracker->fgThres, kltTracker->stThres);
    } else if (type == "LOAD" && params != "") {
        trackIO = new TrackIO;
        trackIO->setInput(params);
        trackIO->readInfo(this->srcPath,
                          this->nrFeature, this->fgThres, this->stThres);
    }

    trackDisplayer = NULL;
    if (needDisplay) {
        if (type == "KLT") {
            trackDisplayer = new TrackDisplayer(kltTracker->nrFeature,
                                                kltTracker->stThres);
        } else if (type == "LOAD") {
            trackDisplayer = new TrackDisplayer(this->nrFeature,
                                                this->stThres);
        }
    }

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
    } else if (trackIO != NULL) {
        trackIO->readFrame(trackPoints);
    }

    if (kltTracker != NULL && trackIO != NULL) {
        trackIO->writeFrame(trackPoints);
    }

    if (trackDisplayer != NULL) {
        trackDisplayer->dispFrame(frame, trackPoints);
    }
}

int TrackerManager::getNrFeature()
{
    if (kltTracker != NULL) return kltTracker->nrFeature;
    else if (trackIO != NULL) return this->nrFeature;
    return -1;
}

int TrackerManager::getFgThres()
{
    if (kltTracker != NULL) return kltTracker->fgThres;
    else if (trackIO != NULL) return this->fgThres;
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

    if (trackDisplayer != NULL) {
        trackDisplayer->release();
        delete trackDisplayer;
        trackDisplayer = NULL;
    }
}
