#pragma once

#include <QWidget>

class QDial;
class QLabel;

class ControlKnob : public QWidget
{
    Q_OBJECT

public:
    explicit ControlKnob(QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

private:
    QDial *m_dial;
    QLabel *m_label;
};
