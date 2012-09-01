#ifndef COHERENTFILTER_H
#define COHERENTFILTER_H

#include <QObject>
#include <QFileInfoList>
#include <QString>
#include <QStringList>

#include <opencv2/opencv.hpp>

#include "kernelutils.h"

class CoherentFilter : public QObject
{
    Q_OBJECT

public:
    explicit CoherentFilter(const QString &params,
                            QObject *parent = 0);

    void displayResult(const QString &windowName, const TrackSet &trackSet, bool *isForeground, cv::Mat &img);
    void saveResult(const TrackSet &trackSet, const QString &ifName, const QString &ofName);
    
private:
    // Parameters.
    int period, nrNeighbor;
    double vThres;

    int startFrame, endFrame, nrFrame;
    int nrFeature;

    int frameIdx;

    int *nrTrack;
    int *trackLists;
    int *knn;
    int *label;

    void parseParams(const QString &params);
    void computeTrackLists(const TrackSet &trackSet);
    void computeKNN(const TrackSet &trackSet);
    void KNNSort(int *dist, int *idx, int l, int r);
    TrackPoint getTrackPoint(const TrackSet &trackSet, int idx, int t);
    void unionJoin(int *father, int i, int j);
    int unionGetRoot(int *father, int i);    
};

#endif // COHERENTFILTER_H
