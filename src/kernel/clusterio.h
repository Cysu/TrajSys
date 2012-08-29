#ifndef CLUSTERIO_H
#define CLUSTERIO_H

#include <QObject>
#include <QString>

#include "kernelutils.h"

class ClusterIO : public QObject
{
    Q_OBJECT

public:
    explicit ClusterIO(QObject *parent = 0);
    
    void setInput(const QString &filePath);
    bool readPath(QString &path);
    bool readCluster(Cluster &cluster);
    void closeInput();

    void setOutput(const QString &filePath);
    void writePath(const QString &path);
    void writeCluster(const Cluster &cluster);
    void closeOutput();

private:
    FILE *fin, *fout;
    
};

#endif // CLUSTERIO_H
