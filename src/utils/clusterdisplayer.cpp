#include "clusterdisplayer.h"
#include "gui/guiutils.h"

ClusterDisplayer::ClusterDisplayer(const int &nrFeature, QObject *parent) :
    QObject(parent),
    nrFeature(nrFeature),
    window("Cluster Displayer")
{
    cv::namedWindow(window.toStdString());
    mode = 0;
    ignoreSmallCluster = false;

    clusterIO = NULL;
    frameIO = NULL;

    tot = new int[nrFeature];
}

ClusterDisplayer::ClusterDisplayer(const QString &clsFile, QObject *parent) :
    QObject(parent),
    window("Cluster Displayer")
{
    clusterIO = new ClusterIO;
    clusterIO->setInput(clsFile);

    QString srcPath;
    clusterIO->readInfo(srcPath, nrFeature);

    frameIO = new FrameIO(srcPath);
    cv::Mat bgFrame;
    frameIO->readNextFrame(bgFrame);

    cv::namedWindow(window.toStdString());
    mode = 0;
    ignoreSmallCluster = false;

    tot = new int[nrFeature];
}

void ClusterDisplayer::release()
{
    cv::destroyWindow(window.toStdString());

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
    cv::Mat frame;
    ClusterPoint *clusterPoints = new ClusterPoint[nrFeature];

    while (true) {
        int t = cv::waitKey(50);
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

void ClusterDisplayer::dispFrame(const cv::Mat &frame, ClusterPoint *clusterPoints)
{
    cv::Mat img = frame.clone();

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

        cv::Point p(clusterPoints[i].x, clusterPoints[i].y);
        if (mode == 0) {
            if (clusterPoints[i].ascLabel == -1) continue;
            cv::circle(img, p, 3, EXAMPLE_COLOR[clusterPoints[i].ascLabel%6], -1);
        } else {
            if (clusterPoints[i].rawLabel == -1) continue;
            cv::circle(img, p, 3, EXAMPLE_COLOR[clusterPoints[i].rawLabel%6], -1);
        }
    }
    cv::imshow(window.toStdString(), img);

}
