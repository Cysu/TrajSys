#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QStringList>
#include <QObject>
#include <QVector>
#include <QFileInfo>
#include <QDir>

class TaskManager : public QObject
{
    Q_OBJECT

public:
    struct Task
    {
        QString mode;
        QString sources;
        QString trackerType, trackerParams;
        QString appType, appParams;
    };

    explicit TaskManager(const QVector<Task> &tasks, QObject *parent = 0);

    void handleTasks();

private:
    QVector<Task> tasks;

    void getImageFiles(const QString &folder, QFileInfoList &files);
};

#endif // TASKMANAGER_H
