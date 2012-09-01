#ifndef KLTTRACKER_H
#define KLTTRACKER_H

#include <QObject>
#include <QFileInfoList>
#include <QString>
#include <QStringList>

#include <opencv2/opencv.hpp>

#include "kernelutils.h"
#include "utils/klt/klt.h"
#include "trackio.h"

class KltTracker : public QObject
{
    Q_OBJECT

public:
    explicit KltTracker(const QString &params, QObject *parent = 0);

    // Offline mode.
    void getFromFiles(const QFileInfoList &files,
                      TrackSet *trackSet,
                      const QString &ofName);

    // Online mode.
    void getFromImage(const cv::Mat &img,
                      TrackSet *trackSet, bool *(&mark));

    void finish();
    
private:
    int nrFeatures, fgThres;

    KLT_TrackingContext tc;
    KLT_FeatureList fl;

    Track *tracks;
    bool *isForeground;

    cv::Mat bgFrame, prevFrame, cntFrame;
    TrackIO trackIO;

    int frameIdx;

    void parseParams(const QString &params);
    void addTrackPoint(int idx, int t, int x, int y);
    static bool isStableTrack(const Track &track);
};

#endif // KLTTRACKER_H
