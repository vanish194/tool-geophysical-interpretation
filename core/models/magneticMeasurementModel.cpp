#include "core/models/magneticMeasurementModel.h"
#include <QColor>

MagneticMeasurementModel::MagneticMeasurementModel(DataStorage* storage, QObject* parent)
    : QAbstractTableModel(parent)
    , storage_(storage)
{
    connect(storage_, &DataStorage::magneticChanged,
            this, &MagneticMeasurementModel::onDataChanged);
}

MagneticMeasurementModel::~MagneticMeasurementModel()
{
}

int MagneticMeasurementModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(storage_->magnetic().size());
}

int MagneticMeasurementModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant MagneticMeasurementModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount())
        return QVariant();

    const auto& measurements = storage_->magnetic();
    if (index.row() >= static_cast<int>(measurements.size()))
        return QVariant();

    const auto& measurement = measurements[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case ColProfile:
            return measurement.point().profile();
        case ColPicket:
            return measurement.point().picket();
        case ColValue:
            return measurement.value();
        case ColSource:
            return sourceToString(measurement.source());
        case ColTimestamp:
            return measurement.timestamp().toString("yyyy-MM-dd HH:mm:ss");
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    else if (role == Qt::BackgroundRole) {
        if (measurement.source() == MagneticMeasurement::Source::Manual) {
            return QColor(255, 255, 200);
        }
        else if (measurement.source() == MagneticMeasurement::Source::Reference_Sensor) {
            return QColor(200, 255, 200); // Light green for reference
        }
    }

    return QVariant();
}

QVariant MagneticMeasurementModel::headerData(int section, Qt::Orientation orientation,
                                              int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case ColProfile:    return tr("Profile");
        case ColPicket:     return tr("Picket");
        case ColValue:      return tr("Value");
        case ColSource:     return tr("Source");
        case ColTimestamp:  return tr("Timestamp");
        default:            return QVariant();
        }
    }

    return section + 1;
}

bool MagneticMeasurementModel::setData(const QModelIndex& index, const QVariant& value,
                                       int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() >= rowCount() || index.column() >= columnCount())
        return false;

    auto measurement = storage_->magneticAt(index.row());
    bool success = false;

    switch (index.column()) {
    case ColProfile: {
        int profile = value.toInt(&success);
        if (success) {
            MeasurementPoint point = measurement.point();
            success = point.setProfile(profile);
            if (success) {
                measurement.setPoint(point);
                storage_->blockSignals(true);
                success = storage_->updateMagnetic(index.row(), measurement);
                storage_->blockSignals(false);
            }
        }
        break;
    }
    case ColPicket: {
        int picket = value.toInt(&success);
        if (success) {
            MeasurementPoint point = measurement.point();
            success = point.setPicket(picket);
            if (success) {
                measurement.setPoint(point);
                storage_->blockSignals(true);
                success = storage_->updateMagnetic(index.row(), measurement);
                storage_->blockSignals(false);
            }
        }
        break;
    }
    case ColValue: {
        double val = value.toDouble(&success);
        if (success) {
            measurement.setValue(val);
            storage_->blockSignals(true);
            success = storage_->updateMagnetic(index.row(), measurement);
            storage_->blockSignals(false);
        }
        break;
    }
    case ColSource: {
        auto source = stringToSource(value.toString());
        measurement.setSource(source);
        storage_->blockSignals(true);
        success = storage_->updateMagnetic(index.row(), measurement);
        storage_->blockSignals(false);
        break;
    }
    case ColTimestamp: {
        QDateTime timestamp = QDateTime::fromString(value.toString(), "yyyy-MM-dd HH:mm:ss");
        if (timestamp.isValid()) {
            measurement.setTimestamp(timestamp);
            storage_->blockSignals(true);
            success = storage_->updateMagnetic(index.row(), measurement);
            storage_->blockSignals(false);
        }
        break;
    }
    }

    if (success) {
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags MagneticMeasurementModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (index.isValid()) {
        flags |= Qt::ItemIsEditable;
        flags |= Qt::ItemIsSelectable;
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

bool MagneticMeasurementModel::addMeasurement(const MagneticMeasurement& measurement)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    storage_->blockSignals(true);
    storage_->addMagnetic(measurement);
    storage_->blockSignals(false);
    endInsertRows();
    return true;
}

bool MagneticMeasurementModel::removeMeasurement(int row)
{
    if (row < 0 || row >= rowCount())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    storage_->blockSignals(true);
    bool success = storage_->removeMagnetic(row);
    storage_->blockSignals(false);
    endRemoveRows();

    return success;
}

bool MagneticMeasurementModel::updateMeasurement(int row, const MagneticMeasurement& measurement)
{
    if (row < 0 || row >= rowCount())
        return false;

    storage_->blockSignals(true);
    bool success = storage_->updateMagnetic(row, measurement);
    storage_->blockSignals(false);
    if (success) {
        emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    }

    return success;
}

MagneticMeasurement MagneticMeasurementModel::getMeasurement(int row) const
{
    if (row < 0 || row >= rowCount())
        return MagneticMeasurement(0.0, QDateTime());

    return storage_->magneticAt(row);
}

void MagneticMeasurementModel::onDataChanged()
{
    beginResetModel();
    endResetModel();
}

QString MagneticMeasurementModel::sourceToString(MagneticMeasurement::Source source) const
{
    switch (source) {
    case MagneticMeasurement::Source::Field_Sensor:    return tr("Field Sensor");
    case MagneticMeasurement::Source::Reference_Sensor:return tr("Reference Sensor");
    case MagneticMeasurement::Source::Manual:          return tr("Manual");
    case MagneticMeasurement::Source::Unknown:         return tr("Unknown");
    default:                                           return tr("Unknown");
    }
}

MagneticMeasurement::Source MagneticMeasurementModel::stringToSource(const QString& str) const
{
    if (str == tr("Field Sensor"))     return MagneticMeasurement::Source::Field_Sensor;
    if (str == tr("Reference Sensor")) return MagneticMeasurement::Source::Reference_Sensor;
    if (str == tr("Manual"))           return MagneticMeasurement::Source::Manual;
    return MagneticMeasurement::Source::Unknown;
}
