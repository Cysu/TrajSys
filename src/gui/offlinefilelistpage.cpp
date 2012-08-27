#include "offlinefilelistpage.h"
#include "mainwizard.h"

OfflineFilelistPage::OfflineFilelistPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Offline File List"));
    setSubTitle(tr("Specify the videos and folders to be processed."));


    QIcon addFolderIcon(QPixmap("../res/offline_filelist_addfolder.png"));
    QIcon addVideoIcon(QPixmap("../res/offline_filelist_addvideo.png"));
    QIcon delItemIcon(QPixmap("../res/offline_filelist_delitem.png"));
    QIcon delAllIcon(QPixmap("../res/offline_filelist_delall.png"));

    addFolderPushButton = new QPushButton;
    addVideoPushButton = new QPushButton;
    delItemPushButton = new QPushButton;
    delAllPushButton = new QPushButton;
    addFolderPushButton->setIcon(addFolderIcon);
    addVideoPushButton->setIcon(addVideoIcon);
    delItemPushButton->setIcon(delItemIcon);
    delAllPushButton->setIcon(delAllIcon);
    addFolderPushButton->setToolTip(tr("Add a folder"));
    addVideoPushButton->setToolTip(tr("Add a video"));
    delItemPushButton->setToolTip(tr("Delete the selected items"));
    delAllPushButton->setToolTip(tr("Delete all the items"));

    connect(addFolderPushButton, SIGNAL(clicked()), this, SLOT(addFolder()));
    connect(addVideoPushButton, SIGNAL(clicked()), this, SLOT(addVideo()));
    connect(delItemPushButton, SIGNAL(clicked()), this, SLOT(delItems()));
    connect(delAllPushButton, SIGNAL(clicked()), this, SLOT(delAll()));

    filelistWidget = new QListWidget;
    filelistWidget->setSelectionMode(QListWidget::MultiSelection);


    QGridLayout *offlineFilelistPageLayout = new QGridLayout;
    offlineFilelistPageLayout->addWidget(filelistWidget, 0, 0, 1, 12);
    offlineFilelistPageLayout->addWidget(addFolderPushButton, 1, 0);
    offlineFilelistPageLayout->addWidget(addVideoPushButton, 1, 1);
    offlineFilelistPageLayout->addWidget(delItemPushButton, 1, 2);
    offlineFilelistPageLayout->addWidget(delAllPushButton, 1, 3);

    setLayout(offlineFilelistPageLayout);
}

QStringList OfflineFilelistPage::getFilePaths()
{
    QStringList ret;
    for (int i = 0; i < filelistWidget->count(); i ++) {
        ret.push_back(filelistWidget->item(i)->text());
    }
    return ret;
}

int OfflineFilelistPage::nextId() const
{
    return MainWizard::PAGE_OFFLINE_TRACKER;
}

void OfflineFilelistPage::addFolder()
{
    QString path = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), QDir::currentPath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;
    filelistWidget->addItem(path);

    emit itemChanged(getFilePaths());
}

void OfflineFilelistPage::addVideo()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Video"), QDir::currentPath(),
                tr("Videos (*.avi *.wmv)"));
    if (path.isEmpty()) return;
    filelistWidget->addItem(path);

    emit itemChanged(getFilePaths());
}

void OfflineFilelistPage::delItems()
{
    QList<QListWidgetItem *> selectedItems = filelistWidget->selectedItems();
    for (int i = 0; i < selectedItems.size(); i ++) {
        filelistWidget->takeItem(filelistWidget->row(selectedItems[i]));
    }

    emit itemChanged(getFilePaths());
}

void OfflineFilelistPage::delAll()
{
    filelistWidget->clear();

    emit itemChanged(getFilePaths());
}
