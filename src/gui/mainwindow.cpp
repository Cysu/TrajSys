#include "mainwindow.h"
#include "guiutils.h"

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
                this, tr("Open Track File"), "D:/CST/MyProgram/TrajSys/output",
                tr("Track (*.trk)"));
    if (path.isEmpty()) return;

//    QString windowName = "Track Displayer";
//    cv::namedWindow(windowName.toStdString());

//    TrackDisplayer trackDisplayer;
//    trackDisplayer.display(windowName, path);

//    cv::destroyWindow(windowName.toStdString());
}

void MainWindow::displayCluster()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Clsuter File"), "D:/CST/MyProgram/TrajSys/output",
                tr("Cluster (*.cls)"));
    if (path.isEmpty()) return;

//    ClusterDisplayer clusterDisplayer(path);
//    clusterDisplayer.display();
}
