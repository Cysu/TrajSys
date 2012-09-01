#include "taskmanager.h"
#include "trackermanager.h"
#include "appmanager.h"
#include "sourcemanager.h"

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
        QString ofPath = "../output/" + QFileInfo(task.sources).fileName();
        QString trackerOfPath = (task.sources == "Camera") ?
                    "" : ofPath+"("+task.trackerParams+").trk";
        QString appOfPath = (task.sources == "Camera") ?
                    "" : ofPath+"("+task.appParams+").cls";

        SourceManager sourceManager(task.sources);
        TrackerManager trackerManager(task.trackerType, task.trackerParams,
                                      task.sources, trackerOfPath);
        AppManager appManager(task.appType, task.appParams, trackerManager.getNrFeature(),
                              task.sources, appOfPath);

        cv::Mat frame;
        TrackPoint *trackPoints = new TrackPoint[trackerManager.getNrFeature()];

        if (trackerManager.getFgThres() > 0) {
            int frameIdx = sourceManager.getNextFrame(frame);
            if (frameIdx == -1) return;
            trackerManager.recordBgFrame(frame);
        }

        while (true) {
            int frameIdx = sourceManager.getNextFrame(frame);
            if (frameIdx == -1) break;

            trackerManager.getTrackPoints(frame, trackPoints);

            appManager.getResult(frame, trackPoints);
        }

        delete[] trackPoints;

        sourceManager.release();
        trackerManager.release();
        appManager.release();
    }

            /*if (task.sources == "Camera") {

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
            } else
        }
    }*/
}
