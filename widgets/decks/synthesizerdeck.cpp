#include "synthesizerdeck.h"

#include <QLabel>

SynthesizerDeck::SynthesizerDeck(QWidget *parent) :
    DeckTemplate(parent)
{
    setMinimumHeight(100);

    setCentralWidget(new QLabel(tr("SynthesizerDeck")));
}

void SynthesizerDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
