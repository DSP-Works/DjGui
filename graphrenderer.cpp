#include "graphrenderer.h"

#include <QPixmap>
#include <QPainter>

#include <iterator>

QPixmap GraphRenderer::render(const QSize &size, const float *sampleBegin, const float *sampleEnd)
{
    QPixmap pixmap(size);
    pixmap.fill();
    QPainter painter;
    painter.begin(&pixmap);

    painter.setPen(Qt::white);
    painter.setBrush(Qt::black);

    painter.drawRect(QRect({}, size));

    render(size, sampleBegin, sampleEnd, painter);
    painter.end();
    return pixmap;
}

void GraphRenderer::render(const QSize &size, const float *sampleBegin, const float *sampleEnd, QPainter &painter)
{
    if (sampleEnd == sampleBegin)
        return;

    const auto samplesPerPixel = std::distance(sampleBegin, sampleEnd) / size.width();

    for (int x = 0; x < size.width(); x++)
    {
        const float *begin = sampleBegin + (x * samplesPerPixel);
        const float *end = begin + samplesPerPixel;

        const auto minmax = std::minmax_element(begin, end);

        painter.drawLine(x, (size.height() / 2) - (float(*minmax.first) * (size.height() / 2)),
                         x, (size.height() / 2) + (float(*minmax.second) * (size.height() / 2)));
    }
}
