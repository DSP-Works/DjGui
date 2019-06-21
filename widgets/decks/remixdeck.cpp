#include "remixdeck.h"

#include <QLabel>

RemixDeck::RemixDeck(QWidget *parent) :
    QWidget(parent)
{
    setMinimumHeight(100);

    new QLabel(tr("RemixDeck"), this);
}

void RemixDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
