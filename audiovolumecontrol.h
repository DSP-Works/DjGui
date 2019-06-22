#pragma once

#include "audiosource.h"

#include <vector>

class AudioVolumeControl : public AudioSource
{
public:
    explicit AudioVolumeControl() = default;
    explicit AudioVolumeControl(AudioSource *audioSource);

    AudioSource *audioSource() const { return m_audioSource; }
    void setAudioSource(AudioSource *audioSource) { m_audioSource = audioSource; }

    float volume() const { Q_ASSERT(m_volumes.begin() < m_volumes.end()); return *m_volumes.begin(); }
    void setVolume(float volume) { setVolumes({ volume }); }

    const std::vector<float> &volumes() { return m_volumes; }
    void setVolumes(const std::vector<float> &volumes) { m_volumes = volumes; }

    void writeSamples(float *buffer, std::size_t frames) override;

private:
    AudioSource *m_audioSource;
    std::vector<float> m_volumes { 1.f };
};
