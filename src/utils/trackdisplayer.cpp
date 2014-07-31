#include "trackdisplayer.h"
#include "gui/guiutils.h"

#define BUFTP(t, i) bufTrackPoints[(t)*nrFeature + (i)]

using std::string;
using cv::Mat;
using cv::Point;
using cv::imshow;
using cv::waitKey;
using cv::namedWindow;
using cv::destroyWindow;

TrackDisplayer::TrackDisplayer(int nrFeature, int trackLength)
    : window("Track Displayer"),
      nrFeature(nrFeature),
      trackLength(trackLength)
{
    trackIO = NULL;
    frameIO = NULL;

    bufTrackPoints = new TrackPoint[trackLength * nrFeature];
    frameIdx = 0;
    namedWindow(window);
}

TrackDisplayer::TrackDisplayer(const string& trkFile)
    : window("Track Displayer"),
      trackLength(10)
{
    trackIO = new TrackIO;
    trackIO->setInput(trkFile);

    string srcPath;
    int nrFeature, fgThres, stThres;
    trackIO->readInfo(srcPath, nrFeature, fgThres, stThres);
    this->nrFeature = nrFeature;

    frameIO = new FrameIO(srcPath);
    Mat bgFrame;
    if (fgThres > 0) frameIO->readNextFrame(bgFrame);

    bufTrackPoints = new TrackPoint[trackLength * nrFeature];
    frameIdx = 0;
    namedWindow(window);
}

void TrackDisplayer::release()
{
    delete[] bufTrackPoints;
    bufTrackPoints = NULL;

    if (trackIO != NULL) {
        trackIO->closeInput();
        delete trackIO;
        trackIO = NULL;
    }

    if (frameIO != NULL) {
        delete frameIO;
        frameIO = NULL;
    }

    destroyWindow(window);
}

void TrackDisplayer::disp()
{
    Mat frame;
    TrackPoint* trackPoints = new TrackPoint[nrFeature];

    while (true) {
        int frameIdx = frameIO->readNextFrame(frame);
        if (frameIdx == -1) break;
        if (!trackIO->readFrame(trackPoints)) break;
        dispFrame(frame, trackPoints);
        if (waitKey(50) == 'q') break;
    }

    delete[] trackPoints;
}

void TrackDisplayer::dispFrame(const Mat &frame, TrackPoint *trackPoints)
{
    int cntTid = frameIdx % trackLength;
    for (int i = 0; i < nrFeature; i ++) BUFTP(cntTid, i) = trackPoints[i];

    if (frameIdx < trackLength-1) {
        frameIdx ++;
        return;
    }

    Mat img = frame.clone();
    for (int i = 0; i < nrFeature; i ++) {
        // Check if the track is foreground.
        if (!(BUFTP(cntTid, i).flag & FLAG_IS_FOREGROUND)) continue;

        // Check if the track is longer than trackLength.
        bool shorter = false;
        for (int j = 0; j < trackLength-1; j ++) {
            int tid = (frameIdx-j) % trackLength;
            if (BUFTP(tid, i).flag & FLAG_NEW_TRACKPOINT) {
                shorter = true;
                break;
            }
        }
        if (shorter) continue;

        // Draw track.
        Point p(trackPoints[i].x, trackPoints[i].y), q;
        for (int j = 1; j < trackLength; j ++) {
            int tid = (frameIdx-j) % trackLength;
            q = Point(BUFTP(tid, i).x, BUFTP(tid, i).y);
            line(img, p, q, EXAMPLE_COLOR[i%6], 2);
            p = q;
        }
    }

    imshow(window, img);
    frameIdx ++;
}
