#include "ui/views/mapViews/abstractHeatMapWidget.h"

#include <QPainter>
#include <algorithm>

AbstractHeatMapWidget::AbstractHeatMapWidget(
    AbstractSpatialDataAdapter* adapter,
    QWidget* parent)
    : QWidget(parent)
    , adapter_(adapter)
{
    Q_ASSERT(adapter_);

    connect(adapter_, &AbstractSpatialDataAdapter::dataChanged,
            this, &AbstractHeatMapWidget::onDataChanged);

    setMinimumSize(300, 200);
}

QString AbstractHeatMapWidget::units() const
{
    return QString();
}

void AbstractHeatMapWidget::onDataChanged()
{
    update();
}

void AbstractHeatMapWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    constexpr int legendWidth = 80;
    constexpr int margin = 6;

    QRect mapRect = rect();
    mapRect.setRight(mapRect.right() - legendWidth);

    QRect legendRect(
        mapRect.right() + margin,
        margin,
        legendWidth - 2 * margin,
        height() - 2 * margin
        );

    drawHeatMap(p, mapRect);
    drawLegend(p, legendRect);
}

void AbstractHeatMapWidget::drawHeatMap(QPainter& p, const QRect& area)
{
    const auto& pts = adapter_->points();
    if (pts.isEmpty())
        return;

    int minX = pts.first().x;
    int maxX = pts.first().x;
    int minY = pts.first().y;
    int maxY = pts.first().y;

    minValue_ = maxValue_ = pts.first().value;

    for (const auto& sp : pts) {
        minX = std::min(minX, sp.x);
        maxX = std::max(maxX, sp.x);
        minY = std::min(minY, sp.y);
        maxY = std::max(maxY, sp.y);
        minValue_ = std::min(minValue_, sp.value);
        maxValue_ = std::max(maxValue_, sp.value);
    }

    const int cols = maxX - minX + 1;
    const int rows = maxY - minY + 1;

    const double cellW = area.width()  / double(cols);
    const double cellH = area.height() / double(rows);

    for (const auto& sp : pts) {
        double t = (maxValue_ == minValue_)
        ? 0.0
        : (sp.value - minValue_) / (maxValue_ - minValue_);

        QRectF cell(
            area.left() + (sp.x - minX) * cellW,
            area.bottom() - (sp.y - minY + 1) * cellH,
            cellW,
            cellH
            );

        p.fillRect(cell, colorAt(std::clamp(t, 0.0, 1.0)));
    }
}

void AbstractHeatMapWidget::drawLegend(QPainter& p, const QRect& area)
{
    constexpr int barWidth = 20;
    constexpr int tickCount = 20;
    constexpr int tickLength = 6;

    QRect barRect(
        area.left(),
        area.top() + 20,
        barWidth,
        area.height() - 40
        );

    // Градиент
    for (int y = 0; y < barRect.height(); ++y) {
        double t = 1.0 - double(y) / (barRect.height() - 1);
        p.setPen(colorAt(t));
        p.drawLine(barRect.left(),
                   barRect.top() + y,
                   barRect.right(),
                   barRect.top() + y);
    }

    p.setPen(Qt::black);
    p.drawRect(barRect);

    QFontMetrics fm(font());

    for (int i = 0; i <= tickCount; ++i) {
        double t = double(i) / tickCount;
        int y = barRect.bottom() - t * barRect.height();

        p.drawLine(barRect.right(),
                   y,
                   barRect.right() + tickLength,
                   y);

        double value = minValue_ + t * (maxValue_ - minValue_);
        QString text = QString::number(value, 'f', 2);

        p.drawText(barRect.right() + tickLength + 4,
                   y + fm.ascent() / 2,
                   text);
    }

    // Единицы измерения
    if (!units().isEmpty()) {
        QFont f = font();
        f.setBold(true);
        p.setFont(f);

        p.drawText(
            QRect(area.left(), area.top(), area.width(), 16),
            Qt::AlignCenter,
            units()
            );

        p.setFont(font());
    }
}
