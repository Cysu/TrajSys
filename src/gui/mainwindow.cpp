#include "mainwindow.h"
#include "guiutils.h"
#include "utils/trackdisplayer.h"
#include "utils/clusterdisplayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mainWizard(NULL)
{
    setWindowTitle(tr("Trajectory Analysis System"));
    showMaximized();

    createMenu();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createMenu()
{
    CREATE_ACTION(openWizardAction, "&Wizard", "Ctrl+N", openWizard);

    CREATE_ACTION(displayTrackAction, "&Track", "", displayTrack);
    CREATE_ACTION(displayClusterAction, "&Cluster", "", displayCluster);

    QMenu *displayMenu = new QMenu(tr("&Display"), this);
    displayMenu->addAction(displayTrackAction);
    displayMenu->addAction(displayClusterAction);

    menuBar()->addAction(openWizardAction);
    menuBar()->addMenu(displayMenu);

}

void MainWindow::openWizard()
{
    mainWizard = new MainWizard;
    mainWizard->show();
}

void MainWindow::displayTrack()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Track File"), "../output",
                tr("Track (*.trk)"));
    if (path.isEmpty()) return;

    TrackDisplayer trackDisplayer(path.toStdString());
    trackDisplayer.disp();
    trackDisplayer.release();
}

void MainWindow::displayCluster()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Clsuter File"), "../output",
                tr("Cluster (*.cls)"));
    if (path.isEmpty()) return;

    ClusterDisplayer clusterDisplayer(path.toStdString());
    clusterDisplayer.disp();
    clusterDisplayer.release();
}
