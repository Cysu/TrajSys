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
        QString sources;
        QString trackerType, trackerParams;
        bool trackerNeedDisplay;
        QString appType, appParams;
        bool appNeedDisplay;
    };

    explicit TaskManager(const QVector<Task> &tasks, QObject *parent = 0);

    void handleTasks();

private:
    QVector<Task> tasks;
};

#endif // TASKMANAGER_H
