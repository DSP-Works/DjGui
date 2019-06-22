#pragma once

#include <QAudioBuffer>

#include <memory>

#include "widgets/decktemplate.h"
#include "audioplayer.h"

class QThread;
class QProgressBar;

class AudioDecoder;

class TrackDeck : public DeckTemplate
{
    Q_OBJECT

public:
    explicit TrackDeck(QThread &decodingThread, QWidget *parent = nullptr);
    ~TrackDeck() override;

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
    std::unique_ptr<AudioDecoder> m_decoder;
    AudioPlayer m_player;;

    QAudioBuffer m_buffer;
};
