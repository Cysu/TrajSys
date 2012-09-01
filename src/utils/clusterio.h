#ifndef CLUSTERIO_H
#define CLUSTERIO_H

#include <QString>

#include "utils.h"

class ClusterIO
{
public:
    ClusterIO();
    
    void setInput(const QString &filePath);
    bool readInfo(QString &sourcePath, int &nrFeature);
    bool readFrame(ClusterPoint *clusterPoints);
    void closeInput();

    void setOutput(const QString &filePath);
    void writeInfo(const QString &sourcePath, const int &nrFeature);
    void writeFrame(ClusterPoint *clusterPoints);
    void closeOutput();

private:
    FILE *fin, *fout;
    int nrFeature;
    
};

#endif // CLUSTERIO_H
