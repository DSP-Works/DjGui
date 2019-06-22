#pragma once

class QPixmap;
class QSize;
class QPainter;

namespace GraphRenderer
{
    QPixmap render(const QSize &size, const float *sampleBegin, const float *sampleEnd);
    void render(const QSize &size, const float *sampleBegin, const float *sampleEnd, QPainter &painter);
}
