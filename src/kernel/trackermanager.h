#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>

#include <opencv2/opencv.hpp>

#include "kernelutils.h"
#include "klttracker.h"

class TrackerManager : public QObject
{
    Q_OBJECT

public:
    explicit TrackerManager(const QString &type, const QString &params,
                            QObject *parent = 0);

    void getFromFiles(const QFileInfoList &files,
                      TrackSet *trackSet,
                      const QString &ofName = "");

    void getFromImage(const cv::Mat &img,
                      TrackSet *trackSet);

    void finish();

private:
    QString type;

    KltTracker *kltTracker;
};

#endif // TRACKERMANAGER_H
