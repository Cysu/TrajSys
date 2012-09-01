#include "klttracker.h"

KltTracker::KltTracker(const QString &params,
                       QObject *parent) :
    QObject(parent)
{
    // Get parameters of KLT tracker.
    parseParams(params);

    // Initialize some auxiliary variables.
    tracks = new Track[nrFeatures];
    isForeground = new bool[nrFeatures];
    memset(isForeground, false, nrFeatures*sizeof(bool));

    tc = NULL;
    fl = NULL;

    frameIdx = 0;
}

void KltTracker::getFromFiles(const QFileInfoList &files,
                              TrackSet *trackSet,
                              const QString &ofName)
{
    // Initialize the output.
    trackIO.setOutput(ofName);
    trackIO.writePath(files.at(0).absolutePath());

    int startFrame = 0, endFrame = files.size()-1;
    if (fgThres > 0) startFrame = 1;
    bgFrame = cv::imread(files.at(0).filePath().toStdString(), 0);
    prevFrame = cv::imread(files.at(startFrame).filePath().toStdString(), 0);
    int nrCols = prevFrame.cols, nrRows = prevFrame.rows;

    // Initialize KLT tracker.
    tc = KLTCreateTrackingContext();
    fl = KLTCreateFeatureList(nrFeatures);

    // Select good features to be tracked.
    // Remove those background features.
    KLTSelectGoodFeatures(tc, prevFrame.data, nrCols, nrRows, fl);
    for (int k = 0; k < nrFeatures; k ++) {
        int x = (int)(fl->feature[k]->x + 0.5);
        int y = (int)(fl->feature[k]->y + 0.5);
        int v = (int)(prevFrame.at<quint8>(y, x));
        int bgV = (int)(bgFrame.at<quint8>(y, x));
        if (abs(v - bgV) >= fgThres)
            addTrackPoint(k, startFrame, x, y);
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
                if (isForeground[k]) {
                    trackSet->push_back(tracks[k]);
                    trackIO.writeTrack(tracks[k]);
                }

                // Add the new track.
                if (abs(v - bgV) < fgThres) isForeground[k] = false;
                else addTrackPoint(k, i, x, y);
            }
        }
        prevFrame = cntFrame;
    }

    for (int k = 0; k < nrFeatures; k ++)
        if (isForeground[k]) {
            trackSet->push_back(tracks[k]);
            trackIO.writeTrack(tracks[k]);
        }

    trackIO.closeOutput();
}

void KltTracker::getFromImage(const cv::Mat &img,
                              TrackSet *trackSet, bool *(&mark))
{
    if (tc == NULL) {
        // This is the first frame.

        // Initialize KLT tracker.
        tc = KLTCreateTrackingContext();
        fl = KLTCreateFeatureList(nrFeatures);

        // Select good features to be tracked.
        int nrCols = img.cols, nrRows = img.rows;
        KLTSelectGoodFeatures(tc, img.data, nrCols, nrRows, fl);
        for (int k = 0; k < nrFeatures; k ++) {
            int x = (int)(fl->feature[k]->x + 0.5);
            int y = (int)(fl->feature[k]->y + 0.5);
            addTrackPoint(k, 0, x, y);
        }

    } else {
        frameIdx ++;

        // Tracking features.
        // Replace the lost features.
        int nrCols = img.cols, nrRows = img.rows;
        KLTTrackFeatures(tc, prevFrame.data, img.data, nrCols, nrRows, fl);
        KLTReplaceLostFeatures(tc, img.data, nrCols, nrRows, fl);

        // Ensure the features are all foreground features.
        for (int k = 0; k < nrFeatures; k ++) {
            int x = (int)(fl->feature[k]->x + 0.5);
            int y = (int)(fl->feature[k]->y + 0.5);

            if (fl->feature[k]->val == 0) {
                // The feature is tracked this frame.
                if (isForeground[k]) {
                    TrackPoint trackPoint = {frameIdx, x, y};
                    tracks[k].push_back(trackPoint);
                    if (isStableTrack(tracks[k])) isForeground[k] = false;
                }
            } else if (fl->feature[k]->val > 0) {
                // The feature is newly selected.
                // Add the new track.
                addTrackPoint(k, frameIdx, x, y);
            }
        }
    }

    prevFrame = img.clone();
    for (int k = 0; k < nrFeatures; k ++) {
//        if (!isForeground[k]) continue;
        trackSet->push_back(tracks[k]);
    }
    mark = isForeground;
}

void KltTracker::finish()
{
    delete[] isForeground;
    delete[] tracks;

    isForeground = NULL;
    tracks = NULL;

    delete tc;
    delete fl;

    tc = NULL;
    fl = NULL;

    frameIdx = 0;
}

void KltTracker::parseParams(const QString &params)
{
    QStringList paramList = params.split(",");
    nrFeatures = paramList.at(0).toInt();
    fgThres = paramList.at(1).toInt();
}

void KltTracker::addTrackPoint(int idx, int t, int x, int y)
{
    TrackPoint trackPoint = {t, x, y};
    Track track;
    track.push_back(trackPoint);
    tracks[idx] = track;
    isForeground[idx] = true;
}

bool KltTracker::isStableTrack(const Track &track)
{
    if (track.size() <= 10) return false;
    int x0 = track.back().x;
    int y0 = track.back().y;

    for (int i = track.size()-10; i < track.size()-1; i ++) {
        int x = track[i].x, y = track[i].y;
        int d = SQR(x-x0) + SQR(y-y0);
        if (d >= 16) return false;
    }

    return true;
}
