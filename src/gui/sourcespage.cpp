#include "sourcespage.h".h"
#include "mainwizard.h"

SourcesPage::SourcesPage(QWidget *parent) :
    QWizardPage(parent)
{
    release();
}

void SourcesPage::release() {
    selectFilesRadioButton = NULL;
    selectCameraRadioButton = NULL;
    selectSourceGroupBox = NULL;
    sourcesListWidget = NULL;
    addFolderPushButton = NULL;
    addVideoPushButton = NULL;
    delItemsPushButton = NULL;
    delAllPushButton = NULL;
    selectFilesGroupBox = NULL;
}

void SourcesPage::initializePage()
{
    release();

    setTitle(tr("Sources"));
    setSubTitle(tr("Specify the sources to be processed."));

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

    QGridLayout *selectFilesGroupBoxLayout = new QGridLayout;
    selectFilesGroupBoxLayout->addWidget(sourcesListWidget, 0, 0, 1, 12);
    selectFilesGroupBoxLayout->addWidget(addFolderPushButton, 1, 0);
    selectFilesGroupBoxLayout->addWidget(addVideoPushButton, 1, 1);
    selectFilesGroupBoxLayout->addWidget(delItemsPushButton, 1, 2);
    selectFilesGroupBoxLayout->addWidget(delAllPushButton, 1, 3);

    selectFilesGroupBox = new QGroupBox(tr("&Select Files"));
    selectFilesGroupBox->setLayout(selectFilesGroupBoxLayout);

    selectFilesRadioButton = new QRadioButton(tr("&Files"));
    selectCameraRadioButton = new QRadioButton(tr("&Camera"));
    selectCameraRadioButton->setChecked(true);

    connect(selectFilesRadioButton, SIGNAL(clicked()), this, SLOT(selectFiles()));
    connect(selectCameraRadioButton, SIGNAL(clicked()), this, SLOT(selectCamera()));

    selectSourceGroupBox = new QGroupBox(tr("&Select Sources"));

    QGridLayout *selectSourceGroupBoxLayout = new QGridLayout;
    selectSourceGroupBoxLayout->addWidget(selectFilesRadioButton, 0, 0);
    selectSourceGroupBoxLayout->addWidget(selectCameraRadioButton, 0, 1);
    selectSourceGroupBox->setLayout(selectSourceGroupBoxLayout);

    QGridLayout *sourcePageLayout = new QGridLayout;
    sourcePageLayout->addWidget(selectSourceGroupBox, 0, 0);

    delete layout();
    setLayout(sourcePageLayout);
}

void SourcesPage::getSources(QStringList &sources)
{
    if (selectCameraRadioButton->isChecked()) {
        sources.push_back("Camera");
    } else {
        for (int i = 0; i < sourcesListWidget->count(); i ++)
            sources.push_back(sourcesListWidget->item(i)->text());
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

void SourcesPage::selectFiles()
{
    QGridLayout *onlineSourcePageLayout = new QGridLayout;
    onlineSourcePageLayout->addWidget(selectSourceGroupBox, 0, 0);
    onlineSourcePageLayout->addWidget(selectFilesGroupBox, 1, 0);

    delete layout();
    setLayout(onlineSourcePageLayout);
}

void SourcesPage::selectCamera()
{
    QGridLayout *onlineSourcePageLayout = new QGridLayout;
    onlineSourcePageLayout->addWidget(selectSourceGroupBox, 0, 0);

    delete layout();
    setLayout(onlineSourcePageLayout);
}
