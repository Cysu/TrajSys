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
    getTrackers(trackersType, trackersParams);

    QStringList appsType, appsParams;
    getApps(appsType, appsParams);

    QVector<TaskManager::Task> tasks;
    for (int i = 0; i < sources.size(); i ++) {
        TaskManager::Task task =
        {
            sources[i],
            trackersType[i],
            trackersParams[i],
            appsType[i],
            appsParams[i]
        };
        tasks.push_back(task);
    }

    TaskManager taskManager(tasks);
    taskManager.handleTasks();

    QDialog::accept();
}
