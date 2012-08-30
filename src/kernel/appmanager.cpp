#include "appmanager.h"

AppManager::AppManager(const QString &type, const QString &params,
                       QObject *parent) :
    QObject(parent),
    type(type)
{
    if (type == "Coherent Filter") {
        coherentFilter = new CoherentFilter(params);
    }
}

void AppManager::displayResult(const QString &windowName, const TrackSet &trackSet, cv::Mat &img)
{
    if (type == "Coherent Filter") {
        coherentFilter->displayResult(windowName, trackSet, img);
    }
}

void AppManager::saveResult(const TrackSet &trackSet, const QString &ifName, const QString &ofName)
{
    if (type == "Coherent Filter") {
        coherentFilter->saveResult(trackSet, ifName, ofName+".cls");
    }
}
