#include "decktemplate.h"

#include <QHBoxLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSlider>

#include "controlknob.h"

DeckTemplate::DeckTemplate(QWidget *parent) :
    QWidget(parent)
{
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
            auto hboxLayuot = new QHBoxLayout;

            m_fx1Button = new QToolButton;
            m_fx1Button->setText(tr("1"));
            hboxLayuot->addWidget(m_fx1Button);

            m_fx2Button = new QToolButton;
            m_fx2Button->setText(tr("2"));
            hboxLayuot->addWidget(m_fx2Button);

            vboxLayout->addLayout(hboxLayuot);
        }

        {
            auto label = new QLabel(tr("FX"));
            label->setAlignment(Qt::AlignCenter);
            vboxLayout->addWidget(label);
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

            m_volumeSlider = new QSlider;
            m_volumeSlider->setOrientation(Qt::Vertical);
            hboxLayout->addWidget(m_volumeSlider);

            hboxLayout->addStretch();

            vboxLayout->addLayout(hboxLayout, 1);
        }

        m_layout->addLayout(vboxLayout);
    }

    setLayout(m_layout);
}

void DeckTemplate::setCentralWidget(QWidget *widget)
{
    m_layout->replaceWidget(m_centralWidget, widget, Qt::FindDirectChildrenOnly);
    delete m_centralWidget;
    m_centralWidget = widget;
}
