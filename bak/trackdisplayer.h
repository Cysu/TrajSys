#ifndef TRACKDISPLAYER_H
#define TRACKDISPLAYER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QDir>

#include <opencv2/opencv.hpp>
#include "utils/utils.h"

class TrackDisplayer : public QObject
{
    Q_OBJECT

public:
    explicit TrackDisplayer(QObject *parent = 0);

    void display(const QString &window, const QString &trkFile);
    void display(const QString &window, const TrackSet &trackSet, cv::Mat &img);
    
private:
    void getImageFiles(const QString &folder, QFileInfoList &files);
    void drawTrack(cv::Mat &img, const TrackSet &trackSet, int time = -1);
    
};

#endif // TRACKDISPLAYER_H
