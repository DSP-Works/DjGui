#pragma once

#include <QWidget>

#include "audiosource.h"

class QLabel;

class TrackDeck : public QWidget, public AudioSource
{
    Q_OBJECT

public:
    explicit TrackDeck(QWidget *parent = nullptr);

    void writeSamples(float *buffer, std::size_t frames) override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QLabel *m_label;
};
