#include "controlknob.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDial>
#include <QLabel>

ControlKnob::ControlKnob(QWidget *parent) :
    QWidget(parent)
{
    setupWidget();
}

ControlKnob::ControlKnob(const QString &text, QWidget *parent) :
    QWidget(parent)
{
    setupWidget();
    setText(text);
}

ControlKnob::ControlKnob(int minimum, int maximum, int value, QWidget *parent) :
    QWidget(parent)
{
    setupWidget();
    setRange(minimum, maximum);
    setValue(value);
}

ControlKnob::ControlKnob(const QString &text, int minimum, int maximum, int value, QWidget *parent) :
    QWidget(parent)
{
    setupWidget();
    setText(text);
    setRange(minimum, maximum);
    setValue(value);
}

QString ControlKnob::text() const
{
    return m_label->text();
}

void ControlKnob::setText(const QString &text)
{
    m_label->setText(text);
    textChanged(text);
}

int ControlKnob::minimum() const
{
    return m_dial->minimum();
}

void ControlKnob::setMinimum(int minimum)
{
    m_dial->setMinimum(minimum);
}

int ControlKnob::maximum() const
{
    return m_dial->maximum();
}

void ControlKnob::setMaximum(int maximum)
{
    m_dial->setMaximum(maximum);
}

void ControlKnob::setRange(int minimum, int maximum)
{
    m_dial->setRange(minimum, maximum);
}

int ControlKnob::value() const
{
    return m_dial->value();
}

void ControlKnob::setValue(int value)
{
    m_dial->setValue(value);
}

void ControlKnob::setupWidget()
{
    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addStretch();

        m_dial = new QDial;
        m_dial->setMaximumSize(32, 32);
        connect(m_dial, &QAbstractSlider::valueChanged, this, &ControlKnob::valueChanged);
        connect(m_dial, &QAbstractSlider::sliderPressed, this, &ControlKnob::sliderPressed);
        connect(m_dial, &QAbstractSlider::sliderMoved, this, &ControlKnob::sliderMoved);
        connect(m_dial, &QAbstractSlider::sliderReleased, this, &ControlKnob::sliderReleased);
        connect(m_dial, &QAbstractSlider::rangeChanged, this, &ControlKnob::rangeChanged);
        hboxLayout->addWidget(m_dial);

        hboxLayout->addStretch();

        layout->addLayout(hboxLayout);
    }

    m_label = new QLabel;
    m_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_label);

    setLayout(layout);
}
