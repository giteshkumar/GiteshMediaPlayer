#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Playlist XML:"
             << playlistFilePath();

    loadPlaylist();

    // Create media player
    player = new QMediaPlayer(this);

    // Create audio output
    audioOutput = new QAudioOutput(this);

    // Connect audio output to player
    player->setAudioOutput(audioOutput);
    testPlaylistFile();

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

        // Create MediaTrack
        MediaTrack track;

        QFileInfo fileInfo(fileName);

        track.title = fileInfo.completeBaseName();
        track.filePath = fileName;

        // Add MediaTrack to playlist
        playlist.append(track);

        // Make the newly added track current
        currentTrackIndex = playlist.size() - 1;
            qDebug() << "Track added:";
            qDebug() << "Title:" << track.title;
            qDebug() << "Path:" << track.filePath;
            qDebug() << "Playlist size:" << playlist.size();
        savePlaylist();
        // Play it
        player->setSource(QUrl::fromLocalFile(track.filePath));
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

    const MediaTrack &track = playlist.at(currentTrackIndex);

    QUrl mediaUrl = QUrl::fromLocalFile(track.filePath);

    player->setSource(mediaUrl);

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
QString MainWindow::playlistFilePath()
{
    QString dataLocation =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation
        );

    QDir dir(dataLocation);

    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            qDebug() << "Failed to create directory:"
                     << dataLocation;
        }
    }

    QString filePath =
        dir.filePath("playlist.xml");

    qDebug() << "Playlist XML:" << filePath;

    return filePath;
}
void MainWindow::savePlaylist()
{
    QString filePath = playlistFilePath();

    qDebug() << "Saving playlist...";
    qDebug() << "File:" << filePath;
    qDebug() << "Number of tracks:" << playlist.size();

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "ERROR: Cannot open playlist file for writing";
        qDebug() << "Reason:" << file.errorString();
        return;
    }

    QXmlStreamWriter xml(&file);

    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    xml.writeStartElement("playlist");

    for (const MediaTrack &track : playlist)
    {
        qDebug() << "Saving track:"
                 << track.title
                 << track.filePath;

        xml.writeStartElement("track");

        xml.writeTextElement(
            "title",
            track.title
        );

        xml.writeTextElement(
            "path",
            track.filePath
        );

        xml.writeEndElement();
    }

    xml.writeEndElement();

    xml.writeEndDocument();

    file.close();

        qDebug() << "Playlist saved successfully.";
        qDebug() << "File exists:" << QFile::exists(filePath);
        qDebug() << "================================";
}
void MainWindow::loadPlaylist()
{
    const QString filePath = playlistFilePath();

    QFile file(filePath);

    if (!file.exists())
    {
        qDebug() << "Playlist file does not exist:"
                 << filePath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open playlist file:"
                 << file.errorString();
        return;
    }

    QXmlStreamReader xml(&file);

    playlist.clear();

    while (!xml.atEnd())
    {
        xml.readNext();

        if (xml.isStartElement())
        {
            if (xml.name() == QStringLiteral("track"))
            {
                MediaTrack track;

                while (!xml.atEnd())
                {
                    xml.readNext();

                    if (xml.isStartElement())
                    {
                        if (xml.name() == QStringLiteral("title"))
                        {
                            track.title = xml.readElementText();
                        }
                        else if (xml.name() == QStringLiteral("path"))
                        {
                            track.filePath = xml.readElementText();
                        }
                    }

                    if (xml.isEndElement() &&
                        xml.name() == QStringLiteral("track"))
                    {
                        break;
                    }
                }

                if (!track.filePath.isEmpty())
                {
                    playlist.append(track);
                }
            }
        }
    }

    if (xml.hasError())
    {
        qDebug() << "XML parsing error:"
                 << xml.errorString();
    }

    file.close();

    qDebug() << "Playlist loaded:"
             << playlist.size()
             << "tracks";
}
void MainWindow::testPlaylistFile()
{
    QString path = playlistFilePath();

    qDebug() << "TEST PATH:" << path;

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "TEST FAILED:";
        qDebug() << file.errorString();
        return;
    }

    file.write("TEST PLAYLIST\n");

    file.close();

    qDebug() << "TEST FILE CREATED SUCCESSFULLY";
}
MainWindow::~MainWindow()
{
    delete ui;
}
