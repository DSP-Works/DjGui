#include "audiovolumecontrol.h"

#include <QDebug>

AudioVolumeControl::AudioVolumeControl(AudioSource *audioSource) :
    m_audioSource(audioSource)
{
}

void AudioVolumeControl::writeSamples(float *buffer, std::size_t frames)
{
    if (!m_audioSource)
    {
        qWarning() << "no audio source!";
        return;
    }

    m_audioSource->writeSamples(buffer, frames);
    std::transform(buffer, buffer + frames,
                   buffer,
                   [volume=m_volume](float sample){ return sample * volume; });
}
