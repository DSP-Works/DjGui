#include "synthesizerdeck.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include "audiogenerator.h"

SynthesizerDeck::SynthesizerDeck(bool rightSide, QWidget *parent) :
    DeckTemplate(rightSide, parent),
    m_generator(std::make_unique<AudioGenerator>(1000, false))
{
    setMinimumHeight(100);

    auto layout = new QVBoxLayout;
    layout->setMargin(0);

    layout->addWidget(new QLabel(tr("SynthesizerDeck")));

    layout->addStretch(1);

    {
        auto hboxLayout = new QHBoxLayout;

        auto label = new QLabel(tr("%0 Hz").arg(1000));

        {
            auto slider = new QSlider(Qt::Horizontal);
            slider->setRange(0, 10000);
            slider->setValue(1000);
            connect(slider, &QSlider::valueChanged, this, [&generator=*m_generator,label](int value){
                generator.setFrequency(value);
                label->setText(tr("%0 Hz").arg(value));
            });
            hboxLayout->addWidget(slider, 1);
        }

        hboxLayout->addWidget(label);

        m_button = new QPushButton(tr("Enable"));
        connect(m_button, &QAbstractButton::pressed, this, &SynthesizerDeck::pressed);
        hboxLayout->addWidget(m_button);

        layout->addLayout(hboxLayout);
    }

    layout->addStretch(1);

    auto widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

SynthesizerDeck::~SynthesizerDeck() = default;

AudioSource &SynthesizerDeck::deckAudioSource()
{
    return *m_generator;
}

void SynthesizerDeck::pressed()
{
    m_generator->setEnabled(!m_generator->enabled());
    m_button->setText(m_generator->enabled() ? tr("Disable") : tr("Enable"));
}
