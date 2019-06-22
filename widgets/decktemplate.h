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
    explicit DeckTemplate(bool rightSide, QWidget *parent = nullptr);
    ~DeckTemplate() override;

    void writeSamples(float *buffer, std::size_t frames) override final;

    int gain() const;
    void setGain(int gain);

    int filter() const;
    void setFilter(int filter);

    bool fx1() const;
    void setFx1(bool fx1);

    bool fx2() const;
    void setFx2(bool fx2);

    int key() const;
    void setKey(int key);

    bool monitor() const;
    void setMonitor(bool monitor);

    int pan() const;
    void setPan(int pan);

    int hi() const;
    void setHi(int hi);

    int mid() const;
    void setMid(int mid);

    int low() const;
    void setLow(int low);

    int volume() const;
    void setVolume(int volume);

signals:
    void gainChanged(int gain);
    void filterChanged(int filter);
    void fx1Changed(bool fx1);
    void fx2Changed(bool fx2);
    void keyChanged(int key);
    void monitorChanged(bool monitor);
    void panChanged(int pan);
    void hiChanged(int hi);
    void midChanged(int mid);
    void lowChanged(int low);
    void volumeChanged(int volume);

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
    const std::unique_ptr<AudioVolumeControl> m_panController;
};
