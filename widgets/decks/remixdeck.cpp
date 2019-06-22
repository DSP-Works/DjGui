#include "remixdeck.h"

#include <QLabel>

#include "audiomixer.h"

RemixDeck::RemixDeck(QWidget *parent) :
    DeckTemplate(parent),
    m_mixer(std::make_unique<AudioMixer>())
{
    setMinimumHeight(100);

    setCentralWidget(new QLabel(tr("RemixDeck")));
}

RemixDeck::~RemixDeck() = default;

AudioSource &RemixDeck::deckAudioSource()
{
    return *m_mixer;
}
