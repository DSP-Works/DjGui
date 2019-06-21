#include "audiosource.h"

#include <QAudioFormat>

QAudioFormat AudioSource::format()
{
    QAudioFormat format;
    format.setSampleRate(sampleRate());
    format.setChannelCount(channelCount());
    format.setSampleSize(sampleSize());
    format.setCodec(codec());
    format.setByteOrder(byteOrder());
    format.setSampleType(sampleType());
    return format;
}
