#include "appmanager.h"
#include "coherentfilter.h"

AppManager::AppManager(QFileInfoList *files, TrackSet *trackSet,
                       const QString &type, const QString &params,
                       const QString &outputFileName, QObject *parent) :
    QObject(parent),
    files(files),
    trackSet(trackSet),
    type(type),
    params(params),
    outputFileName(outputFileName)
{
}

void AppManager::work()
{
    if (type == "Coherent Filter") {
        CoherentFilter *coherentFilter = new CoherentFilter(
                    files, trackSet, params,
                    outputFileName+"("+params+").cls");
        coherentFilter->run();
    }

}
