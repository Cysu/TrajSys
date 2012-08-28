#include "trackermanager.h"
#include "kernelutils.h"
#include "klttracker.h"

TrackerManager::TrackerManager(const QFileInfoList &files,
                               const QString &type, const QString &params,
                               const QString &outputFileName,
                               QObject *parent) :
    QObject(parent),
    files(files),
    type(type),
    params(params),
    outputFileName(outputFileName)
{

}

void TrackerManager::work()
{
    if (type == "KLT") {
        KltTracker *kltTracker = new KltTracker(
                    files, params, outputFileName+"("+params+").trk");
        kltTracker->run();
    }
}
