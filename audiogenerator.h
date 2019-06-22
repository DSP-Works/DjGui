#pragma once

#include "audiosource.h"

class AudioGenerator : public AudioSource
{
public:
    explicit AudioGenerator() = default;
    explicit AudioGenerator(float frequency) : m_frequency(frequency) {}
    explicit AudioGenerator(bool enabled) : m_enabled(enabled) {}
    explicit AudioGenerator(float frequency, bool enabled) : m_frequency(frequency), m_enabled(enabled) {}
    virtual ~AudioGenerator() = default;

    float frequency() const { return m_frequency; }
    void setFrequency(float frequency) { m_frequency = frequency; }

    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }

    void writeSamples(float *buffer, std::size_t frames) override;

private:
    float m_pos { 0.f };
    float m_frequency { 1000.f };
    bool m_enabled { true };
};
