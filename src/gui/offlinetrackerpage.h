#ifndef OFFLINETRACKERPAGE_H
#define OFFLINETRACKERPAGE_H

#include <QtGui>

class OfflineTrackerPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit OfflineTrackerPage(QWidget *parent = 0);

    void release();

    int nextId() const;


public slots:
    void refreshFiles(QStringList filePaths);
    
private:
    QLabel *fileLabels;
    QComboBox *trackerComboBoxes;
    QLineEdit *trackerParamsEdits;

};

#endif // OFFLINETRACKERPAGE_H
