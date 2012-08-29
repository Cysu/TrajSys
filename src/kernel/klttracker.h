#ifndef KLTTRACKER_H
#define KLTTRACKER_H

#include <QObject>
#include <QFileInfoList>
#include <QString>
#include <QStringList>

#include "kernelutils.h"

class KltTracker : public QObject
{
    Q_OBJECT

public:
    explicit KltTracker(QFileInfoList *files,
                        const QString &params,
                        TrackSet *trackSet,
                        const QString &outputFilePath,
                        QObject *parent = 0);

    void run();
    
private:
    QFileInfoList *files;
    QString params;
    TrackSet *trackSet;
    QString outputFilePath;

    void parseParams(int &nrFeatures, int &fgThres);
    void addTrackPoint(Track *tracks, int idx, int t, int x, int y);
    static bool isStableTrack(const Track &track);
};

#endif // KLTTRACKER_H
