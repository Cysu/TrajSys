#include "trackio.h"
#include <cstdio>

using std::string;


TrackIO::TrackIO()
{
    fin = NULL;
    fout = NULL;
    nrFeature = 0;
}

void TrackIO::setOutput(const string& filePath)
{
    fout = fopen(filePath.c_str(), "w");
}

void TrackIO::writeInfo(const string& sourcePath,
                        int nrFeature, int fgThres, int stThres)
{
    fprintf(fout, "%s\n", sourcePath.c_str());
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

void TrackIO::setInput(const string& filePath)
{
    fin = fopen(filePath.c_str(), "r");
}

bool TrackIO::readInfo(string& sourcePath,
                       int& nrFeature, int& fgThres, int& stThres)
{
    char buf[1000];
    if (fscanf(fin, "%s\n", buf) == EOF) return false;
    sourcePath = buf;
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
