#include <cstdio>

#include "trackio.h"

TrackIO::TrackIO()
{
    fin = NULL;
    fout = NULL;
    nrFeature = 0;
}

void TrackIO::setOutput(const QString &filePath)
{
    fout = fopen(filePath.toStdString().c_str(), "w");
}

void TrackIO::writeInfo(const QString &sourcePath,
                        const int &nrFeature, const int &fgThres, const int &stThres)
{
    fprintf(fout, "%s\n", sourcePath.toStdString().c_str());
    fprintf(fout, "%d %d %d\n", nrFeature, fgThres, stThres);
    this->nrFeature = nrFeature;
}

void TrackIO::writeFrame(TrackPoint *trackPoints)
{
    for (int k = 0; k < nrFeature; k ++)
        fprintf(fout, "(%d,%d,%d)", trackPoints[k].flag,
                trackPoints[k].x, trackPoints[k].y);
    fprintf(fout, "\n");
}

void TrackIO::closeOutput()
{
    if (fout != NULL) fclose(fout);
}

void TrackIO::setInput(const QString &filePath)
{
    fin = fopen(filePath.toStdString().c_str(), "r");
}

bool TrackIO::readInfo(QString &sourcePath,
                       int &nrFeature, int &fgThres, int &stThres)
{
    char buf[1000];
    if (fscanf(fin, "%s\n", buf) == EOF) return false;
    sourcePath = QString(buf);
    if (fscanf(fin, "%d %d %d\n", &nrFeature, &fgThres, &stThres) == EOF) return false;
    this->nrFeature = nrFeature;
    return true;
}

bool TrackIO::readFrame(TrackPoint *trackPoints)
{
    for (int k = 0; k < nrFeature; k ++) {
        TrackPoint trackPoint;
        fscanf(fin, "(%d,%d,%d)", &trackPoint.flag, &trackPoint.x, &trackPoint.y);
        trackPoints[k] = trackPoint;
    }
    fscanf(fin, "\n");
    return true;
}

void TrackIO::closeInput()
{
    if (fin != NULL) fclose(fin);
}
