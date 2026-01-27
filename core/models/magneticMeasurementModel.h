#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include "core/storage/dataStorage.h"

class MagneticMeasurementModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MagneticMeasurementModel(DataStorage* storage, QObject* parent = nullptr);
    ~MagneticMeasurementModel();

    // QAbstractTableModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Custom methods
    bool addMeasurement(const MagneticMeasurement& measurement);
    bool removeMeasurement(int row);
    bool updateMeasurement(int row, const MagneticMeasurement& measurement);
    MagneticMeasurement getMeasurement(int row) const;

    enum Columns {
        ColProfile = 0,
        ColPicket,
        ColValue,
        ColSource,
        ColTimestamp,
        ColumnCount
    };

signals:
    void measurementUpdated(int row, const MagneticMeasurement& oldMeasurement, const MagneticMeasurement& newMeasurement);

private slots:
    void onDataChanged();

private:
    DataStorage* storage_;
    QString sourceToString(MagneticMeasurement::Source source) const;
    MagneticMeasurement::Source stringToSource(const QString& str) const;
};
