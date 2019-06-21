#include "audiolimiter.h"

#include <QDebug>

#include <algorithm>

AudioLimiter::AudioLimiter(AudioSource *audioSource) :
    m_audioSource(audioSource)
{
}

void AudioLimiter::writeSamples(float *buffer, std::size_t frames)
{
    if (!m_audioSource)
    {
        qWarning() << "no audio source!";
        return;
    }

    m_audioSource->writeSamples(buffer, frames);
    std::transform(buffer, buffer + frames,
                   buffer,
                   [](float sample){ return std::max(-1.f, std::min(1.f, sample)); });
}
