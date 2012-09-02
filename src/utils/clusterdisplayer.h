#ifndef CLUSTERDISPLAYER_H
#define CLUSTERDISPLAYER_H

#include <QObject>
#include <QString>

#include "utils.h"
#include "frameio.h"
#include "clusterio.h"
#include "opencv2/opencv.hpp"

class ClusterDisplayer : public QObject
{
    Q_OBJECT

public:
    explicit ClusterDisplayer(const int &nrFeature, QObject *parent = 0);
    explicit ClusterDisplayer(const QString &clsFile, QObject *parent = 0);

    void release();

    void dispFrame(const cv::Mat &frame, ClusterPoint *clusterPoints);
    void disp();

    bool ignoreSmallCluster;
    int mode;
    
private:
    FrameIO *frameIO;
    ClusterIO *clusterIO;

    QString window;
    int nrFeature;

    int *tot;
};

#endif // CLUSTERDISPLAYER_H
