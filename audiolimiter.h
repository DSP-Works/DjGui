#pragma once

#include "audiosource.h"

class AudioLimiter : public AudioSource
{
public:
    explicit AudioLimiter() = default;
    explicit AudioLimiter(AudioSource *audioSource);

    AudioSource *audioSource() const { return m_audioSource; }
    void setAudioSource(AudioSource *audioSource) { m_audioSource = audioSource; }

    void writeSamples(float *buffer, std::size_t frames) override;

private:
    AudioSource *m_audioSource { nullptr };
};
