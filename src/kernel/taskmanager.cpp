#include "taskmanager.h"
#include "trackermanager.h"
#include "appmanager.h"
#include "sourcemanager.h"

#include <QProgressDialog>

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
        QString trackerOfPath = (task.sources == "Camera" || task.trackerType == "LOAD") ?
                    "" : ofPath+"("+task.trackerParams+").trk";
        QString appOfPath = (task.sources == "Camera") ?
                    "" : ofPath+"("+task.appParams+").cls";

        SourceManager sourceManager(task.sources.toStdString());
        TrackerManager trackerManager(task.trackerType.toStdString(),
                                      task.trackerParams.toStdString(),
                                      task.sources.toStdString(),
                                      trackerOfPath.toStdString(),
                                      task.trackerNeedDisplay);
        AppManager appManager(task.appType.toStdString(),
                              task.appParams.toStdString(),
                              trackerManager.getNrFeature(),
                              task.sources.toStdString(),
                              appOfPath.toStdString(),
                              task.appNeedDisplay);

        int pmax = sourceManager.getNrFrame();
        if (pmax == -1) pmax = 100;
        QProgressDialog progress("Processing task "+i, "Cancel", 0, pmax);
        progress.setWindowModality(Qt::WindowModal);

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

            if (task.trackerNeedDisplay || task.appNeedDisplay) {
                int t = cv::waitKey(50);
                if (t == 'r') appManager.clusterDisplayer->mode = 1;
                if (t == 'a') appManager.clusterDisplayer->mode = 0;
                if (t == 'i') appManager.clusterDisplayer->ignoreSmallCluster = true;
                if (t == 'n') appManager.clusterDisplayer->ignoreSmallCluster = false;
            }

            if (progress.wasCanceled()) {
                break;
            }
            if (sourceManager.getNrFrame() != -1)
                progress.setValue(frameIdx+1);
            else
                progress.setValue(50);

            trackerManager.getTrackPoints(frame, trackPoints);
            appManager.getResult(frame, trackPoints);
        }

        delete[] trackPoints;

        sourceManager.release();
        trackerManager.release();
        appManager.release();
    }
}
