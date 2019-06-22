#include "audioplayer.h"

#include <QAudioBuffer>

#include <cmath>

AudioPlayer::AudioPlayer(QObject *parent) :
    QObject(parent),
    AudioSource()
{
}

AudioPlayer::AudioPlayer(const QAudioBuffer *buffer, QObject *parent) :
    QObject(parent),
    AudioSource(),
    m_buffer(buffer)
{
}

void AudioPlayer::writeSamples(float *buffer, std::size_t frames)
{
    if (!m_buffer || !m_playing)
    {
        std::fill(buffer, buffer + frames, 0.f);
        return;
    }

    std::generate(buffer, buffer+frames, [this](){
        const std::size_t left = std::floor(m_position);
        const std::size_t right = std::ceil(m_position);
        const float factor = m_position - left;

        if (m_position < 0. || right >= m_buffer->sampleCount())
        {
            if (m_playing)
            {
                m_playing = false;
                emit playingChanged(m_playing);
            }
            return 0.f;
        }

        m_position += m_playbackSpeed;

        const auto leftSample = m_buffer->constData<float>()[left];
        const auto rightSample = m_buffer->constData<float>()[right];

        const auto diff = rightSample - leftSample;
        return leftSample + (factor * diff);
    });

    emit currentSampleChanged(m_position);
}
