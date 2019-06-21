#pragma once

#include <QWidget>

#include "audiosource.h"

class SynthesizerDeck : public QWidget, public AudioSource
{
    Q_OBJECT

public:
    explicit SynthesizerDeck(QWidget *parent = nullptr);

    void writeSamples(float *buffer, std::size_t frames) override;
};
