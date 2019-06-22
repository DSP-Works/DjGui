#include "audiodecoder.h"

#include <QThread>
#include <QDebug>

AudioDecoder::AudioDecoder(const QAudioFormat &audioFormat, QObject *parent) :
    QObject(parent),
    m_decoder(this)
{
    QObject::connect(&m_decoder, qOverload<QMultimedia::AvailabilityStatus>(&QMediaObject::availabilityChanged),
                     this, &AudioDecoder::availabilityChanged);
    QObject::connect(&m_decoder, &QMediaObject::metaDataAvailableChanged,
                     this, &AudioDecoder::metaDataAvailableChanged);
    QObject::connect(&m_decoder, qOverload<>(&QMediaObject::metaDataChanged),
                     this, &AudioDecoder::metaDataChanged);
    QObject::connect(&m_decoder, qOverload<QAudioDecoder::Error>(&QAudioDecoder::error),
                     this, &AudioDecoder::error);
    QObject::connect(&m_decoder, &QAudioDecoder::finished,
                     this, &AudioDecoder::finished);
    QObject::connect(&m_decoder, &QAudioDecoder::bufferReady,
                     this, &AudioDecoder::bufferReady);
    QObject::connect(&m_decoder, &QAudioDecoder::durationChanged,
                     this, &AudioDecoder::durationChanged);

    m_decoder.setAudioFormat(audioFormat);
}

void AudioDecoder::start(const QString &filename)
{
    m_decoder.setSourceFilename(filename);
    m_bytearray = {};
    m_buffer = {};
    m_decoder.start();
}

const QAudioBuffer &AudioDecoder::buffer() const
{
    return m_buffer;
}

QAudioBuffer &&AudioDecoder::takeBuffer()
{
    return std::move(m_buffer);
}

void AudioDecoder::availabilityChanged(const QMultimedia::AvailabilityStatus availability)
{
    qDebug() << "media available:" << availability;
}

void AudioDecoder::metaDataAvailableChanged(const bool available)
{
    qDebug() << "metadata available:" << available;
}

void AudioDecoder::metaDataChanged()
{
    qDebug() << "metadata:" << m_decoder.availableMetaData();
}

void AudioDecoder::error(const QAudioDecoder::Error error)
{
    qDebug() << "error:" << error << m_decoder.errorString() << m_decoder.sourceFilename();
}

void AudioDecoder::finished()
{
    const auto &format = m_decoder.audioFormat();

    m_buffer = QAudioBuffer(m_bytearray, format);

    emit decodingFinished();

    qDebug() << "finished" << m_buffer.duration();
}

void AudioDecoder::bufferReady()
{
    const auto &format = m_decoder.audioFormat();
    const auto buffer = m_decoder.read();

    Q_ASSERT(buffer.byteCount() == buffer.sampleCount() * (format.sampleSize()/8));

    m_bytearray.append(buffer.constData<char>(), buffer.byteCount());

    const auto now = QDateTime::currentDateTime();
    if (m_lastProgressUpdate.isNull() || m_lastProgressUpdate.msecsTo(now) > 100)
    {
        emit progress(m_decoder.position(), m_decoder.duration());
        m_lastProgressUpdate = now;
    }
}

void AudioDecoder::durationChanged(const qint64 duration)
{
    if (duration == -1)
        return;

    const auto &format = m_decoder.audioFormat();
    const auto reserve = (format.sampleSize()/8) * format.sampleRate() * format.channelCount() * (duration + 1000) / 1000;
    m_bytearray.reserve(reserve);
    qDebug() << "duration:" << duration << reserve;
}
