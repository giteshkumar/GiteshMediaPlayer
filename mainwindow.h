#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
struct MediaTrack {
    QString title;
    QString filePath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void openMedia();
    void playMedia();
    void pauseMedia();
    void stopMedia();

    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);

    void changeVolume(int value);
    void toggleMute();
    void seekBackward();
    void seekForward();
    void playNext();
    void playPrevious();
    void toggleRepeat();
    void toggleShuffle();
    void playCurrentTrack();

private:

    QString playlistFilePath();
    void testPlaylistFile();
    void savePlaylist();
    void loadPlaylist();

    Ui::MainWindow *ui;

    QMediaPlayer *player;

    QAudioOutput *audioOutput;

    QList<MediaTrack> playlist;

    int currentTrackIndex = 0;

    bool repeatEnabled = false;
    bool shuffleEnabled = false;


};

#endif // MAINWINDOW_H
