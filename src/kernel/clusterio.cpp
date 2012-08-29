#include "clusterio.h"

#include <cstdio>

ClusterIO::ClusterIO(QObject *parent) :
    QObject(parent)
{
}

void ClusterIO::setInput(const QString &filePath)
{
    fin = fopen(filePath.toStdString().c_str(), "r");
}

bool ClusterIO::readPath(QString &path)
{
    char buf[1000];
    if (fscanf(fin, "%s\n", buf) == EOF) return false;
    path = QString(buf);
    return true;
}

bool ClusterIO::readCluster(Cluster &cluster)
{
    int n;
    if (fscanf(fin, "%d ", &n) == EOF) return false;
    for (int k = 0; k < n; k ++) {
        ClusterPoint clusterPoint;
        fscanf(fin, "(%d,%d,%d)", &clusterPoint.x, &clusterPoint.y, &clusterPoint.l);
        cluster.push_back(clusterPoint);
    }
    fscanf(fin, "\n");
    return true;
}

void ClusterIO::closeInput()
{
    fclose(fin);
}

void ClusterIO::setOutput(const QString &filePath)
{
    fout = fopen(filePath.toStdString().c_str(), "w");
}

void ClusterIO::writePath(const QString &path)
{
    fprintf(fout, "%s\n", path.toStdString().c_str());
}

void ClusterIO::writeCluster(const Cluster &cluster)
{
    fprintf(fout, "%d ", cluster.size());
    for (int k = 0; k < cluster.size(); k ++)
        fprintf(fout, "(%d,%d,%d)", cluster[k].x, cluster[k].y, cluster[k].l);
    fprintf(fout, "\n");
}

void ClusterIO::closeOutput()
{
    fclose(fout);
}
