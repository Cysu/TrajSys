#include "clusterdisplayer.h"
#include "kernel/clusterio.h"
#include "gui/guiutils.h"

ClusterDisplayer::ClusterDisplayer(const QString &filePath,
                                   QObject *parent) :
    QObject(parent),
    filePath(filePath)
{
}

void ClusterDisplayer::display()
{
    ClusterIO clusterIO;
    clusterIO.setInput(filePath);

    QString folderPath;
    if (!clusterIO.readPath(folderPath)) return;

    QFileInfoList files;
    getImageFiles(folderPath, files);

    ClusterSet clusterSet;
    while (true) {
        Cluster cluster;
        if (!clusterIO.readCluster(cluster)) break;
        clusterSet.push_back(cluster);
    }

    clusterIO.closeInput();

    cv::namedWindow("Cluster Displayer");
    for (int i = 0; i < files.size(); i ++) {
        cv::Mat img = cv::imread(files.at(i).filePath().toStdString());
        if (i < clusterSet.size()) drawCluster(img, clusterSet[i]);
        cv::imshow("Cluster Displayer", img);
        if (cv::waitKey(50) >= 0) break;
    }
    cv::destroyWindow("Cluster Displayer");
}

void ClusterDisplayer::drawCluster(cv::Mat &img, const Cluster &cluster)
{
    for (int i = 0; i < cluster.size(); i ++) {
        const ClusterPoint &clusterPoint = cluster[i];
        cv::Point p(clusterPoint.x, clusterPoint.y);
        cv::circle(img, p, 3, EXAMPLE_COLOR[clusterPoint.l%6], -1);
    }
}

void ClusterDisplayer::getImageFiles(const QString &folder, QFileInfoList &files)
{
    QDir dir(folder);
    QStringList imgFilters;
    imgFilters << "*.bmp" << "*.jpg"<< "*.tiff" << "*.jpeg" << "*.png";
    dir.setNameFilters(imgFilters);
    files = dir.entryInfoList();
}
