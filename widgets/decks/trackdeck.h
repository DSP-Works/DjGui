#pragma once

#include <memory>

#include "widgets/decktemplate.h"

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
    std::unique_ptr<AudioDecoder> m_decoder;
};
