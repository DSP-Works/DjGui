#pragma once

#include <QWidget>

class QDial;
class QLabel;

class ControlKnob : public QWidget
{
    Q_OBJECT

public:
    explicit ControlKnob(QWidget *parent = nullptr);
    explicit ControlKnob(const QString &text, QWidget *parent = nullptr);
    explicit ControlKnob(int minimum, int maximum, int value, QWidget *parent = nullptr);
    explicit ControlKnob(const QString &text, int minimum, int maximum, int value, QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    int minimum() const;
    void setMinimum(int minimum);

    int maximum() const;
    void setMaximum(int maximum);

    void setRange(int minimum, int maximum);

    int value() const;
    void setValue(int value);

signals:
    void textChanged(const QString &text);

    void valueChanged(int value);
    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();
    void rangeChanged(int min, int max);

private:
    void setupWidget();

    QDial *m_dial;
    QLabel *m_label;
};
