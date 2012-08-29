#ifndef COHERENTFILTER_H
#define COHERENTFILTER_H

#include <QObject>
#include <QFileInfoList>
#include <QString>
#include <QStringList>

#include "kernelutils.h"

class CoherentFilter : public QObject
{
    Q_OBJECT

public:
    explicit CoherentFilter(QFileInfoList *files, TrackSet *trackSet,
                            const QString &params,
                            const QString &outputFilePath,
                            QObject *parent = 0);

    void run();
    
private:
    QFileInfoList *files;
    TrackSet *trackSet;
    QString params;
    QString outputFilePath;

    int period, nrNeighbor;
    double vThres;

    int startFrame, endFrame, nrFrame;
    int nrFeature;

    int *nrTrack;
    int *trackLists;
    int *knn;
    int *label;

    void parseParams();

    void computeTrackLists();
    void computeKNN();
    void KNNSort(int *dist, int *idx, int l, int r);
    TrackPoint getTrackPoint(int idx, int t);

    void unionJoin(int *father, int i, int j);
    int unionGetRoot(int *father, int i);
    
};

#endif // COHERENTFILTER_H
