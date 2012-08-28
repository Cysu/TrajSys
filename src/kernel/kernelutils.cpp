#include <cstdio>

#include "kernelutils.h"

void printTrack(FILE *fid, const Track &track)
{
    fprintf(fid, "%d ", track.size());
    for (int k = 0; k < track.size(); k ++)
        fprintf(fid, "(%d,%d,%d)", track[k].x, track[k].y, track[k].t);
    fprintf(fid, "\n");
}

