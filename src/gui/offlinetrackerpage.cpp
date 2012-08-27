#include "offlinetrackerpage.h"
#include "utils/const.h"

OfflineTrackerPage::OfflineTrackerPage(QWidget *parent) :
    QWizardPage(parent),
    fileLabels(NULL),
    trackerComboBoxes(NULL),
    trackerParamsEdits(NULL)
{
}

void OfflineTrackerPage::refreshFiles(QStringList filePaths) {
    release();

    int nrFiles = filePaths.size();
    printf("%d\n", nrFiles);

    fileLabels = new QLabel[nrFiles];
    for (int i = 0; i < nrFiles; i ++) {
        QFileInfo file(filePaths[i]);
        fileLabels[i].setText(file.fileName());
    }

    trackerComboBoxes = new QComboBox[nrFiles];
    for (int i = 0; i < nrFiles; i ++) {
        for (int j = 0; j < NR_TRACKER_TYPE; j ++)
            trackerComboBoxes[i].addItem(TRACKER_TYPE[j]);
    }

    trackerParamsEdits = new QLineEdit[nrFiles];
    for (int i = 0; i < nrFiles; i ++) {
        trackerParamsEdits[i].setText(TRACKER_DEFAULT_PARAMS[0]);
    }

    QGridLayout *subLayout = new QGridLayout;
    for (int i = 0; i < nrFiles; i ++) {
        subLayout->addWidget(&fileLabels[i], i, 0);
        subLayout->addWidget(&trackerComboBoxes[i], i, 1);
        subLayout->addWidget(&trackerParamsEdits[i], i, 2);
    }

    QScrollArea *scrollPanel = new QScrollArea;
    scrollPanel->setLayout(subLayout);

    QGridLayout *wholeLayout = new QGridLayout;
    wholeLayout->addWidget(scrollPanel);

    setLayout(wholeLayout);
}

int OfflineTrackerPage::nextId() const {
    return -1;
}

void OfflineTrackerPage::release() {
    delete layout();
    qDeleteAll(children());
    fileLabels = NULL;
    trackerComboBoxes = NULL;
    trackerParamsEdits = NULL;
}
