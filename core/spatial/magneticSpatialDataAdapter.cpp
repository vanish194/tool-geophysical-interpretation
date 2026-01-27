#include "magneticSpatialDataAdapter.h"

MagneticSpatialDataAdapter::MagneticSpatialDataAdapter(
    MagneticMeasurementModel* model,
    QObject* parent)
    : AbstractSpatialDataAdapter(parent)
    , model_(model)
{
    Q_ASSERT(model_);
    //НУЖНО ПЕРЕСТАВИТЬ МОМЕНТ ОТРИСОВКИ
    connect(model_, &QAbstractItemModel::dataChanged,
            this, &MagneticSpatialDataAdapter::rebuild);
    connect(model_, &QAbstractItemModel::modelReset,
            this, &MagneticSpatialDataAdapter::rebuild);
    connect(model_, &QAbstractItemModel::rowsInserted,
            this, &MagneticSpatialDataAdapter::rebuild);
    connect(model_, &QAbstractItemModel::rowsRemoved,
            this, &MagneticSpatialDataAdapter::rebuild);

    rebuild();
}

const QVector<SpatialPoint>& MagneticSpatialDataAdapter::points() const
{
    return points_;
}

void MagneticSpatialDataAdapter::rebuild()
{
    points_.clear();

    QMap<QPair<int, int>, AvgAccumulator> acc;

    for (int row = 0; row < model_->rowCount(); ++row) {
        const auto m = model_->getMeasurement(row);

        QPair<int, int> key{
            m.point().profile(),
            m.point().picket()
        };

        auto& cell = acc[key];
        cell.sum += m.value();
        cell.count++;
    }

    points_.reserve(acc.size());

    for (auto it = acc.begin(); it != acc.end(); ++it) {
        SpatialPoint p;
        p.x = it.key().first;
        p.y = it.key().second;
        p.value = it.value().sum / it.value().count;

        points_.push_back(p);
    }

    emit dataChanged();
}
