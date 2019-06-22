#include "audiocrossmixer.h"

AudioCrossMixer::AudioCrossMixer() :
    m_leftControl(&m_leftMixer),
    m_rightControl(&m_rightMixer),
    m_masterMixer({ &m_leftControl, &m_rightControl })
{
}

AudioCrossMixer::AudioCrossMixer(const std::vector<AudioSource *> &leftInputs, const std::vector<AudioSource *> &rightInputs) :
    m_leftMixer(leftInputs),
    m_leftControl(&m_leftMixer),
    m_rightMixer(rightInputs),
    m_rightControl(&m_rightMixer),
    m_masterMixer({ &m_leftControl, &m_rightControl })
{
}

AudioCrossMixer::AudioCrossMixer(std::vector<AudioSource *> &&leftInputs, std::vector<AudioSource *> &&rightInputs) :
    m_leftMixer(leftInputs),
    m_leftControl(&m_leftMixer),
    m_rightMixer(rightInputs),
    m_rightControl(&m_rightMixer),
    m_masterMixer({ &m_leftControl, &m_rightControl })
{
}

void AudioCrossMixer::writeSamples(float *buffer, std::size_t frames)
{
    m_masterMixer.writeSamples(buffer, frames);
}

void AudioCrossMixer::setPosition(float position)
{
    if (position < 0.f)
    {
        m_leftControl.setVolume(1.f);
        m_rightControl.setVolume(1.f + position);
    }
    else
    {
        m_leftControl.setVolume(1.f - position);
        m_rightControl.setVolume(1.f);
    }

    m_position = position;
}
