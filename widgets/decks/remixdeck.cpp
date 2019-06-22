#include "remixdeck.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QToolButton>

#include "audiomixer.h"

RemixDeck::RemixDeck(bool rightSide, QWidget *parent) :
    DeckTemplate(rightSide, parent),
    m_mixer(std::make_unique<AudioMixer>())
{
    setMinimumHeight(100);

    auto layout = new QVBoxLayout;
    layout->setMargin(0);

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addWidget(new QLabel(tr("RemixDeck")));
        hboxLayout->addStretch(1);

        layout->addLayout(hboxLayout);
    }

    layout->addStretch(1);

    {
        auto hboxLayout = new QHBoxLayout;

        for (int i = 0; i < 2; i++)
        {
            auto gridLayout = new QGridLayout;

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 3; x++)
                    gridLayout->addWidget(new QToolButton, y, x);

            hboxLayout->addLayout(gridLayout);
        }

        layout->addLayout(hboxLayout);
    }

    layout->addStretch(1);

    auto widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

RemixDeck::~RemixDeck() = default;

AudioSource &RemixDeck::deckAudioSource()
{
    return *m_mixer;
}
