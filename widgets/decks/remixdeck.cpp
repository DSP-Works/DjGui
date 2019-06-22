#include "remixdeck.h"

#include <QLabel>

RemixDeck::RemixDeck(QWidget *parent) :
    DeckTemplate(parent)
{
    setMinimumHeight(100);

    setCentralWidget(new QLabel(tr("RemixDeck")));
}

void RemixDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
