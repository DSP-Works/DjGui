#include "decktemplate.h"

#include <QHBoxLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSlider>

#include "audiovolumecontrol.h"
#include "controlknob.h"

DeckTemplate::DeckTemplate(QWidget *parent) :
    QFrame(parent),
    m_volumeController(std::make_unique<AudioVolumeControl>())
{
    setFrameShape(QFrame::Box);

    m_layout = new QHBoxLayout;

    m_centralWidget = new QWidget;
    m_layout->addWidget(m_centralWidget, 1);

    {
        auto vboxLayout = new QVBoxLayout;
        vboxLayout->setMargin(0);

        m_gainDial = new ControlKnob;
        m_gainDial->setText(tr("Gain"));
        vboxLayout->addWidget(m_gainDial);

        m_filterDial = new ControlKnob;
        m_filterDial->setText(tr("Filter"));
        vboxLayout->addWidget(m_filterDial);

        {
            auto fxLayout = new QVBoxLayout;
            fxLayout->setMargin(0);
            fxLayout->setSpacing(0);

            {
                auto hboxLayuot = new QHBoxLayout;

                m_fx1Button = new QToolButton;
                m_fx1Button->setText(tr("1"));
                m_fx1Button->setCheckable(true);
                hboxLayuot->addWidget(m_fx1Button);

                m_fx2Button = new QToolButton;
                m_fx2Button->setText(tr("2"));
                m_fx2Button->setCheckable(true);
                hboxLayuot->addWidget(m_fx2Button);

                fxLayout->addLayout(hboxLayuot);
            }

            {
                auto label = new QLabel(tr("FX"));
                label->setAlignment(Qt::AlignCenter);
                fxLayout->addWidget(label);
            }

            vboxLayout->addLayout(fxLayout);
        }

        m_keyDial = new ControlKnob;
        m_keyDial->setText(tr("Key"));
        vboxLayout->addWidget(m_keyDial);

        {
            auto hboxLayuot = new QHBoxLayout;

            hboxLayuot->addStretch();

            m_monitorButton = new QToolButton;
            m_monitorButton->setText(tr("🎧"));
            hboxLayuot->addWidget(m_monitorButton);

            hboxLayuot->addStretch();

            vboxLayout->addLayout(hboxLayuot);
        }

        m_panDial = new ControlKnob;
        m_panDial->setText(tr("Pan"));
        vboxLayout->addWidget(m_panDial);

        m_layout->addLayout(vboxLayout);
    }

    {
        auto vboxLayout = new QVBoxLayout;
        vboxLayout->setMargin(0);

        m_hiDial = new ControlKnob;
        m_hiDial->setText(tr("Hi"));
        vboxLayout->addWidget(m_hiDial);

        m_midDial = new ControlKnob;
        m_midDial->setText(tr("Mid"));
        vboxLayout->addWidget(m_midDial);

        m_lowDial = new ControlKnob;
        m_lowDial->setText(tr("Low"));
        vboxLayout->addWidget(m_lowDial);

        {
            auto hboxLayout = new QHBoxLayout;

            hboxLayout->addStretch();

            m_volumeSlider = new QSlider(Qt::Vertical);
            m_volumeSlider->setRange(0, 100);
            m_volumeSlider->setValue(100);
            connect(m_volumeSlider, &QSlider::valueChanged, this, [&controller=*m_volumeController](int value){ controller.setVolume(float(value) / 100.f); });
            hboxLayout->addWidget(m_volumeSlider);

            hboxLayout->addStretch();

            vboxLayout->addLayout(hboxLayout, 1);
        }

        m_layout->addLayout(vboxLayout);
    }

    setLayout(m_layout);
}

DeckTemplate::~DeckTemplate() = default;

void DeckTemplate::writeSamples(float *buffer, std::size_t frames)
{
    auto &source = deckAudioSource();
    if (m_volumeController->audioSource() != &source)
        m_volumeController->setAudioSource(&source);
    m_volumeController->writeSamples(buffer, frames);
}

void DeckTemplate::setCentralWidget(QWidget *widget)
{
    m_layout->replaceWidget(m_centralWidget, widget, Qt::FindDirectChildrenOnly);
    delete m_centralWidget;
    m_centralWidget = widget;
}
