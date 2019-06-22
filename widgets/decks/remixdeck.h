#pragma once

#include "widgets/decktemplate.h"
#include "audiosource.h"

class RemixDeck : public DeckTemplate, public AudioSource
{
    Q_OBJECT

public:
    explicit RemixDeck(QWidget *parent = nullptr);

    void writeSamples(float *buffer, std::size_t frames) override;
};
