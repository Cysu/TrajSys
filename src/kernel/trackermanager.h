#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>

class TrackerManager : public QObject
{
    Q_OBJECT

public:
    explicit TrackerManager(const QFileInfoList &files,
                            const QString &type, const QString &params,
                            const QString &outputFileName,
                            QObject *parent = 0);

    void work();
    
private:
    QString type, params;
    QFileInfoList files;
    QString outputFileName;
};

#endif // TRACKERMANAGER_H
