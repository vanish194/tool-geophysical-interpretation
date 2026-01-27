#pragma once

#include "core/spatial/abstractSpatialDataAdapter.h"
#include "core/models/GPSMeasurementModel.h"

class GPSSpatialDataAdapter : public AbstractSpatialDataAdapter
{
    Q_OBJECT

public:
    explicit GPSSpatialDataAdapter(GPSMeasurementModel* model,
                                   QObject* parent = nullptr);

    const QVector<SpatialPoint>& points() const override;

private slots:
    void rebuild();

private:
    GPSMeasurementModel* model_ = nullptr;
    QVector<SpatialPoint> points_;
};
