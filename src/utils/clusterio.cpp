#include "clusterio.h"

#include <cstdio>

ClusterIO::ClusterIO()
{
    fin = NULL;
    fout = NULL;
    nrFeature = 0;
}

void ClusterIO::setInput(const QString &filePath)
{
    fin = fopen(filePath.toStdString().c_str(), "r");
}

bool ClusterIO::readInfo(QString &sourcePath, int &nrFeature)
{
    char buf[1000];
    if (fscanf(fin, "%s\n", buf) == EOF) return false;
    sourcePath = QString(buf);

    if (fscanf(fin, "%d\n", &nrFeature) == EOF) return false;
    this->nrFeature = nrFeature;
    return true;
}

bool ClusterIO::readFrame(ClusterPoint *clusterPoints)
{
    for (int k = 0; k < nrFeature; k ++) {
        ClusterPoint clusterPoint;
        fscanf(fin, "(%d,%d,%d,%d,%d)", &clusterPoint.flag,
               &clusterPoint.x, &clusterPoint.y,
               &clusterPoint.rawLabel, &clusterPoint.ascLabel);
        clusterPoints[k] = clusterPoint;
    }
    fscanf(fin, "\n");
    return true;
}

void ClusterIO::closeInput()
{
    if (fin != NULL) fclose(fin);
}

void ClusterIO::setOutput(const QString &filePath)
{
    fout = fopen(filePath.toStdString().c_str(), "w");
}

void ClusterIO::writeInfo(const QString &sourcePath, const int &nrFeature)
{
    fprintf(fout, "%s\n", sourcePath.toStdString().c_str());
    fprintf(fout, "%d\n", nrFeature);
    this->nrFeature = nrFeature;
}

void ClusterIO::writeFrame(ClusterPoint *clusterPoints)
{
    if (clusterPoints != NULL) {
        for (int k = 0; k < nrFeature; k ++)
            fprintf(fout, "(%d,%d,%d,%d,%d)", clusterPoints[k].flag,
                    clusterPoints[k].x, clusterPoints[k].y,
                    clusterPoints[k].rawLabel, clusterPoints[k].ascLabel);
    }
    fprintf(fout, "\n");
}

void ClusterIO::closeOutput()
{
    if (fout != NULL) fclose(fout);
}
