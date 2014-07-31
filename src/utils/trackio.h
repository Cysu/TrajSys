#ifndef TRACKIO_H
#define TRACKIO_H

#include "utils.h"

class TrackIO
{
public:
    TrackIO();

    void setInput(const std::string& filePath);
    bool readInfo(std::string& sourcePath,
                  int& nrFeature, int& fgThres, int& stThres);
    bool readFrame(TrackPoint* trackPoints);
    void closeInput();

    void setOutput(const std::string& filePath);
    void writeInfo(const std::string& sourcePath,
                   int nrFeature, int fgThres, int stThres);
    void writeFrame(TrackPoint* trackPoints);
    void closeOutput();
    
private:
    FILE *fin, *fout;
    int nrFeature;
    
};

#endif // TRACKIO_H
