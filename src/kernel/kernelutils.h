#ifndef KERNELUTILS_H
#define KERNELUTILS_H

#include <QVector>

#define SQR(x) ((x)*(x))

inline void swap(int &a, int &b) { int t = a; a = b; b = t; }

struct TrackPoint
{
    int t, x, y;
};
typedef QVector<TrackPoint> Track;
typedef QVector<Track> TrackSet;

struct ClusterPoint
{
    int l, x, y;
};
typedef QVector<ClusterPoint> Cluster;
typedef QVector<Cluster> ClusterSet;

#endif // KERNELUTILS_H
