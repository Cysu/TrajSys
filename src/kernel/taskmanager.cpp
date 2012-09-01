#include "taskmanager.h"
#include "trackermanager.h"
#include "appmanager.h"
#include "kernelutils.h"
#include "gui/trackdisplayer.h"

#include <opencv2/opencv.hpp>


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

                TrackerManager trackerManager(task.trackerType, task.trackerParams);
                trackerManager.getFromFiles(
                            files, &trackSet,
                            "../output/"+src.fileName()+
                            "_("+task.trackerParams+").trk");

                AppManager appManager(task.appType, task.appParams);
                appManager.saveResult(
                            trackSet,
                            files.at(0).absolutePath(),
                            "../output/"+src.fileName()+
                            "_("+task.appParams+")");

            } else {

            }

        } else if (task.mode == "Online") {
            if (task.sources == "Camera") {

                cv::VideoCapture videoCapture(0);
                if (!videoCapture.isOpened()) return;

                TrackerManager trackerManager(task.trackerType, task.trackerParams);
                TrackDisplayer trackDisplayer;

                AppManager appManager(task.appType, task.appParams);

                QString windowName = "Cluster Displayer";
                cv::Mat img, grayimg;
                cv::namedWindow(windowName.toStdString());

                for (int frameIdx = 0; ; frameIdx ++) {
                    if (cv::waitKey(10) >= 0) break;

                    videoCapture >> img;

                    if (frameIdx < 30) continue;

                    CvSize size = {320, 240};
                    cv::resize(img, img, size);
                    cv::cvtColor(img, grayimg, CV_BGR2GRAY);

                    TrackSet trackSet;
                    bool *mark;
                    trackerManager.getFromImage(grayimg, &trackSet, mark);

//                    trackDisplayer.display(windowName, trackSet, img);

                    appManager.displayResult(windowName, trackSet, mark, img);

                }

                cv::destroyWindow(windowName.toStdString());
                trackerManager.finish();
            }
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
