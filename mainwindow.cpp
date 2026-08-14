#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QUrl>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create media player
    player = new QMediaPlayer(this);

    // Create audio output
    audioOutput = new QAudioOutput(this);

    // Connect audio output to player
    player->setAudioOutput(audioOutput);

    // Default volume = 50%
    audioOutput->setVolume(0.5);

    // Connect buttons
    connect(ui->openButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openMedia);

    connect(ui->playButton,
            &QPushButton::clicked,
            this,
            &MainWindow::playMedia);

    connect(ui->pauseButton,
            &QPushButton::clicked,
            this,
            &MainWindow::pauseMedia);

    connect(ui->stopButton,
            &QPushButton::clicked,
            this,
            &MainWindow::stopMedia);

    // Update progress bar
    connect(player,
            &QMediaPlayer::positionChanged,
            this,
            &MainWindow::updatePosition);

    connect(player,
            &QMediaPlayer::durationChanged,
            this,
            &MainWindow::updateDuration);

    connect(ui->volumeSlider,
            &QSlider::valueChanged,
            this,
            &MainWindow::changeVolume);

    connect(ui->muteButton,
            &QPushButton::clicked,
            this,
            &MainWindow::toggleMute);

    connect(ui->backwardButton,
            &QPushButton::clicked,
            this,
            &MainWindow::seekBackward);

    connect(ui->forwardButton,
            &QPushButton::clicked,
            this,
            &MainWindow::seekForward);

    connect(ui->previousButton,
            &QPushButton::clicked,
            this,
            &MainWindow::playPrevious);

    connect(ui->nextButton,
            &QPushButton::clicked,
            this,
            &MainWindow::playNext);

    connect(ui->repeatButton,
            &QPushButton::clicked,
            this,
            &MainWindow::toggleRepeat);

    connect(ui->shuffleButton,
            &QPushButton::clicked,
            this,
            &MainWindow::toggleShuffle);
}


void MainWindow::openMedia()
{
    QString fileName =
        QFileDialog::getOpenFileName(
            this,
            "Open Media",
                QDir::homePath(),
                        "Media Files (*.mp3 *.wav *.ogg *.mp4 *.avi *.mkv *.m4a);;"
                        "Audio Files (*.mp3 *.wav *.ogg *.m4a);;"
                        "Video Files (*.mp4 *.avi *.mkv);;"
                        "All Files (*)"
                    );

                    if (fileName.isEmpty())
                        return;

                      QUrl mediaUrl = QUrl::fromLocalFile(fileName);

                      playlist.append(mediaUrl);

                      currentTrackIndex = playlist.size() - 1;

                      player->setSource(mediaUrl);

                      player->play();
                }
/*
 * Play
 */

void MainWindow::playMedia()
{
    player->play();
}


/*
 * Pause
 */

void MainWindow::pauseMedia()
{
    player->pause();
}


/*
 * Stop
 */

void MainWindow::stopMedia()
{
    player->stop();
}


/*
 * Update progress bar
 */

/*
 * Update current playback position
 */
/*
 * Update current playback position
 */
void MainWindow::updatePosition(qint64 position)
{
    ui->progressSlider->setValue(static_cast<int>(position));

    int seconds = static_cast<int>(position / 1000);
    int minutes = seconds / 60;
    seconds = seconds % 60;

    QString time =
        QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

    ui->currentTimeLabel->setText(time);
}


void MainWindow::updateDuration(qint64 duration)
{
    ui->progressSlider->setRange(
        0,
        static_cast<int>(duration)
    );

    int seconds = static_cast<int>(duration / 1000);
    int minutes = seconds / 60;
    seconds = seconds % 60;

    QString time =
        QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

    ui->durationLabel->setText(time);
}

void MainWindow::changeVolume(int value)
{
    audioOutput->setVolume(value / 100.0);
}

void MainWindow::toggleMute()
{
    if (audioOutput->isMuted())
    {
        audioOutput->setMuted(false);
        ui->muteButton->setText("🔊 Mute");
    }
    else
    {
        audioOutput->setMuted(true);
        ui->muteButton->setText("🔇 Unmute");
    }
}
void MainWindow::seekBackward()
{
    qint64 newPosition = player->position() - 10000;

    if (newPosition < 0)
        newPosition = 0;

    player->setPosition(newPosition);
}
void MainWindow::seekForward()
{
    qint64 newPosition = player->position() + 10000;

    if (newPosition > player->duration())
        newPosition = player->duration();

    player->setPosition(newPosition);
}
void MainWindow::toggleRepeat()
{
    repeatEnabled = !repeatEnabled;

    if (repeatEnabled)
    {
        ui->repeatButton->setText("🔁 Repeat ON");
    }
    else
    {
        ui->repeatButton->setText("🔁 Repeat");
    }
}
void MainWindow::playNext()
{
    if (playlist.isEmpty())
        return;

    if (shuffleEnabled)
    {
        currentTrackIndex =
            QRandomGenerator::global()->bounded(
                playlist.size()
            );
    }
    else
    {
        currentTrackIndex++;

        if (currentTrackIndex >= playlist.size())
        {
            if (repeatEnabled)
                currentTrackIndex = 0;
            else
                currentTrackIndex = playlist.size() - 1;
        }
    }

    playCurrentTrack();
}
void MainWindow::playPrevious()
{
    if (playlist.isEmpty())
        return;

    currentTrackIndex--;

    if (currentTrackIndex < 0)
    {
        if (repeatEnabled)
            currentTrackIndex = playlist.size() - 1;
        else
            currentTrackIndex = 0;
    }

    playCurrentTrack();
}
void MainWindow::playCurrentTrack()
{
    if (playlist.isEmpty())
        return;

    if (currentTrackIndex < 0 ||
        currentTrackIndex >= playlist.size())
        return;

    player->setSource(
        playlist.at(currentTrackIndex)
    );

    player->play();
}
void MainWindow::toggleShuffle()
{
    shuffleEnabled = !shuffleEnabled;

    if (shuffleEnabled)
    {
        ui->shuffleButton->setText("🔀 Shuffle ON");
    }
    else
    {
        ui->shuffleButton->setText("🔀 Shuffle");
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
