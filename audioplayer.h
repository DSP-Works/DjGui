#pragma once

#include "audiosource.h"

#include <QAudioBuffer>

class AudioPlayer : public AudioSource
{
public:
    explicit AudioPlayer() = default;
    explicit AudioPlayer(const QAudioBuffer &buffer);
    explicit AudioPlayer(QAudioBuffer &&buffer);

    void writeSamples(float *buffer, std::size_t frames) override;

    bool playing() const { return m_playing; }
    void setPlaying(bool playing) { m_playing = playing; }

    float playbackSpeed() const { return m_playbackSpeed; }
    void setPlaybackSpeed(float playbackSpeed) { m_playbackSpeed = playbackSpeed; }

    double position() const { return m_position; }
    void setPosition(double position) { m_position = position; }

    QAudioBuffer &buffer() { return m_buffer; }
    const QAudioBuffer &buffer() const { return m_buffer; }
    void setAudioBuffer(const QAudioBuffer &buffer) { m_buffer = buffer; }
    void setAudioBuffer(QAudioBuffer &&buffer) { m_buffer = std::move(buffer); }

private:
    bool m_playing { false };
    float m_playbackSpeed { 1.f };
    double m_position { 0. };

    QAudioBuffer m_buffer;
};
