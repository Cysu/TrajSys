#include "taskmanager.h"
#include "trackermanager.h"
#include "appmanager.h"
#include "kernelutils.h"

TaskManager::TaskManager(const QVector<Task> &tasks, QObject *parent) :
    QObject(parent),
    tasks(tasks)
{
}

void TaskManager::handleTasks()
{
    for (int i = 0; i < tasks.size(); i ++) {
        Task &task = tasks[i];

        if (task.mode == "Offline") {

            QFileInfo src(task.sources);
            if (src.isDir()) {
                QFileInfoList files;
                getImageFiles(task.sources, files);

                TrackSet trackSet;

                TrackerManager trackerManager(
                            &files,
                            task.trackerType, task.trackerParams,
                            &trackSet,
                            "D:/CST/MyProgram/TrajSys/output/"+src.fileName());
                trackerManager.work();

                AppManager appManager(
                            &files, &trackSet,
                            task.appType, task.appParams,
                            "D:/CST/MyProgram/TrajSys/output/"+src.fileName());
                appManager.work();
            } else {

            }

        } else {

        }
    }
}

void TaskManager::getImageFiles(const QString &folder, QFileInfoList &files)
{
    QDir dir(folder);
    QStringList imgFilters;
    imgFilters << "*.bmp" << "*.jpg"<< "*.tiff" << "*.jpeg" << "*.png";
    dir.setNameFilters(imgFilters);
    files = dir.entryInfoList();
}
