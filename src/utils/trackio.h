#ifndef TRACKIO_H
#define TRACKIO_H

#include <QObject>
#include <QString>

#include "utils.h"

class TrackIO
{
public:
    TrackIO();

    void setInput(const QString &filePath);
    bool readInfo(QString &sourcePath, int &nrFeature);
    bool readFrame(TrackPoint *trackPoints);
    void closeInput();

    void setOutput(const QString &filePath);
    void writeInfo(const QString &sourcePath, const int &nrFeature);
    void writeFrame(TrackPoint *trackPoints);
    void closeOutput();
    
private:
    FILE *fin, *fout;
    int nrFeature;
    
};

#endif // TRACKIO_H
