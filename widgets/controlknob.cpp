#include "controlknob.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDial>
#include <QLabel>

ControlKnob::ControlKnob(QWidget *parent) :
    QWidget(parent)
{
    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addStretch();

        m_dial = new QDial;
        m_dial->setMaximumSize(32, 32);
        hboxLayout->addWidget(m_dial);

        hboxLayout->addStretch();

        layout->addLayout(hboxLayout);
    }

    m_label = new QLabel;
    m_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_label);

    setLayout(layout);
}

QString ControlKnob::text() const
{
    return m_label->text();
}

void ControlKnob::setText(const QString &text)
{
    m_label->setText(text);
}
