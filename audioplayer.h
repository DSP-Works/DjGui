#pragma once

#include <QObject>

#include "audiosource.h"

class QAudioBuffer;

class AudioPlayer : public QObject, public AudioSource
{
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);
    explicit AudioPlayer(const QAudioBuffer *buffer, QObject *parent = nullptr);

    void writeSamples(float *buffer, std::size_t frames) override;

    bool playing() const { return m_playing; }
    void setPlaying(bool playing) { m_playing = playing; emit playingChanged(m_playing); }

    float playbackSpeed() const { return m_playbackSpeed; }
    void setPlaybackSpeed(float playbackSpeed) { m_playbackSpeed = playbackSpeed; }

    double position() const { return m_position; }
    void setPosition(double position) { m_position = position; emit currentSampleChanged(m_position); }

    const QAudioBuffer *buffer() const { return m_buffer; }
    void setAudioBuffer(const QAudioBuffer *buffer) { m_buffer = buffer; }

signals:
    void playingChanged(bool playing);
    void currentSampleChanged(double currentSample);

private:
    bool m_playing { false };
    float m_playbackSpeed { 1.f };
    double m_position { 0. };

    const QAudioBuffer *m_buffer;
};
