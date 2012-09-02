#include "mainwizard.h"
#include "kernel/taskmanager.h"

MainWizard::MainWizard(QWidget *parent) :
    QWizard(parent),
    sourcesPage(new SourcesPage),
    trackersPage(new TrackersPage),
    appsPage(new AppsPage)
{
    setWindowTitle(tr("Trajectory Analysis Wizard"));

    addPage(sourcesPage);
    addPage(trackersPage);
    addPage(appsPage);
}

void MainWizard::accept()
{
    QStringList sources;
    getSources(sources);

    QStringList trackersType, trackersParams;
    QVector<bool> trackersNeedDisplay;
    getTrackers(trackersType, trackersParams, trackersNeedDisplay);

    QStringList appsType, appsParams;
    QVector<bool> appsNeedDisplay;
    getApps(appsType, appsParams, appsNeedDisplay);

    QVector<TaskManager::Task> tasks;
    for (int i = 0; i < sources.size(); i ++) {
        TaskManager::Task task =
        {
            sources[i],
            trackersType[i],
            trackersParams[i],
            trackersNeedDisplay[i],
            appsType[i],
            appsParams[i],
            appsNeedDisplay[i]
        };
        tasks.push_back(task);
    }

    QDialog::accept();

    TaskManager taskManager(tasks);
    taskManager.handleTasks();
}
