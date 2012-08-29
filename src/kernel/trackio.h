#ifndef TRACKIO_H
#define TRACKIO_H

#include <QObject>
#include <QString>

#include "kernelutils.h"

class TrackIO : public QObject
{
    Q_OBJECT

public:
    explicit TrackIO(QObject *parent = 0);

    void setInput(const QString &filePath);
    bool readPath(QString &path);
    bool readTrack(Track &track);
    void closeInput();

    void setOutput(const QString &filePath);
    void writePath(const QString &path);
    void writeTrack(const Track &track);
    void closeOutput();
    
private:
    FILE *fin, *fout;
    
};

#endif // TRACKIO_H
