#include "synthesizerdeck.h"

#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

SynthesizerDeck::SynthesizerDeck(QWidget *parent) :
    QWidget(parent)
{
}

void SynthesizerDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
