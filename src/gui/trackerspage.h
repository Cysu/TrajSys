#ifndef TRACKERSPAGE_H
#define TRACKERSPAGE_H

#include <QtGui>

class TrackersPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TrackersPage(QWidget *parent = 0);

    void release();

    void initializePage();

    void getTrackers(QStringList &trackersTypes,
                     QStringList &trackersParams,
                     QVector<bool> &needDisplay);

private slots:
    void trackersTypesChanged(int cntIdx);
    void loadTrkFile();
    
private:
    int nrSources;
    QLabel *sourcesLabels;
    QCheckBox *needDisplayCheckBoxes;
    QComboBox *trackersTypesComboBoxes;
    QLineEdit *trackersParamsLineEdits;
    QPushButton *loadTrkFilePushButtons;

};

#endif // TRACKERSPAGE_H
