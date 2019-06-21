#pragma once

#include <QIODevice>

class AudioSource;

class StreamDevice : public QIODevice
{
    Q_OBJECT

public:
    explicit StreamDevice(QObject *parent = nullptr);
    explicit StreamDevice(AudioSource *audioSource, QObject *parent = nullptr);

    AudioSource *audioSource() const { return m_audioSource; }
    void setAudioSource(AudioSource *audioSource) { m_audioSource = audioSource; }

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    AudioSource *m_audioSource { nullptr };
};
