#pragma once

#include "ui/views/mapViews/abstractHeatMapWidget.h"
#include "core/spatial/magneticSpatialDataAdapter.h"

class MagneticHeatMapWidget : public AbstractHeatMapWidget
{
    Q_OBJECT
public:
    explicit MagneticHeatMapWidget(MagneticSpatialDataAdapter* adapter,
                                   QWidget* parent = nullptr);

protected:
    QColor colorAt(double t) const override;
    QString units() const override;
};
