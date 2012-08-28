#ifndef KERNELUTILS_H
#define KERNELUTILS_H

#include <QVector>

#define SQR(x) ((x)*(x))

struct TrackPoint
{
    int t, x, y;
};
typedef QVector<TrackPoint> Track;

void printTrack(FILE *fid, const Track &track);

#endif // KERNELUTILS_H
