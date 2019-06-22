#include "decktemplate.h"

#include <QHBoxLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSlider>

#include "audiovolumecontrol.h"
#include "controlknob.h"

DeckTemplate::DeckTemplate(bool rightSide, QWidget *parent) :
    QFrame(parent),
    m_gainController(std::make_unique<AudioVolumeControl>()),
    m_volumeController(std::make_unique<AudioVolumeControl>(m_gainController.get())),
    m_panController(std::make_unique<AudioVolumeControl>(m_volumeController.get()))
{
    setFrameShape(QFrame::Box);

    m_layout = new QHBoxLayout;

    m_centralWidget = new QWidget;

    auto firstControlsLayout = new QVBoxLayout;
    firstControlsLayout->setMargin(0);

    m_gainDial = new ControlKnob(tr("Gain"), 50, 150, 100);
    connect(m_gainDial, &ControlKnob::valueChanged, this, &DeckTemplate::gainChanged);
    connect(m_gainDial, &ControlKnob::valueChanged, this, [&controller=*m_gainController](int value){ controller.setVolume(float(value) / 100.f); });
    firstControlsLayout->addWidget(m_gainDial);

    m_filterDial = new ControlKnob(tr("Filter"), 0, 100, 50);
    connect(m_filterDial, &ControlKnob::valueChanged, this, &DeckTemplate::filterChanged);
    firstControlsLayout->addWidget(m_filterDial);

    {
        auto fxLayout = new QVBoxLayout;
        fxLayout->setMargin(0);
        fxLayout->setSpacing(0);

        {
            auto hboxLayuot = new QHBoxLayout;

            m_fx1Button = new QToolButton;
            m_fx1Button->setText(tr("1"));
            m_fx1Button->setCheckable(true);
            connect(m_fx1Button, &QAbstractButton::toggled, this, &DeckTemplate::fx1Changed);
            hboxLayuot->addWidget(m_fx1Button);

            m_fx2Button = new QToolButton;
            m_fx2Button->setText(tr("2"));
            m_fx2Button->setCheckable(true);
            connect(m_fx2Button, &QAbstractButton::toggled, this, &DeckTemplate::fx2Changed);
            hboxLayuot->addWidget(m_fx2Button);

            fxLayout->addLayout(hboxLayuot);
        }

        {
            auto label = new QLabel(tr("FX"));
            label->setAlignment(Qt::AlignCenter);
            fxLayout->addWidget(label);
        }

        firstControlsLayout->addLayout(fxLayout);
    }

    m_keyDial = new ControlKnob(tr("Key"), 0, 100, 50);
    connect(m_keyDial, &ControlKnob::valueChanged, this, &DeckTemplate::keyChanged);
    firstControlsLayout->addWidget(m_keyDial);

    {
        auto hboxLayuot = new QHBoxLayout;

        hboxLayuot->addStretch();

        m_monitorButton = new QToolButton;
        m_monitorButton->setText(tr("🎧"));
        m_monitorButton->setCheckable(true);
        connect(m_monitorButton, &QAbstractButton::toggled, this, &DeckTemplate::monitorChanged);
        hboxLayuot->addWidget(m_monitorButton);

        hboxLayuot->addStretch();

        firstControlsLayout->addLayout(hboxLayuot);
    }

    m_panDial = new ControlKnob(tr("Pan"), -100, 100, 0);
    connect(m_panDial, &ControlKnob::valueChanged, this, [&controller=*m_panController](int value){
        const auto scaledValue = float(value) / 100.f;

        std::vector<float> volumes;

        if (scaledValue < 0.f)
            volumes = { 1.f, 1.f + scaledValue };
        else
            volumes = { 1.f - scaledValue, 1.f };

        controller.setVolumes(volumes);
    });
    connect(m_panDial, &ControlKnob::valueChanged, this, &DeckTemplate::panChanged);
    firstControlsLayout->addWidget(m_panDial);

    auto secondControlsLayout = new QVBoxLayout;
    secondControlsLayout->setMargin(0);

    m_hiDial = new ControlKnob(tr("Hi"), 0, 100, 50);
    connect(m_hiDial, &ControlKnob::valueChanged, this, &DeckTemplate::hiChanged);
    secondControlsLayout->addWidget(m_hiDial);

    m_midDial = new ControlKnob(tr("Mid"), 0, 100, 50);
    connect(m_midDial, &ControlKnob::valueChanged, this, &DeckTemplate::midChanged);
    secondControlsLayout->addWidget(m_midDial);

    m_lowDial = new ControlKnob(tr("Low"), 0, 100, 50);
    connect(m_lowDial, &ControlKnob::valueChanged, this, &DeckTemplate::lowChanged);
    secondControlsLayout->addWidget(m_lowDial);

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addStretch();

        m_volumeSlider = new QSlider(Qt::Vertical);
        m_volumeSlider->setRange(0, 100);
        m_volumeSlider->setValue(100);
        connect(m_volumeSlider, &QAbstractSlider::valueChanged, this, [&controller=*m_volumeController](int value){ controller.setVolume(float(value) / 100.f); });
        connect(m_volumeSlider, &QAbstractSlider::valueChanged, this, &DeckTemplate::volumeChanged);
        hboxLayout->addWidget(m_volumeSlider);

        hboxLayout->addStretch();

        secondControlsLayout->addLayout(hboxLayout, 1);
    }

    auto controlsLayout = new QHBoxLayout;
    controlsLayout->setMargin(0);

    if (rightSide)
    {
        controlsLayout->addLayout(secondControlsLayout);
        controlsLayout->addLayout(firstControlsLayout);
        m_layout->addLayout(controlsLayout);
    }

    m_layout->addWidget(m_centralWidget, 1);

    if (!rightSide)
    {
        controlsLayout->addLayout(firstControlsLayout);
        controlsLayout->addLayout(secondControlsLayout);
        m_layout->addLayout(controlsLayout);
    }

    setLayout(m_layout);
}

