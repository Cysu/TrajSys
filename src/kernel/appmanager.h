#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QVector>

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(const QFileInfoList &files,
                        const QString &type, const QString &params,
                        const QString &outputFileName,
                        QObject *parent = 0);
    void work();

private:
    QString type, params;
    QFileInfoList files;
    QString outputFileName;
};

#endif // APPMANAGER_H
