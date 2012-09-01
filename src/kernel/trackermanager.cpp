#include "trackermanager.h"

TrackerManager::TrackerManager(const QString &type, const QString &params,
                               QObject *parent) :
    QObject(parent),
    type(type)
{
    if (type == "KLT") {
        kltTracker = new KltTracker(params);
    }
}

void TrackerManager::getFromFiles(const QFileInfoList &files,
                                  TrackSet *trackSet,
                                  const QString &ofName)
{
    if (type == "KLT") {
        kltTracker->getFromFiles(files, trackSet, ofName);
    }
}

void TrackerManager::getFromImage(const cv::Mat &img,
                                  TrackSet *trackSet, bool *(&mark))
{
    if (type == "KLT") {
        kltTracker->getFromImage(img, trackSet, mark);
    }
}

void TrackerManager::finish()
{
    if (type == "KLT") {
        kltTracker->finish();
        delete kltTracker;
        kltTracker = NULL;
    }
}
