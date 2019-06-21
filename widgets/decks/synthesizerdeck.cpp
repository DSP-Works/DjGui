#include "synthesizerdeck.h"

#include <QLabel>

SynthesizerDeck::SynthesizerDeck(QWidget *parent) :
    QWidget(parent)
{
    setMinimumHeight(100);

    new QLabel(tr("SynthesizerDeck"), this);
}

void SynthesizerDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
