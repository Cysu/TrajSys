#include "clusterdisplayer.h"
#include "gui/guiutils.h"

using std::string;
using cv::Mat;
using cv::Point;
using cv::imshow;
using cv::waitKey;
using cv::namedWindow;
using cv::destroyWindow;

ClusterDisplayer::ClusterDisplayer(int nrFeature)
    : nrFeature(nrFeature),
      window("Cluster Displayer")
{
    namedWindow(window);
    mode = 0;
    ignoreSmallCluster = false;

    clusterIO = NULL;
    frameIO = NULL;

    tot = new int[nrFeature];
}

ClusterDisplayer::ClusterDisplayer(const string& clsFile)
    : window("Cluster Displayer")
{
    clusterIO = new ClusterIO;
    clusterIO->setInput(clsFile);

    string srcPath;
    clusterIO->readInfo(srcPath, nrFeature);

    frameIO = new FrameIO(srcPath.c_str());
    Mat bgFrame;
    frameIO->readNextFrame(bgFrame);

    namedWindow(window);
    mode = 0;
    ignoreSmallCluster = false;

    tot = new int[nrFeature];
}

void ClusterDisplayer::release()
{
    destroyWindow(window);

    delete[] tot;

    if (frameIO != NULL) {
        delete frameIO;
        frameIO = NULL;
    }

    if (clusterIO != NULL) {
        clusterIO->closeInput();
        delete clusterIO;
        clusterIO = NULL;
    }
}

void ClusterDisplayer::disp()
{
    Mat frame;
    ClusterPoint* clusterPoints = new ClusterPoint[nrFeature];

    while (true) {
        int t = waitKey(50);
        if (t == 'q') break;
        else if (t == 'r') mode = 1;
        else if (t == 'a') mode = 0;
        else if (t == 'i') ignoreSmallCluster = true;
        else if (t == 'n') ignoreSmallCluster = false;

        int frameIdx = frameIO->readNextFrame(frame);
        if (frameIdx == -1) break;
        if (!clusterIO->readFrame(clusterPoints)) break;
        dispFrame(frame, clusterPoints);
    }

    delete[] clusterPoints;
}

void ClusterDisplayer::dispFrame(const Mat& frame, ClusterPoint* clusterPoints)
{
    Mat img = frame.clone();

    if (ignoreSmallCluster) {
        memset(tot, 0, nrFeature*sizeof(int));
        for (int i = 0; i < nrFeature; i ++) {
            if (clusterPoints[i].rawLabel == -1) continue;
            tot[clusterPoints[i].rawLabel] ++;
        }
    }

    for (int i = 0; i < nrFeature; i ++) {
        if (!(clusterPoints[i].flag & FLAG_IS_FOREGROUND)) continue;

        if (ignoreSmallCluster) {
            if (tot[clusterPoints[i].rawLabel] < 4) continue;
        }

        Point p(clusterPoints[i].x, clusterPoints[i].y);
        if (mode == 0) {
            if (clusterPoints[i].ascLabel == -1) continue;
            cv::circle(img, p, 3, EXAMPLE_COLOR[clusterPoints[i].ascLabel%6], -1);
        } else {
            if (clusterPoints[i].rawLabel == -1) continue;
            cv::circle(img, p, 3, EXAMPLE_COLOR[clusterPoints[i].rawLabel%6], -1);
        }
    }
    imshow(window, img);
}
