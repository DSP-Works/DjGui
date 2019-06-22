#pragma once

#include <QWidget>
#include <QPixmap>

class QAudioBuffer;

class PreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    explicit PreviewWidget(const QAudioBuffer *buffer, QWidget *parent = nullptr);

    const QAudioBuffer *buffer() const { return m_buffer; }
    void setBuffer(const QAudioBuffer *buffer) { m_buffer = buffer; clearCache(); }

    double currentSample() const { return m_currentSample; }

signals:
    void sampleSelected(double sample);

public slots:
    void setCurrentSample(double currentSample) { m_currentSample = currentSample; repaint(); }
    void clearCache() { m_graphCache = {}; repaint(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void clicked(int x, int y);

    const QAudioBuffer *m_buffer { nullptr };
    double m_currentSample {};
    QPixmap m_graphCache;
};
