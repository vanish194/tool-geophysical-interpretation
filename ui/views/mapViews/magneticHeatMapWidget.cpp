#include "ui/views/mapViews/magneticHeatMapWidget.h"

MagneticHeatMapWidget::MagneticHeatMapWidget(
    MagneticSpatialDataAdapter* adapter,
    QWidget* parent)
    : AbstractHeatMapWidget(adapter, parent)
{
}

QColor MagneticHeatMapWidget::colorAt(double t) const
{
    const QColor red("#e74c3c");
    const QColor blue("#3498db");

    return QColor(
        red.red()   + t * (blue.red()   - red.red()),
        red.green() + t * (blue.green() - red.green()),
        red.blue()  + t * (blue.blue()  - red.blue())
        );
}

QString MagneticHeatMapWidget::units() const
{
    return QStringLiteral("нТ");
}
