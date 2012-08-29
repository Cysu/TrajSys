#include <cstdio>

#include "trackio.h"

TrackIO::TrackIO(QObject *parent) :
    QObject(parent),
    fin(NULL),
    fout(NULL)
{
}

void TrackIO::setOutput(const QString &filePath)
{
    fout = fopen(filePath.toStdString().c_str(), "w");
}

void TrackIO::writePath(const QString &path)
{
    fprintf(fout, "%s\n", path.toStdString().c_str());
}

void TrackIO::writeTrack(const Track &track)
{
    fprintf(fout, "%d ", track.size());
    for (int k = 0; k < track.size(); k ++)
        fprintf(fout, "(%d,%d,%d)", track[k].x, track[k].y, track[k].t);
    fprintf(fout, "\n");
}

void TrackIO::closeOutput()
{
    fclose(fout);
}

void TrackIO::setInput(const QString &filePath)
{
    fin = fopen(filePath.toStdString().c_str(), "r");
}

bool TrackIO::readPath(QString &path)
{
    char buf[1000];
    if (fscanf(fin, "%s\n", buf) == EOF) return false;
    path = QString(buf);
    return true;
}

bool TrackIO::readTrack(Track &track)
{
    int n;
    if (fscanf(fin, "%d ", &n) == EOF) return false;
    for (int k = 0; k < n; k ++) {
        TrackPoint trackPoint;
        fscanf(fin, "(%d,%d,%d)", &trackPoint.x, &trackPoint.y, &trackPoint.t);
        track.push_back(trackPoint);
    }
    fscanf(fin, "\n");
    return true;
}

void TrackIO::closeInput()
{
    fclose(fin);
}
