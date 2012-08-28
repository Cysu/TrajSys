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
    explicit KltTracker(const QFileInfoList &files,
                        const QString &params,
                        const QString &outputFileName,
                        QObject *parent = 0);

    void run();
    
private:
    QFileInfoList files;
    QString params;
    QString outputFileName;

    void parseParams(int &nrFeatures, int &fgThres);
    void addTrackPoint(Track *tracks, int idx, int t, int x, int y);
    static bool isStableTrack(const Track &track);
};

#endif // KLTTRACKER_H
