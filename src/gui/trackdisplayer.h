#ifndef TRACKDISPLAYER_H
#define TRACKDISPLAYER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QDir>

#include <opencv2/opencv.hpp>
#include "kernel/kernelutils.h"

class TrackDisplayer : public QObject
{
    Q_OBJECT

public:
    explicit TrackDisplayer(const QString &filePath, QObject *parent = 0);

    void display();
    
private:
    QString filePath;

    void getImageFiles(const QString &folder, QFileInfoList &files);

    void drawTrack(cv::Mat &img, const TrackSet &trackSet, int time);
    
};

#endif // TRACKDISPLAYER_H
