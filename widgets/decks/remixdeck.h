#pragma once

#include <memory>

#include "widgets/decktemplate.h"

class AudioMixer;

class RemixDeck : public DeckTemplate
{
    Q_OBJECT

public:
    explicit RemixDeck(bool rightSide, QWidget *parent = nullptr);
    ~RemixDeck() override;

protected:
    AudioSource &deckAudioSource() override;

private:
    const std::unique_ptr<AudioMixer> m_mixer;
};
