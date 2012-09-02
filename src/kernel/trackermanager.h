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
#include "utils/trackdisplayer.h"

class TrackerManager : public QObject
{
    Q_OBJECT

public:
    explicit TrackerManager(const QString &type, const QString &params,
                            const QString &srcPath, const QString &ofPath,
                            const bool &needDisplay,
                            QObject *parent = 0);

    void recordBgFrame(const cv::Mat &frame);
    void getTrackPoints(const cv::Mat &frame, TrackPoint *trackPoints);

    int getNrFeature();
    int getFgThres();

    void release();

    QString srcPath;

private:
    TrackIO *trackIO;
    TrackDisplayer *trackDisplayer;

    int nrFeature, fgThres, stThres;

    KltTracker *kltTracker;
};

#endif // TRACKERMANAGER_H
