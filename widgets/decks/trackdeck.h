#pragma once

#include <QAudioBuffer>

#include <memory>

#include "widgets/decktemplate.h"

class QThread;
class QProgressBar;

class AudioDecoder;
class AudioPlayer;
class PreviewWidget;

class TrackDeck : public DeckTemplate
{
    Q_OBJECT

public:
    explicit TrackDeck(QThread &decodingThread, bool rightSide, QWidget *parent = nullptr);
    ~TrackDeck() override;

signals:
    void currentSampleChanged(double currentSample);
    void bufferChanged();

protected:
    AudioSource &deckAudioSource() override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void progress(int progress, int total);
    void decodingFinished();

private:
    QThread &m_decodingThread;
    QProgressBar *m_progressBar;
    QSlider *m_slider;
    QToolButton *m_playButton;
    QToolButton *m_cueButton;
    std::unique_ptr<AudioDecoder> m_decoder;
    QAudioBuffer m_buffer;
    const std::unique_ptr<AudioPlayer> m_player;
    PreviewWidget *m_previewWidget;
};
