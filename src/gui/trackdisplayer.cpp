#include "trackdisplayer.h"
#include "kernel/trackio.h"
#include "gui/guiutils.h"

TrackDisplayer::TrackDisplayer(const QString &filePath,
                               QObject *parent) :
    QObject(parent),
    filePath(filePath)
{
}

void TrackDisplayer::display()
{
    TrackIO trackIO;
    trackIO.setInput(filePath);

    QString folderPath;
    if (!trackIO.readPath(folderPath)) return;

    QFileInfoList files;
    getImageFiles(folderPath, files);

    TrackSet trackSet;
    while (true) {
        Track track;
        if (!trackIO.readTrack(track)) break;
        trackSet.push_back(track);
    }

    trackIO.closeInput();

    cv::namedWindow("Track Displayer");
    for (int i = 0; i < files.size(); i ++) {
        cv::Mat img = cv::imread(files.at(i).filePath().toStdString());
        drawTrack(img, trackSet, i);
        cv::imshow("Track Displayer", img);
        if (cv::waitKey(100) >= 0) break;
    }
    cv::destroyWindow("Track Displayer");
}

void TrackDisplayer::drawTrack(cv::Mat &img, const TrackSet &trackSet, int time)
{
    for (int i = 0; i < trackSet.size(); i ++) {
        const Track &track = trackSet[i];
        if (track.size() < 5) continue;
        if (track[0].t > time || track.back().t < time) continue;

        int j = time-track[0].t;
        if (track[j].t != time) continue;

        int s = std::max(0, j-20);
        cv::Point p(track[s].x, track[s].y), q;

        for (int k = s+1; k <= j; k ++) {
            q = cv::Point(track[k].x, track[k].y);
            cv::line(img, p, q, EXAMPLE_COLOR[i%6], 3);
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
