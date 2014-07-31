#ifndef COHERENTFILTER_H
#define COHERENTFILTER_H

#include "utils/utils.h"
#include <opencv2/opencv.hpp>

class CoherentFilter
{
public:
    CoherentFilter(const std::string& params, int nrFeature);

    void getClusterPoints(TrackPoint* trackPoints, ClusterPoint* clusterPoints);

    void release();
    
private:
    int period, nrNeighbor;
    double vThres;
    int nrFeature;

    int frameIdx;

    TrackPoint* bufTrackPoints;
    ClusterPoint* bufClusterPoints;

    int* knn;
    int* dist;
    int* idx;
    int* father;
    int* vote;
    bool* conn;
    int* length;
    int* tot;

    void parseParams(const std::string& params);
    void knnSort(int* dist, int* idx, int l, int r);
    void unionJoin(int* father, int i, int j);
    int unionGetRoot(int* father, int i);
};

#endif // COHERENTFILTER_H
