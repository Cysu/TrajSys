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
                     QStringList &trackersParams);
    
private:
    int nrSources;
    QLabel *sourcesLabels;
    QComboBox *trackersTypesComboBoxes;
    QLineEdit *trackersParamsLineEdits;

};

#endif // TRACKERSPAGE_H
