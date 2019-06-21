#include "remixdeck.h"

RemixDeck::RemixDeck(QWidget *parent) :
    QWidget(parent)
{

}

void RemixDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
