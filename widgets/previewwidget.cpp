#include "previewwidget.h"

#include <QAudioBuffer>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>

#include "graphrenderer.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent)
{
}

PreviewWidget::PreviewWidget(const QAudioBuffer *buffer, QWidget *parent) :
    QWidget(parent),
    m_buffer(buffer)
{

}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!m_buffer)
        return;

    if (m_graphCache.isNull() || m_graphCache.size() != size())
        m_graphCache = GraphRenderer::render(size(), m_buffer->constData<float>(), m_buffer->constData<float>() + m_buffer->sampleCount());

    QPainter painter;
    painter.begin(this);

    painter.drawPixmap(0, 0, m_graphCache);

    if (m_buffer->sampleCount())
    {
        std::size_t currentSample = double(m_currentSample) / m_buffer->sampleCount() * width();

        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawLine(currentSample, 0, currentSample, height());
    }

    painter.end();
}

void PreviewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        setMouseTracking(true);
        clicked(event->x(), event->y());
    }

    QWidget::mousePressEvent(event);
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        setMouseTracking(false);

    QWidget::mouseReleaseEvent(event);
}

void PreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
        clicked(event->x(), event->y());

    QWidget::mouseMoveEvent(event);
}

void PreviewWidget::clicked(int x, int y)
{
    if (!m_buffer)
        return;

    const auto samplesPerPixel = m_buffer->sampleCount() / width();

    emit sampleSelected(x * samplesPerPixel);
}
