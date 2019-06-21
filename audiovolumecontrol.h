#pragma once

#include "audiosource.h"

class AudioVolumeControl : public AudioSource
{
public:
    explicit AudioVolumeControl() = default;
    explicit AudioVolumeControl(AudioSource *audioSource);

    AudioSource *audioSource() const { return m_audioSource; }
    void setAudioSource(AudioSource *audioSource) { m_audioSource = audioSource; }

    float volume() const { return m_volume; }
    void setVolume(float volume) { m_volume = volume; }

    void writeSamples(float *buffer, std::size_t frames) override;

private:
    AudioSource *m_audioSource;
    float m_volume { 1.f };
};
