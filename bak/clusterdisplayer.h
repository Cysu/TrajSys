#ifndef CLUSTERDISPLAYER_H
#define CLUSTERDISPLAYER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QDir>

#include <opencv2/opencv.hpp>

class ClusterDisplayer : public QObject
{
    Q_OBJECT

public:
    explicit ClusterDisplayer(const QString &filePath, QObject *parent = 0);

    void display();
    
private:
    QString filePath;

    void getImageFiles(const QString &folder, QFileInfoList &files);

    void drawCluster(cv::Mat &img, const Cluster &cluster);
    
};

#endif // CLUSTERDISPLAYER_H
