#include "audioplayer.h"

#include <cmath>

AudioPlayer::AudioPlayer(const QAudioBuffer &buffer) :
    AudioSource(),
    m_buffer(buffer)
{
}

AudioPlayer::AudioPlayer(QAudioBuffer &&buffer) :
    AudioSource(),
    m_buffer(std::move(buffer))
{
}

void AudioPlayer::writeSamples(float *buffer, std::size_t frames)
{
    if (!m_playing)
    {
        std::fill(buffer, buffer + frames, 0.f);
        return;
    }

    std::generate(buffer, buffer+frames, [this](){
        const std::size_t left = std::floor(m_position);
        const std::size_t right = std::ceil(m_position);
        const float factor = m_position - left;

        m_position+=m_playbackSpeed;

        const auto leftSample = m_buffer.constData<float>()[left];
        const auto rightSample = m_buffer.constData<float>()[right];

        const auto diff = rightSample - leftSample;
        return leftSample + (factor * diff);
    });
}
