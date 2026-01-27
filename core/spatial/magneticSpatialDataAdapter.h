#pragma once

#include "core/spatial/abstractSpatialDataAdapter.h"
#include "core/models/magneticMeasurementModel.h"

class MagneticSpatialDataAdapter : public AbstractSpatialDataAdapter
{
    Q_OBJECT

public:
    explicit MagneticSpatialDataAdapter(MagneticMeasurementModel* model,
                                        QObject* parent = nullptr);

    const QVector<SpatialPoint>& points() const override;

private slots:
    void rebuild();

private:
    MagneticMeasurementModel* model_ = nullptr;
    QVector<SpatialPoint> points_;
};
