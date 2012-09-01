#ifndef UTILS_H
#define UTILS_H

#define SQR(x) ((x)*(x))

inline void swap(int &a, int &b) { int t = a; a = b; b = t; }

struct TrackPoint
{
    int flag;
    int x, y;
};

struct ClusterPoint
{
    int flag;
    int x, y;
    int rawLabel, ascLabel;
};

const int FLAG_IS_FOREGROUND = 1;
const int FLAG_NEW_TRACKPOINT = 2;

#endif // UTILS_H
