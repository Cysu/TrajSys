#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>

#include <opencv2/opencv.hpp>

#include "coherentfilter.h"
#include "utils/utils.h"
#include "utils/clusterio.h"
#include "utils/clusterdisplayer.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(const QString &type, const QString &params,
                        const int &nrFeature,
                        const QString &srcPath, const QString &ofPath,
                        const bool &needDisplay,
                        QObject *parent = 0);

    void getResult(const cv::Mat &frame, TrackPoint *trackPoints);

    void release();

    ClusterDisplayer *clusterDisplayer;
private:
    ClusterIO *clusterIO;

    CoherentFilter *coherentFilter;
    ClusterPoint *clusterPoints;
};

#endif // APPMANAGER_H
