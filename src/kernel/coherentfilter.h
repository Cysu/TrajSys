#ifndef COHERENTFILTER_H
#define COHERENTFILTER_H

#include <QObject>
#include <QString>
#include <QStringList>

#include <opencv2/opencv.hpp>

#include "utils/utils.h"

class CoherentFilter : public QObject
{
    Q_OBJECT

public:
    explicit CoherentFilter(const QString &params, const int &nrFeature,
                            QObject *parent = 0);

    void getClusterPoints(TrackPoint *trackPoints, ClusterPoint *clusterPoints);

    void release();
    
private:
    int period, nrNeighbor;
    double vThres;
    int nrFeature;

    int frameIdx;

    TrackPoint *bufTrackPoints;
    ClusterPoint *bufClusterPoints;

    int *knn;
    int *dist;
    int *idx;
    int *father;
    int *vote;
    bool *conn;
    int *length;
    int *tot;

    void parseParams(const QString &params);
    void knnSort(int *dist, int *idx, int l, int r);
    void unionJoin(int *father, int i, int j);
    int unionGetRoot(int *father, int i);
};

#endif // COHERENTFILTER_H
