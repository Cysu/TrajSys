#include "mainwizard.h"

MainWizard::MainWizard(QWidget *parent) :
    QWizard(parent),
    modePage(new ModePage),
    offlineFilelistPage(new OfflineFilelistPage),
    offlineTrackerPage(new OfflineTrackerPage)
{
    setWindowTitle(tr("Trajectory Analysis Wizard"));

    setPage(PAGE_MODE, modePage);
    setPage(PAGE_OFFLINE_FILELIST, offlineFilelistPage);
    setPage(PAGE_OFFLINE_TRACKER, offlineTrackerPage);

    setStartId(PAGE_MODE);

    connect(offlineFilelistPage, SIGNAL(itemChanged(QStringList)),
            offlineTrackerPage, SLOT(refreshFiles(QStringList)));
}

void MainWizard::accept()
{
    qDebug() << getOfflineFilePaths();
}

QStringList MainWizard::getOfflineFilePaths()
{
    return offlineFilelistPage->getFilePaths();
}
