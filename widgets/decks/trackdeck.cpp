#include "trackdeck.h"

TrackDeck::TrackDeck(QWidget *parent) :
    QWidget(parent)
{
}

void TrackDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
