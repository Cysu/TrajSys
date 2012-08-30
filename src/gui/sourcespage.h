#ifndef SOURCESPAGE_H
#define SOURCESPAGE_H

#include <QtGui>

class SourcesPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SourcesPage(QWidget *parent = 0);

    void release();

    void initializePage();

    void getSources(QStringList &sources);

private slots:
    void addFolder();
    void addVideo();
    void delItems();
    void delAll();

    void selectFolder();
    void selectVideo();
    
private:
    QListWidget *sourcesListWidget;
    QPushButton *addFolderPushButton;
    QPushButton *addVideoPushButton;
    QPushButton *delItemsPushButton;
    QPushButton *delAllPushButton;

    QRadioButton *selectFolderRadioButton;
    QRadioButton *selectVideoRadioButton;
    QRadioButton *selectCameraRadioButton;
    QGroupBox *selectSourceGroupBox;
    QLineEdit *selectFolderLineEdit;
    QLineEdit *selectVideoLineEdit;
    QPushButton *selectFolderPushButton;
    QPushButton *selectVideoPushButton;
    
};

#endif // SOURCESPAGE_H
