#include "ui/views/mapViews/GPSHeatMapWidget.h"

GPSHeatMapWidget::GPSHeatMapWidget(
    GPSSpatialDataAdapter* adapter,
    QWidget* parent)
    : AbstractHeatMapWidget(adapter, parent)
{
}

QColor GPSHeatMapWidget::colorAt(double t) const
{
    const QColor green("#2ecc71");
    const QColor brown("#8e5a2a");

    return QColor(
        green.red()   + t * (brown.red()   - green.red()),
        green.green() + t * (brown.green() - green.green()),
        green.blue()  + t * (brown.blue()  - green.blue())
        );
}

QString GPSHeatMapWidget::units() const
{
    return QStringLiteral("м");
}
