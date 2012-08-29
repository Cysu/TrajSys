#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>

#include "kernelutils.h"

class TrackerManager : public QObject
{
    Q_OBJECT

public:
    explicit TrackerManager(QFileInfoList *files,
                            const QString &type, const QString &params,
                            TrackSet *trackSet,
                            const QString &outputFileName,
                            QObject *parent = 0);

    void work();
    
private:
    QFileInfoList *files;
    QString type, params;
    TrackSet *trackSet;
    QString outputFileName;
};

#endif // TRACKERMANAGER_H
