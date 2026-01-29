#pragma once

#include "ui/views/mapViews/abstractHeatMapWidget.h"
#include "core/spatial/GPSSpatialDataAdapter.h"

class GPSHeatMapWidget : public AbstractHeatMapWidget
{
    Q_OBJECT
public:
    explicit GPSHeatMapWidget(GPSSpatialDataAdapter* adapter,
                              QWidget* parent = nullptr);

protected:
    QColor colorAt(double t) const override;
    QString units() const override;
};
