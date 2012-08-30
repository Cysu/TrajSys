#include "sourcespage.h".h"
#include "mainwizard.h"

SourcesPage::SourcesPage(QWidget *parent) :
    QWizardPage(parent)
{
    release();
}

void SourcesPage::release() {
    sourcesListWidget = NULL;
    addFolderPushButton = NULL;
    addVideoPushButton = NULL;
    delItemsPushButton = NULL;
    delAllPushButton = NULL;
}

void SourcesPage::initializePage()
{
    release();

    MainWizard *mainWizard = (MainWizard *)wizard();

    QString mode;
    mainWizard->getMode(mode);

    if (mode == QString("Offline")) {
        setTitle(tr("Offline Sources"));
        setSubTitle(tr("Specify the videos and folders to be processed offline."));

        QIcon addFolderIcon(QPixmap("../res/offline_filelist_addfolder.png"));
        QIcon addVideoIcon(QPixmap("../res/offline_filelist_addvideo.png"));
        QIcon delItemIcon(QPixmap("../res/offline_filelist_delitem.png"));
        QIcon delAllIcon(QPixmap("../res/offline_filelist_delall.png"));

        addFolderPushButton = new QPushButton;
        addVideoPushButton = new QPushButton;
        delItemsPushButton = new QPushButton;
        delAllPushButton = new QPushButton;
        addFolderPushButton->setIcon(addFolderIcon);
        addVideoPushButton->setIcon(addVideoIcon);
        delItemsPushButton->setIcon(delItemIcon);
        delAllPushButton->setIcon(delAllIcon);
        addFolderPushButton->setToolTip(tr("Add a folder"));
        addVideoPushButton->setToolTip(tr("Add a video"));
        delItemsPushButton->setToolTip(tr("Delete the selected items"));
        delAllPushButton->setToolTip(tr("Delete all the items"));

        connect(addFolderPushButton, SIGNAL(clicked()), this, SLOT(addFolder()));
        connect(addVideoPushButton, SIGNAL(clicked()), this, SLOT(addVideo()));
        connect(delItemsPushButton, SIGNAL(clicked()), this, SLOT(delItems()));
        connect(delAllPushButton, SIGNAL(clicked()), this, SLOT(delAll()));

        sourcesListWidget = new QListWidget;
        sourcesListWidget->setSelectionMode(QListWidget::MultiSelection);

        QGridLayout *offlineSourcesPageLayout = new QGridLayout;
        offlineSourcesPageLayout->addWidget(sourcesListWidget, 0, 0, 1, 12);
        offlineSourcesPageLayout->addWidget(addFolderPushButton, 1, 0);
        offlineSourcesPageLayout->addWidget(addVideoPushButton, 1, 1);
        offlineSourcesPageLayout->addWidget(delItemsPushButton, 1, 2);
        offlineSourcesPageLayout->addWidget(delAllPushButton, 1, 3);

        delete layout();
        setLayout(offlineSourcesPageLayout);
    } else if (mode == QString("Online")) {
        setTitle(tr("Online Sources"));
        setSubTitle(tr("Specify the videos and folders to be processed offline."));

        selectFolderRadioButton = new QRadioButton(tr("&Folder"));
        selectVideoRadioButton = new QRadioButton(tr("&Video"));
        selectCameraRadioButton = new QRadioButton(tr("&Camera"));
        selectSourceGroupBox = new QGroupBox;
        selectCameraRadioButton->setChecked(true);

        selectFolderLineEdit = new QLineEdit;
        selectVideoLineEdit = new QLineEdit;

        QIcon selectFolderIcon(QPixmap("../res/offline_filelist_addfolder.png"));
        QIcon selectVideoIcon(QPixmap("../res/offline_filelist_addvideo.png"));

        selectFolderPushButton = new QPushButton;
        selectVideoPushButton = new QPushButton;
        selectFolderPushButton->setIcon(selectFolderIcon);
        selectVideoPushButton->setIcon(selectVideoIcon);

        connect(selectFolderPushButton, SIGNAL(clicked()), this, SLOT(selectFolder()));
        connect(selectVideoPushButton, SIGNAL(clicked()), this, SLOT(selectVideo()));

        QGridLayout *selectSourceGroupBoxLayout = new QGridLayout;
        selectSourceGroupBoxLayout->addWidget(selectFolderRadioButton, 0, 0);
        selectSourceGroupBoxLayout->addWidget(selectVideoRadioButton, 1, 0);
        selectSourceGroupBoxLayout->addWidget(selectCameraRadioButton, 2, 0);
        selectSourceGroupBoxLayout->addWidget(selectFolderLineEdit, 0, 1);
        selectSourceGroupBoxLayout->addWidget(selectVideoLineEdit, 1, 1);
        selectSourceGroupBoxLayout->addWidget(selectFolderPushButton, 0, 2);
        selectSourceGroupBoxLayout->addWidget(selectVideoPushButton, 1, 2);
        selectSourceGroupBox->setLayout(selectSourceGroupBoxLayout);

        QGridLayout *onlineSourcePageLayout = new QGridLayout;
        onlineSourcePageLayout->addWidget(selectSourceGroupBox, 0, 0);

        delete layout();
        setLayout(onlineSourcePageLayout);
    }

}

void SourcesPage::getSources(QStringList &sources)
{
    MainWizard *mainWizard = (MainWizard *)wizard();

    QString mode;
    mainWizard->getMode(mode);

    if (mode == "Offline") {
        for (int i = 0; i < sourcesListWidget->count(); i ++)
            sources.push_back(sourcesListWidget->item(i)->text());
    } else if (mode == "Online") {
        if (selectFolderRadioButton->isChecked() && selectFolderLineEdit->text() != "")
            sources.push_back(selectFolderLineEdit->text());
        else if (selectVideoRadioButton->isChecked() && selectVideoLineEdit->text() != "")
            sources.push_back(selectVideoLineEdit->text());
        else if (selectCameraRadioButton->isChecked())
            sources.push_back("Camera");
    }
}

void SourcesPage::addFolder()
{
    QString path = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), QDir::currentPath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;
    sourcesListWidget->addItem(path);
}

void SourcesPage::addVideo()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Video"), QDir::currentPath(),
                tr("Videos (*.avi *.wmv)"));
    if (path.isEmpty()) return;
    sourcesListWidget->addItem(path);
}

void SourcesPage::delItems()
{
    QList<QListWidgetItem *> selectedItems = sourcesListWidget->selectedItems();
    for (int i = 0; i < selectedItems.size(); i ++)
        sourcesListWidget->takeItem(sourcesListWidget->row(selectedItems[i]));
}

void SourcesPage::delAll()
{
    sourcesListWidget->clear();
}

void SourcesPage::selectFolder()
{
    QString path = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), QDir::currentPath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;
    selectFolderLineEdit->setText(path);
}

void SourcesPage::selectVideo()
{
    QString path = QFileDialog::getOpenFileName(
                this, tr("Open Video"), QDir::currentPath(),
                tr("Videos (*.avi *.wmv)"));
    if (path.isEmpty()) return;
    selectVideoLineEdit->setText(path);
}
