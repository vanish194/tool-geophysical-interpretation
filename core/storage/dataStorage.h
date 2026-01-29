#pragma once

#include <QObject>
#include <vector>
#include <memory>

#include "core/datatypes/GPSMeasurement.h"
#include "core/datatypes/magneticMeasurement.h"
#include "core/datatypes/referenceReliefPoint.h"

class DataStorage : public QObject
{
    Q_OBJECT

public:
    explicit DataStorage(QObject* parent = nullptr);

    // ---- GPS ------------------------------------------------------
    void addGPS(GPSMeasurement m);
    void addGPSBatch(const std::vector<GPSMeasurement>& items);
    bool removeGPS(int index);
    bool updateGPS(int index, const GPSMeasurement& m);
    void clearGPS();
    const std::vector<GPSMeasurement>& gps() const noexcept;
    GPSMeasurement& gpsAt(int index);

    // ---- Magnetic -------------------------------------------------
    void addMagnetic(MagneticMeasurement m);
    void addMagneticBatch(const std::vector<MagneticMeasurement>& items);
    bool removeMagnetic(int index);
    bool updateMagnetic(int index, const MagneticMeasurement& m);
    void clearMagnetic();
    const std::vector<MagneticMeasurement>& magnetic() const noexcept;
    MagneticMeasurement& magneticAt(int index);

    // ---- Relief ---------------------------------------------------
    void addRelief(ReferenceReliefPoint p);
    void addReliefBatch(const std::vector<ReferenceReliefPoint>& items);
    bool removeRelief(int index);
    bool updateRelief(int index, const ReferenceReliefPoint& p);
    void clearRelief();
    const std::vector<ReferenceReliefPoint>& relief() const noexcept;
    ReferenceReliefPoint& reliefAt(int index);

signals:
    void gpsChanged();
    void magneticChanged();
    void reliefChanged();

private:
    std::vector<GPSMeasurement>       gps_;
    std::vector<MagneticMeasurement>  magnetic_;
    std::vector<ReferenceReliefPoint> relief_;
};
