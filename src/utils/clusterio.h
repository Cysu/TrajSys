#ifndef CLUSTERIO_H
#define CLUSTERIO_H

#include "utils.h"

class ClusterIO
{
public:
    ClusterIO();
    
    void setInput(const std::string& filePath);
    bool readInfo(std::string& sourcePath, int& nrFeature);
    bool readFrame(ClusterPoint* clusterPoints);
    void closeInput();

    void setOutput(const std::string& filePath);
    void writeInfo(const std::string& sourcePath, int nrFeature);
    void writeFrame(ClusterPoint* clusterPoints);
    void closeOutput();

private:
    FILE *fin, *fout;
    int nrFeature;
    
};

#endif // CLUSTERIO_H
