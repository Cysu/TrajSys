#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>

#include <opencv2/opencv.hpp>

#include "coherentfilter.h"
#include "kernelutils.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(const QString &type, const QString &params,
                        QObject *parent = 0);

    void displayResult(const QString &windowName, const TrackSet &trackSet, cv::Mat &img);
    void saveResult(const TrackSet &trackSet, const QString &ifName, const QString &ofName);

private:
    QString type;

    CoherentFilter *coherentFilter;
};

#endif // APPMANAGER_H
