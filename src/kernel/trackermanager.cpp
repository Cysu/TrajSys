#include "trackermanager.h"
#include "klttracker.h"

TrackerManager::TrackerManager(QFileInfoList *files,
                               const QString &type, const QString &params,
                               TrackSet *trackSet,
                               const QString &outputFileName,
                               QObject *parent) :
    QObject(parent),
    files(files),
    type(type),
    params(params),
    trackSet(trackSet),
    outputFileName(outputFileName)
{

}

void TrackerManager::work()
{
    if (type == "KLT") {
        KltTracker *kltTracker = new KltTracker(
                    files, params,
                    trackSet,
                    outputFileName+"("+params+").trk");
        kltTracker->run();
    }
}
