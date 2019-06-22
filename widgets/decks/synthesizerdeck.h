#pragma once

#include "widgets/decktemplate.h"
#include "audiosource.h"

class SynthesizerDeck : public DeckTemplate, public AudioSource
{
    Q_OBJECT

public:
    explicit SynthesizerDeck(QWidget *parent = nullptr);

    void writeSamples(float *buffer, std::size_t frames) override;
};