DeckTemplate::~DeckTemplate() = default;

void DeckTemplate::writeSamples(float *buffer, std::size_t frames)
{
    auto &source = deckAudioSource();
    if (m_gainController->audioSource() != &source)
        m_gainController->setAudioSource(&source);
    m_panController->writeSamples(buffer, frames);
}

int DeckTemplate::gain() const
{
    return m_gainDial->value();
}

void DeckTemplate::setGain(int gain)
{
    m_gainDial->setValue(gain);
}

int DeckTemplate::filter() const
{
    return m_filterDial->value();
}

void DeckTemplate::setFilter(int filter)
{
    m_filterDial->setValue(filter);
}

bool DeckTemplate::fx1() const
{
    return m_fx1Button->isChecked();
}

void DeckTemplate::setFx1(bool fx1)
{
    m_fx1Button->setChecked(fx1);
}

bool DeckTemplate::fx2() const
{
    return m_fx2Button->isChecked();
}

void DeckTemplate::setFx2(bool fx2)
{
    m_fx2Button->setChecked(fx2);
}

int DeckTemplate::key() const
{
    return m_keyDial->value();
}

void DeckTemplate::setKey(int key)
{
    m_keyDial->setValue(key);
}

bool DeckTemplate::monitor() const
{
    return m_monitorButton->isChecked();
}

void DeckTemplate::setMonitor(bool monitor)
{
    m_monitorButton->setChecked(monitor);
}

int DeckTemplate::pan() const
{
    return m_panDial->value();
}

void DeckTemplate::setPan(int pan)
{
    m_panDial->setValue(pan);
}

int DeckTemplate::hi() const
{
    return m_hiDial->value();
}

void DeckTemplate::setHi(int hi)
{
    m_hiDial->setValue(hi);
}

int DeckTemplate::mid() const
{
    return m_midDial->value();
}

void DeckTemplate::setMid(int mid)
{
    m_midDial->setValue(mid);
}

int DeckTemplate::low() const
{
    return m_lowDial->value();
}

void DeckTemplate::setLow(int low)
{
    m_lowDial->setValue(low);
}

int DeckTemplate::volume() const
{
    return m_volumeSlider->value();
}

void DeckTemplate::setVolume(int volume)
{
    m_volumeSlider->setValue(volume);
}

void DeckTemplate::setCentralWidget(QWidget *widget)
{
    m_layout->replaceWidget(m_centralWidget, widget, Qt::FindDirectChildrenOnly);
    delete m_centralWidget;
    m_centralWidget = widget;
}
