#pragma once

#include <QObject>
#include <QAudioDecoder>
#include <QByteArray>
#include <QAudioBuffer>
#include <QDateTime>

class AudioDecoder : public QObject
{
    Q_OBJECT

public:
    explicit AudioDecoder(const QAudioFormat &audioFormat, QObject *parent = nullptr);

    Q_INVOKABLE void start(const QString &filename);

    const QAudioBuffer &buffer() const;
    QAudioBuffer &&takeBuffer();

signals:
    void progress(int progress, int total);
    void decodingFinished();

private slots:
    void availabilityChanged(const QMultimedia::AvailabilityStatus availability);
    void metaDataAvailableChanged(const bool available);
    void metaDataChanged();
    void error(const QAudioDecoder::Error error);
    void finished();
    void bufferReady();
    void durationChanged(const qint64 duration);

private:
    QAudioDecoder m_decoder;
    QByteArray m_bytearray;
    QAudioBuffer m_buffer;
    QDateTime m_lastProgressUpdate;
};
