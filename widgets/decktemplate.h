#pragma once

#include <QFrame>

#include <memory>

#include "audiosource.h"

class QHBoxLayout;
class QWidget;
class QToolButton;
class QSlider;

class ControlKnob;
class AudioVolumeControl;

class DeckTemplate : public QFrame, public AudioSource
{
    Q_OBJECT

public:
    explicit DeckTemplate(QWidget *parent = nullptr);
    ~DeckTemplate() override;

    void writeSamples(float *buffer, std::size_t frames) override final;

protected:
    void setCentralWidget(QWidget *widget);
    virtual AudioSource &deckAudioSource() = 0;

private:
    QHBoxLayout *m_layout;
    QWidget *m_centralWidget;

    ControlKnob *m_gainDial;
    ControlKnob *m_filterDial;
    QToolButton *m_fx1Button;
    QToolButton *m_fx2Button;
    ControlKnob *m_keyDial;
    QToolButton *m_monitorButton;
    ControlKnob *m_panDial;

    ControlKnob *m_hiDial;
    ControlKnob *m_midDial;
    ControlKnob *m_lowDial;
    QSlider *m_volumeSlider;

    const std::unique_ptr<AudioVolumeControl> m_volumeController;
};
