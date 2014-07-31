#include "coherentfilter.h"

#include <set>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define KNN(t, i, k) knn[(t)*nrFeature*nrNeighbor + (i)*nrNeighbor + (k)]
#define BUFTP(t, i) bufTrackPoints[(t)*nrFeature + i]
#define VOTE(r, l) vote[(r)*nrFeature + (l)]
#define CONN(i, j) conn[(i)*nrFeature + (j)]

using std::string;
using std::vector;
using std::set;

CoherentFilter::CoherentFilter(const string& params, int nrFeature)
    : nrFeature(nrFeature)
{
    parseParams(params);

    frameIdx = 0;

    knn = new int[(period+1) * nrFeature * nrNeighbor];
    dist = new int[nrFeature];
    idx = new int[nrFeature];

    length = new int[nrFeature];
    tot = new int[nrFeature];

    father = new int[nrFeature];
    vote = new int[nrFeature * nrFeature];
    conn = new bool[nrFeature * nrFeature];

    bufTrackPoints = new TrackPoint[(period+1) * nrFeature];
    bufClusterPoints = new ClusterPoint[nrFeature];
}

void CoherentFilter::release()
{
    delete[] knn;
    delete[] dist;
    delete[] idx;
    delete[] length;
    delete[] tot;
    delete[] father;
    delete[] vote;
    delete[] conn;
    delete[] bufTrackPoints;
    delete[] bufClusterPoints;

    knn = NULL;
    dist = NULL;
    idx = NULL;
    length = NULL;
    tot = NULL;
    father = NULL;
    vote = NULL;
    conn = NULL;
    bufTrackPoints = NULL;
    bufClusterPoints = NULL;

    frameIdx = 0;
}

void CoherentFilter::parseParams(const string& params)
{
    vector<string> paramList = split(params, ',');
    period = atoi(paramList[0].c_str());
    nrNeighbor = atoi(paramList[1].c_str());
    vThres = atoi(paramList[2].c_str());
}

