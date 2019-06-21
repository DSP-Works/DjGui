#pragma once

#include "audiosource.h"

class AudioGenerator : public AudioSource
{
public:
    explicit AudioGenerator() = default;

    float frequency() const { return m_frequency; }
    void setFrequency(float frequency) { m_frequency = frequency; }

    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }

    void writeSamples(float *buffer, std::size_t frames) override;

private:
    float m_frequency { 1000.f };
    bool m_enabled { true };
    float m_pos { 0.f };
};
