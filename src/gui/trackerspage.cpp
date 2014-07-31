#include "trackerspage.h"
#include "utils/const.h"
#include "mainwizard.h"

TrackersPage::TrackersPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Trackers"));
    setSubTitle(tr("Specify the tracker and parameters for each file."));

    release();
}

void TrackersPage::release()
{
    nrSources = 0;
    sourcesLabels = NULL;
    trackersTypesComboBoxes = NULL;
    trackersParamsLineEdits = NULL;
    loadTrkFilePushButtons = NULL;
    needDisplayCheckBoxes = NULL;
}

void TrackersPage::initializePage()
{
    MainWizard *mainWizard = (MainWizard *)wizard();
    QStringList sources;
    mainWizard->getSources(sources);

    release();

    nrSources = sources.size();

    sourcesLabels = new QLabel[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        QFileInfo file(sources[i]);
        sourcesLabels[i].setText(file.fileName());
        sourcesLabels[i].setToolTip(file.absoluteFilePath());
    }

    needDisplayCheckBoxes = new QCheckBox[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        needDisplayCheckBoxes[i].setToolTip("Display");
    }

    trackersTypesComboBoxes = new QComboBox[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        for (int j = 0; j < NR_TRACKERS; j ++)
            trackersTypesComboBoxes[i].addItem(TRACKERS_TYPES[j].c_str());
    }

    trackersParamsLineEdits = new QLineEdit[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        trackersParamsLineEdits[i].setText(TRACKERS_DEFAULT_PARAMS[0].c_str());
        trackersParamsLineEdits[i].setToolTip(tr("FeatureNum,ForegroundThres,StableTimeThres"));
    }

    loadTrkFilePushButtons = new QPushButton[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        loadTrkFilePushButtons[i].setText(tr("Choose"));
    }

    for (int i = 0; i < nrSources; i ++) {
        connect(&trackersTypesComboBoxes[i], SIGNAL(currentIndexChanged(int)),
                this, SLOT(trackersTypesChanged(int)));
        connect(&loadTrkFilePushButtons[i], SIGNAL(pressed()),
                this, SLOT(loadTrkFile()));
    }

    QGridLayout *subLayout = new QGridLayout;
    for (int i = 0; i < nrSources; i ++) {
        subLayout->addWidget(&sourcesLabels[i], i, 0);
        subLayout->addWidget(&needDisplayCheckBoxes[i], i, 1);
        subLayout->addWidget(&trackersTypesComboBoxes[i], i, 2);
        subLayout->addWidget(&trackersParamsLineEdits[i], i, 3);
    }

    QWidget *subWidget = new QWidget;
    subWidget->setLayout(subLayout);

    QScrollArea *scrollPanel = new QScrollArea;
    scrollPanel->setWidget(subWidget);
    scrollPanel->setWidgetResizable(true);

    QGridLayout *wholeLayout = new QGridLayout;
    wholeLayout->addWidget(scrollPanel, 0, 0);

    delete layout();
    setLayout(wholeLayout);
}

void TrackersPage::trackersTypesChanged(int cntIdx)
{
    QGridLayout *subLayout = new QGridLayout;
    for (int i = 0; i < nrSources; i ++) {
        subLayout->addWidget(&sourcesLabels[i], i, 0);
        subLayout->addWidget(&needDisplayCheckBoxes[i], i, 1);
        subLayout->addWidget(&trackersTypesComboBoxes[i], i, 2);
        subLayout->addWidget(&trackersParamsLineEdits[i], i, 3);
        int t = trackersTypesComboBoxes[i].currentIndex();
        if (t == NR_TRACKERS-1)
            subLayout->addWidget(&loadTrkFilePushButtons[i], i, 4);
    }

    QWidget *subWidget = new QWidget;
    subWidget->setLayout(subLayout);

    QScrollArea *scrollPanel = new QScrollArea;
    scrollPanel->setWidget(subWidget);
    scrollPanel->setWidgetResizable(true);

    QGridLayout *wholeLayout = new QGridLayout;
    wholeLayout->addWidget(scrollPanel, 0, 0);

    delete layout();
    setLayout(wholeLayout);
}

void TrackersPage::loadTrkFile()
{
    for (int i = 0; i < nrSources; i ++) {
        if (loadTrkFilePushButtons[i].isDown()) {
            QString path = QFileDialog::getOpenFileName(
                        this, tr("Open Trk File"), QDir::currentPath(),
                        tr("Trk (*.trk)"));
            if (path.isEmpty()) return;
            trackersParamsLineEdits[i].setText(path);
        }

    }
}

void TrackersPage::getTrackers(QStringList &trackersTypes,
                               QStringList &trackersParams,
                               QVector<bool> &needDisplay)
{
    for (int i = 0; i < nrSources; i ++) {
        trackersTypes.push_back(trackersTypesComboBoxes[i].currentText());
        trackersParams.push_back(trackersParamsLineEdits[i].text());
        needDisplay.push_back(needDisplayCheckBoxes[i].isChecked());
    }
}

