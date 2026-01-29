#include "core/storage/dataStorage.h"
#include <QDebug>

DataStorage::DataStorage(QObject* parent)
    : QObject(parent)
{
    qDebug() << "DataStorage::DataStorage - constructor";
}

// ---- GPS --------------------------------------------------------
void DataStorage::addGPS(GPSMeasurement m)
{
    gps_.push_back(std::move(m));
    qDebug() << "DataStorage::addGPS";
    emit gpsChanged();
}

void DataStorage::addGPSBatch(const std::vector<GPSMeasurement>& items)
{
    if (items.empty())
        return;

    gps_.reserve(gps_.size() + items.size());
    for (const auto& it : items) {
        gps_.push_back(it);
    }
    qDebug() << "DataStorage::addGPSBatch - added" << items.size() << "items";
    emit gpsChanged();
}

bool DataStorage::removeGPS(int index)
{
    if (index < 0 || index >= static_cast<int>(gps_.size()))
        return false;

    gps_.erase(gps_.begin() + index);
    qDebug() << "DataStorage::removeGPS at index" << index;
    emit gpsChanged();
    return true;
}

bool DataStorage::updateGPS(int index, const GPSMeasurement& m)
{
    if (index < 0 || index >= static_cast<int>(gps_.size()))
        return false;

    gps_[index] = m;
    qDebug() << "DataStorage::updateGPS at index" << index;
    emit gpsChanged();
    return true;
}

void DataStorage::clearGPS()
{
    gps_.clear();
    qDebug() << "DataStorage::clearGPS";
    emit gpsChanged();
}

const std::vector<GPSMeasurement>& DataStorage::gps() const noexcept
{
    //qDebug() << "DataStorage::gps getter, size:" << gps_.size();
    return gps_;
}

GPSMeasurement& DataStorage::gpsAt(int index)
{
    qDebug() << "DataStorage::gpsAt index:" << index;
    return gps_[index];
}

// ---- Magnetic ---------------------------------------------------
void DataStorage::addMagnetic(MagneticMeasurement m)
{
    magnetic_.push_back(std::move(m));
    qDebug() << "DataStorage::addMagnetic";
    emit magneticChanged();
}

void DataStorage::addMagneticBatch(const std::vector<MagneticMeasurement>& items)
{
    if (items.empty())
        return;

    magnetic_.reserve(magnetic_.size() + items.size());
    for (const auto& it : items) {
        magnetic_.push_back(it);
    }
    qDebug() << "DataStorage::addMagneticBatch - added" << items.size() << "items";
    emit magneticChanged();
}

bool DataStorage::removeMagnetic(int index)
{
    if (index < 0 || index >= static_cast<int>(magnetic_.size()))
        return false;

    magnetic_.erase(magnetic_.begin() + index);
    qDebug() << "DataStorage::removeMagnetic at index" << index;
    emit magneticChanged();
    return true;
}

bool DataStorage::updateMagnetic(int index, const MagneticMeasurement& m)
{
    if (index < 0 || index >= static_cast<int>(magnetic_.size()))
        return false;

    magnetic_[index] = m;
    qDebug() << "DataStorage::updateMagnetic at index" << index;
    emit magneticChanged();
    return true;
}

void DataStorage::clearMagnetic()
{
    magnetic_.clear();
    qDebug() << "DataStorage::clearMagnetic";
    emit magneticChanged();
}

const std::vector<MagneticMeasurement>& DataStorage::magnetic() const noexcept
{
    //qDebug() << "DataStorage::magnetic getter, size:" << magnetic_.size();
    return magnetic_;
}

MagneticMeasurement& DataStorage::magneticAt(int index)
{
    qDebug() << "DataStorage::magneticAt index:" << index;
    return magnetic_[index];
}

// ---- Relief -----------------------------------------------------
void DataStorage::addRelief(ReferenceReliefPoint p)
{
    relief_.push_back(std::move(p));
    qDebug() << "DataStorage::addRelief";
    emit reliefChanged();
}

void DataStorage::addReliefBatch(const std::vector<ReferenceReliefPoint>& items)
{
    if (items.empty())
        return;

    relief_.reserve(relief_.size() + items.size());
    for (const auto& it : items) {
        relief_.push_back(it);
    }
    qDebug() << "DataStorage::addReliefBatch - added" << items.size() << "items";
    emit reliefChanged();
}

bool DataStorage::removeRelief(int index)
{
    if (index < 0 || index >= static_cast<int>(relief_.size()))
        return false;

    relief_.erase(relief_.begin() + index);
    qDebug() << "DataStorage::removeRelief at index" << index;
    emit reliefChanged();
    return true;
}

bool DataStorage::updateRelief(int index, const ReferenceReliefPoint& p)
{
    if (index < 0 || index >= static_cast<int>(relief_.size()))
        return false;

    relief_[index] = p;
    qDebug() << "DataStorage::updateRelief at index" << index;
    emit reliefChanged();
    return true;
}

void DataStorage::clearRelief()
{
    relief_.clear();
    qDebug() << "DataStorage::clearRelief";
    emit reliefChanged();
}

const std::vector<ReferenceReliefPoint>& DataStorage::relief() const noexcept
{
    //qDebug() << "DataStorage::relief getter, size:" << relief_.size();
    return relief_;
}

ReferenceReliefPoint& DataStorage::reliefAt(int index)
{
    qDebug() << "DataStorage::reliefAt index:" << index;
    return relief_[index];
}
