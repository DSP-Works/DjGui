#include "audiovolumecontrol.h"

#include <QDebug>

AudioVolumeControl::AudioVolumeControl(AudioSource *audioSource) :
    AudioSource(),
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

    Q_ASSERT(m_volumes.begin() < m_volumes.end());

    auto volume = m_volumes.begin();

    std::transform(buffer, buffer + frames,
                   buffer,
                   [&volumes=m_volumes,&volume](float sample){
        sample *= *volume;
        if (++volume == volumes.end())
            volume = volumes.begin();

        return sample;
    });
}
