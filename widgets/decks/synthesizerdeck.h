#pragma once

#include <QWidget>

#include "audiogenerator.h"

class SynthesizerDeck : public QWidget, public AudioGenerator
{
    Q_OBJECT

public:
    explicit SynthesizerDeck(QWidget *parent = nullptr);
};
