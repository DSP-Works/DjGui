#pragma once

#include "widgets/decktemplate.h"

class QLabel;

class TrackDeck : public DeckTemplate
{
    Q_OBJECT

public:
    explicit TrackDeck(QWidget *parent = nullptr);

protected:
    AudioSource &deckAudioSource() override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
