#include "streamdevice.h"

#include <QDebug>

#include "audiosource.h"

StreamDevice::StreamDevice(QObject *parent) :
    QIODevice(parent)
{
    setOpenMode(QIODevice::ReadOnly);
}

StreamDevice::StreamDevice(AudioSource *audioSource, QObject *parent) :
    QIODevice(parent),
    m_audioSource(audioSource)
{
    setOpenMode(QIODevice::ReadOnly);
}

qint64 StreamDevice::readData(char *data, qint64 maxlen)
{
    if (!m_audioSource)
    {
        qWarning() << "no audio source defined!";
        return 0;
    }

    if (!maxlen)
        return 0;

    m_audioSource->writeSamples(reinterpret_cast<float*>(data), maxlen / sizeof(float));

    return maxlen / sizeof(float) * sizeof(float);
}

qint64 StreamDevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    qFatal("writing not supported");
}
