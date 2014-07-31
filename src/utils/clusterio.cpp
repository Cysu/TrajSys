#include "clusterio.h"
#include <cstdio>

using std::string;

ClusterIO::ClusterIO()
{
    fin = NULL;
    fout = NULL;
    nrFeature = 0;
}

void ClusterIO::setInput(const string& filePath)
{
    fin = fopen(filePath.c_str(), "r");
}

bool ClusterIO::readInfo(string& sourcePath, int& nrFeature)
{
    char buf[1000];
    if (fscanf(fin, "%s\n", buf) == EOF) return false;
    sourcePath = buf;

    if (fscanf(fin, "%d\n", &nrFeature) == EOF) return false;
    this->nrFeature = nrFeature;
    return true;
}

bool ClusterIO::readFrame(ClusterPoint* clusterPoints)
{
    for (int k = 0; k < nrFeature; k ++) {
        ClusterPoint clusterPoint;
        if (fscanf(fin, "(%d,%d,%d,%d,%d)", &clusterPoint.flag,
               &clusterPoint.x, &clusterPoint.y,
               &clusterPoint.rawLabel, &clusterPoint.ascLabel) == EOF) return false;
        clusterPoints[k] = clusterPoint;
    }
    if (fscanf(fin, "\n") == EOF) return false;
    return true;
}

void ClusterIO::closeInput()
{
    if (fin != NULL) fclose(fin);
}

void ClusterIO::setOutput(const string& filePath)
{
    fout = fopen(filePath.c_str(), "w");
}

void ClusterIO::writeInfo(const string& sourcePath, int nrFeature)
{
    fprintf(fout, "%s\n", sourcePath.c_str());
    fprintf(fout, "%d\n", nrFeature);
    this->nrFeature = nrFeature;
}

void ClusterIO::writeFrame(ClusterPoint* clusterPoints)
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
