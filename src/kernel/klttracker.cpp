#include "klttracker.h"
#include <cstdlib>

#define BUFTP(t, k) bufTrackPoints[(t)*nrFeature + k]

using std::string;
using std::vector;
using cv::cvtColor;
using cv::Mat;

KltTracker::KltTracker(const string& params)
{
    // Get parameters of KLT tracker.
    parseParams(params);

    // Initialize some auxiliary variables.
    if (stThres > 0)
        bufTrackPoints = new TrackPoint[stThres * nrFeature];
    else
        bufTrackPoints = NULL;

    // Initialize KLT tracker.
    tc = KLTCreateTrackingContext();
    fl = KLTCreateFeatureList(nrFeature);

    frameIdx = 0;
}

void KltTracker::release()
{
    delete[] bufTrackPoints;
    bufTrackPoints = NULL;

    KLTFreeTrackingContext(tc);
    KLTFreeFeatureList(fl);
    tc = NULL;
    fl = NULL;

    frameIdx = 0;
}

void KltTracker::recordBgFrame(const Mat& frame)
{
    bgFrame = frame.clone();
}

void KltTracker::getTrackPoints(const Mat& frame, TrackPoint* trackPoints)
{
    // First change to gray scale image.
    cvtColor(frame, cntFrame, CV_BGR2GRAY);

    int cntTid, prevTid;
    if (stThres > 0) {
        cntTid = frameIdx % stThres;
        prevTid = (frameIdx-1) % stThres;
    }

    if (frameIdx == 0) {
        // Select good features to be tracked.
        int nrCols = cntFrame.cols, nrRows = cntFrame.rows;
        KLTSelectGoodFeatures(tc, cntFrame.data, nrCols, nrRows, fl);
        for (int k = 0; k < nrFeature; k ++) {
            int x = (int)(fl->feature[k]->x + 0.5);
            int y = (int)(fl->feature[k]->y + 0.5);

            TrackPoint trackPoint;
            trackPoint.x = x;
            trackPoint.y = y;
            trackPoint.flag = FLAG_NEW_TRACKPOINT;

            if (fgThres > 0) {
                int v = (int)(cntFrame.at<unsigned char>(y, x));
                int bgV = (int)(bgFrame.at<unsigned char>(y, x));
                if (abs(v-bgV) >= fgThres)
                    trackPoint.flag |= FLAG_IS_FOREGROUND;
            }

            trackPoints[k] = trackPoint;

            if (stThres > 0)
                BUFTP(cntTid, k) = trackPoint;
        }
    } else {
        // Tracking features.
        // Replace the lost features.
        int nrCols = cntFrame.cols, nrRows = cntFrame.rows;
        KLTTrackFeatures(tc, prevFrame.data, cntFrame.data, nrCols, nrRows, fl);
        KLTReplaceLostFeatures(tc, cntFrame.data, nrCols, nrRows, fl);

        for (int k = 0; k < nrFeature; k ++) {
            int x = (int)(fl->feature[k]->x + 0.5);
            int y = (int)(fl->feature[k]->y + 0.5);

            if (fl->feature[k]->val == 0) {
                // The feature is tracked this frame.
                TrackPoint trackPoint;
                trackPoint.x = x;
                trackPoint.y = y;
                trackPoint.flag = 0;

                if (stThres > 0) {
                    if (BUFTP(prevTid, k).flag & FLAG_IS_FOREGROUND) {
                        if (!isStableTrack(k)) trackPoint.flag = FLAG_IS_FOREGROUND;
                    }
                } else {
                    trackPoint.flag = FLAG_IS_FOREGROUND;
                }

                trackPoints[k] = trackPoint;

                if (stThres > 0)
                    BUFTP(cntTid, k) = trackPoint;
            } else if (fl->feature[k]->val > 0) {
                // The feature is newly selected.
                // Add the new track.
                TrackPoint trackPoint;
                trackPoint.x = x;
                trackPoint.y = y;
                trackPoint.flag = FLAG_NEW_TRACKPOINT;

                if (fgThres > 0) {
                    int v = (int)(cntFrame.at<unsigned char>(y, x));
                    int bgV = (int)(bgFrame.at<unsigned char>(y, x));
                    if (abs(v-bgV) >= fgThres)
                        trackPoint.flag |= FLAG_IS_FOREGROUND;
                }

                trackPoints[k] = trackPoint;

                if (stThres > 0)
                    BUFTP(cntTid, k) = trackPoint;
            }
        }
    }

    prevFrame = cntFrame.clone();
    frameIdx ++;
}

void KltTracker::parseParams(const std::string &params)
{
    vector<string> paramList = split(params, ',');
    nrFeature = atoi(paramList[0].c_str());
    fgThres = atoi(paramList[1].c_str());
    stThres = atoi(paramList[2].c_str());
}

bool KltTracker::isStableTrack(int idx)
{
    if (frameIdx < stThres) return false;
    int x0, y0;
    for (int i = 0; i <= stThres; i ++) {
        int tid = (frameIdx-i) % stThres;
        if (BUFTP(tid, idx).flag & FLAG_NEW_TRACKPOINT) return false;
        if (i == 0) {
            x0 = BUFTP(tid, idx).x;
            y0 = BUFTP(tid, idx).y;
        } else {
            int x = BUFTP(tid, idx).x, y = BUFTP(tid, idx).y;
            int d = SQR(x-x0) + SQR(y-y0);
            if (d >= 16) return false;
        }
    }
    return true;
}
