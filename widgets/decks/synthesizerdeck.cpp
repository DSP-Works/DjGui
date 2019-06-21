#include "synthesizerdeck.h"

#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

SynthesizerDeck::SynthesizerDeck(QWidget *parent) :
    QWidget(parent)
{
    auto layout = new QHBoxLayout;

    auto label = new QLabel("test");

    auto slider = new QSlider;
    slider->setRange(20, 20000);
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximumWidth(1000);
    connect(slider, &QSlider::valueChanged, [this,label](int value){
        setFrequency(value);
        label->setText(QString("%0 Hz").arg(value));
    });

    layout->addWidget(slider);

    layout->addWidget(label);

    setLayout(layout);
}
