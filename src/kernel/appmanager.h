#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>

#include "kernelutils.h"

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(QFileInfoList *files, TrackSet *trackSet,
                        const QString &type, const QString &params,
                        const QString &outputFileName,
                        QObject *parent = 0);
    void work();

private:
    QFileInfoList *files;
    TrackSet *trackSet;
    QString type, params;
    QString outputFileName;
};

#endif // APPMANAGER_H
