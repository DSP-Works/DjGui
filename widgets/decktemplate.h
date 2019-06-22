#pragma once

#include <QWidget>

class QHBoxLayout;
class QWidget;
class QToolButton;
class QSlider;

class ControlKnob;

class DeckTemplate : public QWidget
{
    Q_OBJECT

public:
    explicit DeckTemplate(QWidget *parent = nullptr);

protected:
    void setCentralWidget(QWidget *widget);

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
};
