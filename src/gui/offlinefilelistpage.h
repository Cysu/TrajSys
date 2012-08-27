#ifndef OFFLINEFILELIST_H
#define OFFLINEFILELIST_H

#include <QtGui>

class OfflineFilelistPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit OfflineFilelistPage(QWidget *parent = 0);

    QStringList getFilePaths();

    int nextId() const;

signals:
    void itemChanged(QStringList filePaths);

private slots:
    void addFolder();
    void addVideo();
    void delItems();
    void delAll();
    
private:
    QListWidget *filelistWidget;
    QPushButton *addFolderPushButton;
    QPushButton *addVideoPushButton;
    QPushButton *delItemPushButton;
    QPushButton *delAllPushButton;
    
};

#endif // OFFLINEFILELIST_H
