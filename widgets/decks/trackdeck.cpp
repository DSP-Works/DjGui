#include "trackdeck.h"

#include <QLabel>

TrackDeck::TrackDeck(QWidget *parent) :
    QWidget(parent)
{
    setMinimumHeight(150);

    m_label = new QLabel(tr("TrackDeck n/a"), this);
}

void TrackDeck::writeSamples(float *buffer, std::size_t frames)
{
    std::fill(buffer, buffer + frames, 0.f);
}
