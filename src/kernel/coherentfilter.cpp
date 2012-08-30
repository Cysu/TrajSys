#include "coherentfilter.h"
#include "clusterio.h"

#include <QSet>
#include <cmath>

#define KNN(t, i, k) knn[(t)*nrFeature*nrNeighbor + (i)*nrNeighbor + (k)]
#define TRACKLISTS(t, i) trackLists[(t)*nrFeature + (i)]
#define LABEL(t, i) label[(t)*nrFeature + (i)]
#define VOTE(r, l) vote[(r)*nrFeature + (l)]
#define CONN(i, j) conn[(i)*nrFeature + (j)]

CoherentFilter::CoherentFilter(const QString &params,
                               QObject *parent) :
    QObject(parent)
{
    parseParams(params);
}

void CoherentFilter::saveResult(const TrackSet &trackSet,
                                const QString &ifName, const QString &ofName)
{
    // Initialize the output.
    ClusterIO clusterIO;
    clusterIO.setOutput(ofName);
    clusterIO.writePath(ifName);

    computeTrackLists(trackSet);
    computeKNN(trackSet);

    label = new int[nrFrame * nrFeature];
    memset(label, -1, nrFrame*nrFeature*sizeof(int));

    int *father = new int[nrFeature];
    int *vote = new int[nrFeature * nrFeature];
    bool *conn = new bool[nrFeature * nrFeature];

    for (int t = 0; t < nrFrame-period; t ++) {
        int time = startFrame+t;
        memset(conn, false, nrFeature*nrFeature*sizeof(bool));

        for (int i = 0; i < nrTrack[t]; i ++) {
            if (TRACKLISTS(t, i) == -1) continue;
            if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;

            for (int k = 0; k < nrNeighbor; k ++) {
                int j = KNN(t, i, k);

                // Find invariant neighbor.
                int nbIdx = TRACKLISTS(t, j);
                bool isInvariant = true;
                for (int dt = 1; dt <= period; dt ++) {
                    bool contain = false;
                    for (int r = 0; r < nrNeighbor; r ++) {
                        if (TRACKLISTS(t+dt, KNN(t+dt, i, r)) == nbIdx) {
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
                    TrackPoint u1 = getTrackPoint(trackSet, TRACKLISTS(t+dt, i), time+dt);
                    TrackPoint v1 = getTrackPoint(trackSet, TRACKLISTS(t+dt, j), time+dt);
                    TrackPoint u2 = getTrackPoint(trackSet, TRACKLISTS(t+dt+1, i), time+dt+1);
                    TrackPoint v2 = getTrackPoint(trackSet, TRACKLISTS(t+dt+1, j), time+dt+1);
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
        for (int i = 0; i < nrTrack[t]; i ++) father[i] = i;
        for (int i = 0; i < nrTrack[t]; i ++) {
            if (TRACKLISTS(t, i) == -1) continue;
            for (int j = i+1; j < nrTrack[t]; j ++) {
                if (TRACKLISTS(t, j) == -1) continue;
                if (CONN(i, j) && CONN(j, i))
                    unionJoin(father, i, j);
            }
        }

        if (t > 0) {
            QSet<int> labels;
            for (int i = 0; i < nrTrack[t]; i ++) {
                if (TRACKLISTS(t, i) == -1) continue;
                if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;
                if (TRACKLISTS(t, i) == TRACKLISTS(t-1, i)) {
                    LABEL(t, i) = LABEL(t-1, i);
                    labels.insert(LABEL(t, i));
                }
            }
            int validLabel = 0;
            for (int i = 0; i < nrTrack[t]; i ++) {
                if (TRACKLISTS(t, i) == -1) continue;
                if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;
                if (TRACKLISTS(t, i) != TRACKLISTS(t-1, i)) {
                    while (labels.find(validLabel) != labels.end()) validLabel ++;
                    LABEL(t, i) = validLabel;
                    labels.insert(validLabel);
                }
            }

            memset(vote, 0, nrFeature*nrFeature*sizeof(int));
            QSet<int> roots;
            for (int i = 0; i < nrTrack[t]; i ++) {
                if (TRACKLISTS(t, i) == -1) continue;
                if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;
                int ri = unionGetRoot(father, i);
                roots.insert(ri);
                VOTE(ri, LABEL(t, i)) ++;
            }

            roots.begin();

            foreach (int r, roots) {
                int maxVote = 0, maxLabel = -1;
                for (int i = 0; i < nrTrack[t]; i ++) {
                    if (TRACKLISTS(t, i) == -1 || LABEL(t, i) == -1) continue;
                    if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;
                    if (VOTE(r, LABEL(t, i)) > maxVote) {
                        maxVote = VOTE(r, LABEL(t, i));
                        maxLabel = LABEL(t, i);
                    }
                }
                LABEL(t, r) = maxLabel;
            }

            for (int i = 0; i < nrTrack[t]; i ++) {
                if (TRACKLISTS(t, i) == -1) continue;
                if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;
                int ri = unionGetRoot(father, i);
                LABEL(t, i) = LABEL(t, ri);
            }
        }

//        for (int i = 0; i < nrTrack[t]; i ++) {
//            if (TRACKLISTS(t, i) == -1) continue;
//            if (TRACKLISTS(t+period, i) != TRACKLISTS(t, i)) continue;
//            int ri = unionGetRoot(father, i);
//            LABEL(t, i) = ri;
//        }

        Cluster cluster;
        for (int i = 0; i < nrTrack[t]; i ++) {
            if (TRACKLISTS(t, i) == -1) continue;
            if (LABEL(t, i) == -1) continue;
            TrackPoint trackPoint = getTrackPoint(trackSet, TRACKLISTS(t, i), time);
            ClusterPoint clusterPoint = {
                LABEL(t, i),
                trackPoint.x,
                trackPoint.y
            };
            cluster.push_back(clusterPoint);
        }

        clusterIO.writeCluster(cluster);
    }

    clusterIO.closeOutput();

    delete[] father;
    delete[] vote;
    delete[] conn;
}

void CoherentFilter::parseParams(const QString &params)
{
    QStringList paramList = params.split(",");
    period = paramList.at(0).toInt();
    nrNeighbor = paramList.at(1).toInt();
    vThres = paramList.at(2).toDouble();
}

void CoherentFilter::displayResult(const QString &windowName, const TrackSet &trackSet, cv::Mat &img)
{

}

void CoherentFilter::computeTrackLists(const TrackSet &trackSet)
{
    startFrame = 0x7fffffff;
    endFrame = 0;
    for (int i = 0; i < trackSet.size(); i ++) {
        const Track &track = trackSet[i];
        startFrame = std::min(startFrame, track[0].t);
        endFrame = std::max(endFrame, track.back().t);
    }
    nrFrame = endFrame-startFrame+1;

    int *count = new int[nrFrame];
    memset(count, 0, nrFrame*sizeof(int));
    for (int i = 0; i < trackSet.size(); i ++) {
        const Track &track = trackSet[i];
        for (int t = track[0].t; t <= track.back().t; t ++)
            count[t-startFrame] ++;
    }

    nrFeature = 0;
    for (int t = 0; t < nrFrame; t ++)
        nrFeature = std::max(nrFeature, count[t]);

    delete[] count;


    nrTrack = new int[nrFrame];
    trackLists = new int[nrFrame * nrFeature];
    int *trackIdx = new int[trackSet.size()];
    memset(nrTrack, 0, nrFrame*sizeof(int));
    memset(trackLists, -1, nrFrame*nrFeature*sizeof(int));
    memset(trackIdx, -1,trackSet.size()*sizeof(int));

    for (int t = 0; t < nrFrame; t ++) {
        int time = startFrame+t;
        int idx = 0;
        for (int i = 0; i < trackSet.size(); i ++) {
            const Track &track = trackSet[i];
            if (track.back().t-track[0].t < period) continue;
            if (track[0].t <= time && track.back().t >= time) {
                if (trackIdx[i] == -1) {
                    // The track is not in previous frame.
                    while (TRACKLISTS(t, idx) != -1) idx++;
                    trackIdx[i] = idx;
                    TRACKLISTS(t, idx) = i;
                    nrTrack[t] = std::max(nrTrack[t], idx+1);
                } else{
                    // The track is in previous frame.
                    TRACKLISTS(t, trackIdx[i]) = i;
                    nrTrack[t] = std::max(nrTrack[t], trackIdx[i]+1);
                }
            } else {
                trackIdx[i] = -1;
            }
        }
    }

    delete[] trackIdx;
}

void CoherentFilter::computeKNN(const TrackSet &trackSet)
{
    knn = new int[nrFrame * nrFeature * nrNeighbor];
    int *dist = new int[nrFeature];
    int *idx = new int[nrFeature];

    for (int t = 0; t < nrFrame; t ++) {
        int time = startFrame+t;
        for (int i = 0; i < nrTrack[t]; i ++) {
            if (TRACKLISTS(t, i) == -1) continue;
            TrackPoint u = getTrackPoint(trackSet, TRACKLISTS(t, i), time);
            for (int j = 0; j < nrTrack[t]; j ++) {
                idx[j] = j;
                dist[j] = 0x7fffffff;
                if (TRACKLISTS(t, j) == -1 || i == j) continue;
                TrackPoint v = getTrackPoint(trackSet, TRACKLISTS(t, j), time);
                dist[j] = SQR(u.x-v.x) + SQR(u.y-v.y);
            }
            KNNSort(dist, idx, 0, nrTrack[t]-1);
            for (int k = 0; k < nrNeighbor; k ++) KNN(t, i, k) = idx[k];
        }
    }

    delete[] dist;
    delete[] idx;
}

void CoherentFilter::KNNSort(int *dist, int *idx, int l, int r)
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
    if (l < j) KNNSort(dist, idx, l, j);
    if (i < r) KNNSort(dist, idx, i, r);
}

TrackPoint CoherentFilter::getTrackPoint(const TrackSet &trackSet,
                                         int trackIdx, int time)
{
    const Track &track = trackSet[trackIdx];
    int i = time - track[0].t;
    return track[i];
}

void CoherentFilter::unionJoin(int *father, int i, int j) {
    int ri = unionGetRoot(father, i);
    int rj = unionGetRoot(father, j);
    father[ri] = rj;
}

int CoherentFilter::unionGetRoot(int *father, int i) {
    if (father[i] == i) return i;
    father[i] = unionGetRoot(father, father[i]);
    return father[i];
}
