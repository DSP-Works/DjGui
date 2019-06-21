#pragma once

#include <QString>
#include <QAudioFormat>

#include <cstddef>

class AudioSource
{
public:
    virtual void writeSamples(float *buffer, std::size_t frames) = 0;

    static int sampleRate() { return 44100; }
    static int channelCount() { return 2; }
    static int sampleSize() { return 32; }
    static QString codec() { return QString("audio/pcm"); }
    static QAudioFormat::Endian byteOrder() { return QAudioFormat::LittleEndian; }
    static QAudioFormat::SampleType sampleType() { return QAudioFormat::Float; }
    static QAudioFormat format();
};
