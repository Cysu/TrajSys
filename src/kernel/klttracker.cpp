#include "klttracker.h"

#define BUFTP(t, k) bufTrackPoints[(t)*nrFeature + k]

KltTracker::KltTracker(const QString &params,
                       QObject *parent) :
    QObject(parent)
{
    // Get parameters of KLT tracker.
    parseParams(params);

    // Initialize some auxiliary variables.
    bufTrackPoints = new TrackPoint[stThres * nrFeature];

    // Initialize KLT tracker.
    tc = KLTCreateTrackingContext();
    fl = KLTCreateFeatureList(nrFeature);

    frameIdx = 0;
}

void KltTracker::release()
{
    delete[] bufTrackPoints;
    delete tc;
    delete fl;

    tc = NULL;
    fl = NULL;
    bufTrackPoints = NULL;

    frameIdx = 0;
}

void KltTracker::recordBgFrame(const cv::Mat &frame)
{
    bgFrame = frame.clone();
}

void KltTracker::getTrackPoints(const cv::Mat &frame, TrackPoint *trackPoints)
{
    // First change to gray scale image.
    cv::cvtColor(frame, cntFrame, CV_BGR2GRAY);

    int cntTid = frameIdx % stThres;

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
                int v = (int)(cntFrame.at<quint8>(y, x));
                int bgV = (int)(bgFrame.at<quint8>(y, x));
                if (abs(v-bgV) >= fgThres)
                    trackPoint.flag |= FLAG_IS_FOREGROUND;
            }

            trackPoints[k] = trackPoint;
            BUFTP(cntTid, k) = trackPoint;
        }
    } else {
        int prevTid = (frameIdx-1) % stThres;

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
                trackPoint.y = y;;

                if (BUFTP(prevTid, k).flag & FLAG_IS_FOREGROUND) {
                    if (!isStableTrack(k)) trackPoint.flag = FLAG_IS_FOREGROUND;
                }

                trackPoints[k] = trackPoint;
                BUFTP(cntTid, k) = trackPoint;
            } else if (fl->feature[k]->val > 0) {
                // The feature is newly selected.
                // Add the new track.
                TrackPoint trackPoint;
                trackPoint.x = x;
                trackPoint.y = y;
                trackPoint.flag = FLAG_NEW_TRACKPOINT;

                if (fgThres > 0) {
                    int v = (int)(cntFrame.at<quint8>(y, x));
                    int bgV = (int)(bgFrame.at<quint8>(y, x));
                    if (abs(v-bgV) >= fgThres)
                        trackPoint.flag |= FLAG_IS_FOREGROUND;
                }

                trackPoints[k] = trackPoint;
                BUFTP(cntTid, k) = trackPoint;
            }
        }
    }

    prevFrame = cntFrame.clone();
    frameIdx ++;
}

void KltTracker::parseParams(const QString &params)
{
    QStringList paramList = params.split(",");
    nrFeature = paramList.at(0).toInt();
    fgThres = paramList.at(1).toInt();
    stThres = paramList.at(2).toInt();
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
