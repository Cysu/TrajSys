#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>

#include <opencv2/opencv.hpp>

#include "klttracker.h"
#include "utils/utils.h"
#include "utils/trackio.h"

class TrackerManager : public QObject
{
    Q_OBJECT

public:
    explicit TrackerManager(const QString &type, const QString &params,
                            const QString &srcPath, const QString &ofPath,
                            QObject *parent = 0);
    explicit TrackerManager(const QString &ifPath,
                            QObject *parent = 0);

    void recordBgFrame(const cv::Mat &frame);
    void getTrackPoints(const cv::Mat &frame, TrackPoint *trackPoints);

    int getNrFeature();
    int getFgThres();

    void release();

private:
    TrackIO *trackIO;

    KltTracker *kltTracker;
};

#endif // TRACKERMANAGER_H
