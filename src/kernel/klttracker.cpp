#include "klttracker.h"
#include "utils/klt/klt.h"
#include <opencv2/opencv.hpp>

KltTracker::KltTracker(const QFileInfoList &files,
                       const QString &params,
                       const QString &outputFileName,
                       QObject *parent) :
    QObject(parent),
    files(files),
    params(params),
    outputFileName(outputFileName)
{
}

void KltTracker::run()
{
    // Get parameters of KLT tracker.
    int nrFeatures, fgThres;
    parseParams(nrFeatures, fgThres);


    // Initialize some auxiliary variables.
    Track *tracks = new Track[nrFeatures];
    bool *isForeground = new bool[nrFeatures];
    memset(isForeground, false, nrFeatures*sizeof(bool));

    cv::Mat bgFrame, prevFrame, cntFrame;
    int startFrame = 0, endFrame = 10;
    if (fgThres > 0) startFrame = 1;
    bgFrame = cv::imread(files.at(0).filePath().toStdString(), 0);
    prevFrame = cv::imread(files.at(startFrame).filePath().toStdString(), 0);
    int nrCols = prevFrame.cols, nrRows = prevFrame.rows;


    // Initialize the output.
    FILE *fid = fopen(outputFileName.toStdString().c_str(), "w");

    // Initialize KLT tracker.
    KLT_TrackingContext tc = KLTCreateTrackingContext();
    KLT_FeatureList fl = KLTCreateFeatureList(nrFeatures);


    // Select good features to be tracked.
    // Remove those background features.
    KLTSelectGoodFeatures(tc, prevFrame.data, nrCols, nrRows, fl);
    for (int k = 0; k < nrFeatures; k ++) {
        int x = (int)(fl->feature[k]->x + 0.5);
        int y = (int)(fl->feature[k]->y + 0.5);
        int v = (int)(prevFrame.at<quint8>(y, x));
        int bgV = (int)(bgFrame.at<quint8>(y, x));
        if (abs(v - bgV) >= fgThres) {
            addTrackPoint(tracks, k, startFrame, x, y);
            isForeground[k] = true;
        }
    }


    for (int i = startFrame+1; i <= endFrame; i ++) {
        // Tracking features on each frame.
        // Replace the lost features.
        cntFrame = cv::imread(files.at(i).filePath().toStdString(), 0);
        KLTTrackFeatures(tc, prevFrame.data, cntFrame.data, nrCols, nrRows, fl);
        KLTReplaceLostFeatures(tc, cntFrame.data, nrCols, nrRows, fl);

        // Ensure the features are all foreground features.
        for (int k = 0; k < nrFeatures; k ++) {
            int x = (int)(fl->feature[k]->x + 0.5);
            int y = (int)(fl->feature[k]->y + 0.5);
            int v = (int)(cntFrame.at<quint8>(y, x));
            int bgV = (int)(bgFrame.at<quint8>(y, x));

            if (fl->feature[k]->val == 0) {
                // The feature is tracked this frame.
                if (isForeground[k]) {
                    TrackPoint trackPoint = {i, x, y};
                    tracks[k].push_back(trackPoint);
                    if (isStableTrack(tracks[k])) isForeground[k] = false;
                }
            } else if (fl->feature[k]->val > 0) {
                // The feature is newly selected.
                // Output the lost track if it's foreground track.
                if (isForeground[k]) printTrack(fid, tracks[k]);

                // Add the new track.
                if (abs(v - bgV) < fgThres) isForeground[k] = false;
                else {
                    addTrackPoint(tracks, k, i, x, y);
                    isForeground[k] = true;
                }
            }
        }

        prevFrame = cntFrame;
    }

    for (int k = 0; k < nrFeatures; k ++)
        if (isForeground[k]) printTrack(fid, tracks[k]);

    fclose(fid);

    delete[] isForeground;
    delete[] tracks;
}

void KltTracker::parseParams(int &nrFeatures, int &fgThres)
{
    QStringList paramList = params.split(",");
    nrFeatures = paramList.at(0).toInt();
    fgThres = paramList.at(1).toInt();
}

void KltTracker::addTrackPoint(Track *tracks, int idx, int t, int x, int y)
{
    TrackPoint trackPoint = {t, x, y};
    Track track;
    track.push_back(trackPoint);
    tracks[idx] = track;
}

bool KltTracker::isStableTrack(const Track &track)
{
    if (track.size() <= 20) return false;
    int x0 = track.back().x;
    int y0 = track.back().y;

    for (int i = track.size()-20; i < track.size()-1; i ++) {
        int x = track[i].x, y = track[i].y;
        int d = SQR(x-x0) + SQR(y-y0);
        if (d >= 9) return false;
    }

    return true;
}
