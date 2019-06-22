#pragma once

#include "audiosource.h"

#include <vector>
#include <memory>

class AudioMixer : public AudioSource
{
public:
    explicit AudioMixer() = default;
    explicit AudioMixer(const AudioMixer &) = default;
    explicit AudioMixer(AudioMixer &&) = default;
    explicit AudioMixer(const std::vector<AudioSource*> &inputs);
    explicit AudioMixer(std::vector<AudioSource*> &&inputs);

    AudioMixer &operator=(const AudioMixer &) = default;
    AudioMixer &operator=(AudioMixer &&) = default;

    void addInput(AudioSource &source);
    void removeInput(AudioSource &source);

    const std::vector<AudioSource*> &inputs() const { return m_inputs; }

    void writeSamples(float *buffer, std::size_t frames) override;

private:
    std::vector<AudioSource*> m_inputs;

    std::unique_ptr<float[]> m_tempBuffer;
    std::size_t m_tempBufferSize;
};
