#include "trackdisplayer.h"
#include "kernel/trackio.h"
#include "gui/guiutils.h"

TrackDisplayer::TrackDisplayer(QObject *parent) :
    QObject(parent)
{
}

void TrackDisplayer::display(const QString &window, const QString &trkFile)
{
    TrackIO trackIO;
    trackIO.setInput(trkFile);

    QString folder;
    if (!trackIO.readPath(folder)) return;

    QFileInfoList files;
    getImageFiles(folder, files);

    TrackSet trackSet;
    while (true) {
        Track track;
        if (!trackIO.readTrack(track)) break;
        trackSet.push_back(track);
    }

    trackIO.closeInput();

    for (int i = 0; i < files.size(); i ++) {
        cv::Mat img = cv::imread(files.at(i).filePath().toStdString());
        drawTrack(img, trackSet, i);
        cv::imshow(window.toStdString(), img);
        if (cv::waitKey(100) >= 0) break;
    }
}

void TrackDisplayer::display(const QString &window, const TrackSet &trackSet, cv::Mat &img)
{
    drawTrack(img, trackSet);
    cv::imshow(window.toStdString(), img);
}

void TrackDisplayer::drawTrack(cv::Mat &img, const TrackSet &trackSet, int time)
{
    for (int i = 0; i < trackSet.size(); i ++) {
        const Track &track = trackSet[i];
        if (track.size() < 5) continue;

        int j;
        if (time != -1) {
            if (track[0].t > time || track.back().t < time) continue;
            j = time-track[0].t;
            if (track[j].t != time) continue;
        } else {
            j = track.size()-1;
        }

        int s = std::max(0, j-5);
        cv::Point p(track[s].x, track[s].y), q;

        for (int k = s+1; k <= j; k ++) {
            q = cv::Point(track[k].x, track[k].y);
            cv::line(img, p, q, EXAMPLE_COLOR[i%6], 1);
            p = q;
        }
    }
}

void TrackDisplayer::getImageFiles(const QString &folder, QFileInfoList &files)
{
    QDir dir(folder);
    QStringList imgFilters;
    imgFilters << "*.bmp" << "*.jpg"<< "*.tiff" << "*.jpeg" << "*.png";
    dir.setNameFilters(imgFilters);
    files = dir.entryInfoList();
}
