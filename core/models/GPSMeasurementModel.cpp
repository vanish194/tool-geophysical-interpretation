#include "core/models/GPSMeasurementModel.h"
#include <QColor>

GPSMeasurementModel::GPSMeasurementModel(DataStorage* storage, QObject* parent)
    : QAbstractTableModel(parent)
    , storage_(storage)
{
    connect(storage_, &DataStorage::gpsChanged, this, &GPSMeasurementModel::onDataChanged);
}

GPSMeasurementModel::~GPSMeasurementModel()
{
}

int GPSMeasurementModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(storage_->gps().size());
}

int GPSMeasurementModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant GPSMeasurementModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount() || index.column() >= columnCount())
        return QVariant();

    const auto& measurements = storage_->gps();
    if (index.row() >= static_cast<int>(measurements.size()))
        return QVariant();

    const auto& measurement = measurements[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case ColProfile:
            return measurement.point().profile();
        case ColPicket:
            return measurement.point().picket();
        case ColLatitude:
            return measurement.coordinate().latitude();
        case ColLongitude:
            return measurement.coordinate().longitude();
        case ColAltitude:
            return measurement.coordinate().altitude();
        case ColTimestamp:
            return measurement.timestamp().toString("yyyy-MM-dd HH:mm:ss");
        case ColSource:
            return sourceToString(measurement.source());
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    else if (role == Qt::BackgroundRole) {
        if (measurement.source() == GPSMeasurement::Source::Manual) {
            return QColor(255, 255, 200); // Light yellow for manual entries
        }
    }

    return QVariant();
}

QVariant GPSMeasurementModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case ColProfile:    return tr("Profile");
        case ColPicket:     return tr("Picket");
        case ColLatitude:   return tr("Latitude");
        case ColLongitude:  return tr("Longitude");
        case ColAltitude:   return tr("Altitude");
        case ColTimestamp:  return tr("Timestamp");
        case ColSource:     return tr("Source");
        default:            return QVariant();
        }
    }

    return section + 1; // Row numbers
}

bool GPSMeasurementModel::setData(const QModelIndex& index, const QVariant& value,
                                  int role)
{
    if (!index.isValid() || role != Qt::EditRole ||
        index.row() >= rowCount() || index.column() >= columnCount())
        return false;

    auto measurement = storage_->gpsAt(index.row());
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
                success = storage_->updateGPS(index.row(), measurement);
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
                success = storage_->updateGPS(index.row(), measurement);
                storage_->blockSignals(false);
            }
        }
        break;
    }
    case ColLatitude: {
        double latitude = value.toDouble(&success);
        if (success) {
            GeoCoordinate coord = measurement.coordinate();
            success = coord.setLatitude(latitude);
            if (success) {
                measurement.setCoordinate(coord);
                storage_->blockSignals(true);
                success = storage_->updateGPS(index.row(), measurement);
                storage_->blockSignals(false);
            }
        }
        break;
    }
    case ColLongitude: {
        double longitude = value.toDouble(&success);
        if (success) {
            GeoCoordinate coord = measurement.coordinate();
            success = coord.setLongitude(longitude);
            if (success) {
                measurement.setCoordinate(coord);
                storage_->blockSignals(true);
                success = storage_->updateGPS(index.row(), measurement);
                storage_->blockSignals(false);
            }
        }
        break;
    }
    case ColAltitude: {
        double altitude = value.toDouble(&success);
        if (success) {
            GeoCoordinate coord = measurement.coordinate();
            success = coord.setAltitude(altitude);
            if (success) {
                measurement.setCoordinate(coord);
                storage_->blockSignals(true);
                success = storage_->updateGPS(index.row(), measurement);
                storage_->blockSignals(false);
            }
        }
        break;
    }
    case ColTimestamp: {
        QDateTime timestamp = QDateTime::fromString(value.toString(), "yyyy-MM-dd HH:mm:ss");
        if (timestamp.isValid()) {
            measurement.setTimestamp(timestamp);
            storage_->blockSignals(true);
            success = storage_->updateGPS(index.row(), measurement);
            storage_->blockSignals(false);
        }
        break;
    }
    case ColSource: {
        auto source = stringToSource(value.toString());
        measurement.setSource(source);
        storage_->blockSignals(true);
        success = storage_->updateGPS(index.row(), measurement);
        storage_->blockSignals(false);
        break;
    }
    }

    if (success) {
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags GPSMeasurementModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (index.isValid()) {
        flags |= Qt::ItemIsEditable;
        flags |= Qt::ItemIsSelectable;
        flags |= Qt::ItemIsEnabled;
    }

    return flags;
}

bool GPSMeasurementModel::addMeasurement(const GPSMeasurement& measurement)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    storage_->blockSignals(true);
    storage_->addGPS(measurement);
    storage_->blockSignals(false);
    endInsertRows();
    return true;
}

bool GPSMeasurementModel::removeMeasurement(int row)
{
    if (row < 0 || row >= rowCount())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    storage_->blockSignals(true);
    bool success = storage_->removeGPS(row);
    storage_->blockSignals(false);
    endRemoveRows();

    return success;
}

bool GPSMeasurementModel::updateMeasurement(int row, const GPSMeasurement& measurement)
{
    if (row < 0 || row >= rowCount())
        return false;

    storage_->blockSignals(true);
    bool success = storage_->updateGPS(row, measurement);
    storage_->blockSignals(false);
    if (success) {
        emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    }

    return success;
}

GPSMeasurement GPSMeasurementModel::getMeasurement(int row) const
{
    if (row < 0 || row >= rowCount())
        return GPSMeasurement(MeasurementPoint(), GeoCoordinate(), QDateTime());

    return storage_->gpsAt(row);
}

void GPSMeasurementModel::onDataChanged()
{
    beginResetModel();
    endResetModel();
}

QString GPSMeasurementModel::sourceToString(GPSMeasurement::Source source) const
{
    switch (source) {
    case GPSMeasurement::Source::GPS_Receiver: return tr("GPS Receiver");
    case GPSMeasurement::Source::Manual:       return tr("Manual");
    case GPSMeasurement::Source::Unknown:      return tr("Unknown");
    default:                                   return tr("Unknown");
    }
}

GPSMeasurement::Source GPSMeasurementModel::stringToSource(const QString& str) const
{
    if (str == tr("GPS Receiver")) return GPSMeasurement::Source::GPS_Receiver;
    if (str == tr("Manual"))       return GPSMeasurement::Source::Manual;
    return GPSMeasurement::Source::Unknown;
}
