#include "audiomixer.h"

#include <QDebug>

#include <utility>
#include <memory>
#include <algorithm>

AudioMixer::AudioMixer(const std::vector<AudioSource *> &inputs) :
    m_inputs(inputs)
{
}

AudioMixer::AudioMixer(std::vector<AudioSource *> &&inputs) :
    m_inputs(std::move(inputs))
{
}

void AudioMixer::addInput(AudioSource &source)
{
    m_inputs.push_back(&source);
}

void AudioMixer::removeInput(AudioSource &source)
{
    const auto iter = std::find(std::begin(m_inputs), std::end(m_inputs), &source);
    if (iter == std::end(m_inputs))
    {
        qWarning() << "input not found";
        return;
    }

    m_inputs.erase(iter);
}

void AudioMixer::writeSamples(float *buffer, std::size_t frames)
{
    const auto tempBuffer = std::make_unique<float[]>(frames);

    std::fill(buffer, buffer + frames, 0.f);

    for (AudioSource *audioSource : m_inputs)
    {
        audioSource->writeSamples(tempBuffer.get(), frames);

        std::transform(tempBuffer.get(), tempBuffer.get() + frames,
                       buffer,
                       buffer,
                       [](float a, float b){ return a+b; });
    }
}