void CoherentFilter::getClusterPoints(TrackPoint* trackPoints,
                                      ClusterPoint* clusterPoints)
{
    int cntTid = frameIdx % (period+1);

    // First copy the trackpoints to buffer.
    for (int i = 0; i < nrFeature; i ++) BUFTP(cntTid, i) = trackPoints[i];


    // Compute the length of each track.
    if (frameIdx == 0) {
        for (int i = 0; i < nrFeature; i ++) length[i] = 1;
    } else {
        for (int i = 0; i < nrFeature; i ++) {
            if (trackPoints[i].flag & FLAG_NEW_TRACKPOINT) length[i] = 1;
            else length[i] ++;
        }
    }

    // Compute the KNN of this frame.
    for (int i = 0; i < nrFeature; i ++) {
        if (!(trackPoints[i].flag & FLAG_IS_FOREGROUND)) continue;
        TrackPoint &u = trackPoints[i];
        for (int j = 0; j < nrFeature; j ++) {
            idx[j] = j;
            dist[j] = 0x7fffffff;
            if (i == j || !(trackPoints[j].flag & FLAG_IS_FOREGROUND)) continue;
            TrackPoint &v = trackPoints[j];
            dist[j] = SQR(u.x-v.x) + SQR(u.y-v.y);
        }
        knnSort(dist, idx, 0, nrFeature-1);
        for (int k = 0; k < nrNeighbor; k ++) KNN(cntTid, i, k) = idx[k];
    }

    if (frameIdx < period) {
        frameIdx ++;

        for (int i = 0; i < nrFeature; i ++) {
            ClusterPoint clusterPoint;
            clusterPoint.x = trackPoints[i].x;
            clusterPoint.y = trackPoints[i].y;
            clusterPoint.flag = trackPoints[i].flag;
            clusterPoint.rawLabel = clusterPoint.ascLabel = -1;
            clusterPoints[i] = clusterPoint;
        }

        return;
    }

    // Find coherent neighbors.
    memset(conn, false, nrFeature*nrFeature*sizeof(bool));
    for (int i = 0; i < nrFeature; i ++) {
        if (length[i] < period+1) continue;
        if (!(trackPoints[i].flag & FLAG_IS_FOREGROUND)) continue;

        for (int k = 0; k < nrNeighbor; k ++) {
            int j = KNN(cntTid, i, k);
            if (length[j] < period+1) continue;

            // Find invariant neighbor.
            bool isInvariant = true;
            for (int dt = 1; dt <= period; dt ++) {
                bool contain = false;
                int tid = (frameIdx-dt) % (period+1);
                for (int r = 0; r < nrNeighbor; r ++) {
                    if (KNN(tid, i, r) == j) {
                        contain = true;
                        break;
                    }
                }
                if (!contain) {
                    isInvariant = false;
                    break;
                }
            }
            if (!isInvariant) continue;

            // Compute the velocity coherence.
            double g = 0;
            for (int dt = 0; dt < period; dt ++) {
                int t1 = (frameIdx-dt) % (period+1);
                int t2 = (frameIdx-dt-1) % (period+1);
                TrackPoint u1 = BUFTP(t1, i);
                TrackPoint v1 = BUFTP(t1, j);
                TrackPoint u2 = BUFTP(t2, i);
                TrackPoint v2 = BUFTP(t2, j);
                int ux = u2.x-u1.x, uy = u2.y-u1.y;
                int vx = v2.x-v1.x, vy = v2.y-v1.y;
                int u = SQR(ux)+SQR(uy);
                int v = SQR(vx)+SQR(vy);
                if (u == 0 && v == 0)
                    g += 1.0;
                else if (u != 0 && v != 0)
                    g += (ux*vx+uy*vy) / sqrt(u*v);
                else
                    g += 0.5;
            }
            g /= period;
            if (g > vThres) CONN(i, j) = true;
        }
    }

    // Construct clusters.
    for (int i = 0; i < nrFeature; i ++) father[i] = i;
    for (int i = 0; i < nrFeature; i ++) {
        for (int j = i+1; j < nrFeature; j ++) {
            if (CONN(i, j) && CONN(j, i))
                unionJoin(father, i, j);
        }
    }

    for (int i = 0; i < nrFeature; i ++) {
        ClusterPoint clusterPoint;
        clusterPoint.x = trackPoints[i].x;
        clusterPoint.y = trackPoints[i].y;
        clusterPoint.flag = trackPoints[i].flag;
        if (length[i] < period+1 || !(trackPoints[i].flag & FLAG_IS_FOREGROUND))
            clusterPoint.rawLabel = -1;
        else
            clusterPoint.rawLabel = unionGetRoot(father, i);
        clusterPoint.ascLabel = -1;
        clusterPoints[i] = clusterPoint;
    }


    // Association.
    set<int> labels;
    for (int i = 0; i < nrFeature; i ++) {
        if (length[i] <= period+1) continue;
        if (!(trackPoints[i].flag & FLAG_IS_FOREGROUND)) continue;
        clusterPoints[i].ascLabel = bufClusterPoints[i].ascLabel;
        labels.insert(clusterPoints[i].ascLabel);
    }
    int validLabel = 0;
    for (int i = 0; i < nrFeature; i ++) {
        if (length[i] != period+1) continue;
        if (!(trackPoints[i].flag & FLAG_IS_FOREGROUND)) continue;
        while (labels.find(validLabel) != labels.end()) validLabel ++;
        clusterPoints[i].ascLabel = validLabel;
        labels.insert(validLabel);
    }

    set<int> roots;
    memset(vote, 0, nrFeature*nrFeature*sizeof(int));
    memset(tot, 0, nrFeature*sizeof(int));
    for (int i = 0; i < nrFeature; i ++) {
        if (length[i] < period+1) continue;
        if (!(trackPoints[i].flag & FLAG_IS_FOREGROUND)) continue;
        int r = unionGetRoot(father, i);
        roots.insert(r);
        tot[r] ++;
        VOTE(r, clusterPoints[i].ascLabel) ++;
    }

    for (set<int>::const_iterator it = roots.begin(); it != roots.end(); ++it) {
        int r = (*it);
        int maxVote = 0, maxLabel = -1;
        for (int i = 0; i < nrFeature; i ++) {
            if (length[i] < period+1) continue;
            if (!(trackPoints[i].flag & FLAG_IS_FOREGROUND)) continue;
            if (VOTE(r, clusterPoints[i].ascLabel) > maxVote) {
                maxVote = VOTE(r, clusterPoints[i].ascLabel);
                maxLabel = clusterPoints[i].ascLabel;
            }
        }
        clusterPoints[r].ascLabel = maxLabel;
    }

    for (int i = 0; i < nrFeature; i ++) {
        if (length[i] < period+1 || !(trackPoints[i].flag & FLAG_IS_FOREGROUND)) {
            clusterPoints[i].ascLabel = -1;
        } else {
            int r = unionGetRoot(father, i);
            clusterPoints[i].ascLabel = clusterPoints[r].ascLabel;
        }
    }

    // Copy down the cluster points of this frame.
    for (int i = 0; i < nrFeature; i ++) bufClusterPoints[i] = clusterPoints[i];

    frameIdx ++;
}

void CoherentFilter::knnSort(int* dist, int* idx, int l, int r)
{
    int i = l, j = r;
    int x = dist[(i+j)>>1];
    while (i < j) {
        while (dist[i] < x) i ++;
        while (x < dist[j]) j --;
        if (i <= j) {
            swap(dist[i], dist[j]);
            swap(idx[i], idx[j]);
            i ++;
            j --;
        }
    }
    if (l < j) knnSort(dist, idx, l, j);
    if (i < r) knnSort(dist, idx, i, r);
}

void CoherentFilter::unionJoin(int* father, int i, int j) {
    int ri = unionGetRoot(father, i);
    int rj = unionGetRoot(father, j);
    father[ri] = rj;
}

int CoherentFilter::unionGetRoot(int* father, int i) {
    if (father[i] == i) return i;
    father[i] = unionGetRoot(father, father[i]);
    return father[i];
}
