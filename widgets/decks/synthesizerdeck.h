#pragma once

#include <memory>

#include "widgets/decktemplate.h"

class QPushButton;

class AudioGenerator;

class SynthesizerDeck : public DeckTemplate
{
    Q_OBJECT

public:
    explicit SynthesizerDeck(bool rightSide, QWidget *parent = nullptr);
    ~SynthesizerDeck() override;

protected:
    AudioSource &deckAudioSource() override;

private slots:
    void pressed();

private:
    const std::unique_ptr<AudioGenerator> m_generator;

    QPushButton *m_button;
};
