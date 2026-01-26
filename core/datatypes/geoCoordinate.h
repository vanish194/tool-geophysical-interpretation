#pragma once

#include <stdexcept>
#include <utility>

class GeoCoordinate
{
public:
    GeoCoordinate() = default;

    GeoCoordinate(double latitude, double longitude, double altitude)
    {
        setLatitude(latitude);
        setLongitude(longitude);
        setAltitude(altitude);
    }

    double latitude()  const noexcept { return latitude_; }
    double longitude() const noexcept { return longitude_; }
    double altitude()  const noexcept { return altitude_; }

    bool setLatitude(double latitude) noexcept
    {
        if (latitude < -90.0 || latitude > 90.0)
            return false;
        latitude_ = latitude;
        return true;
    }

    bool setLongitude(double longitude) noexcept
    {
        if (longitude < -180.0 || longitude > 180.0)
            return false;
        longitude_ = longitude;
        return true;
    }

    bool setAltitude(double altitude) noexcept
    {
        altitude_ = altitude;
        return true;
    }

private:
    double latitude_  = 0.0;
    double longitude_ = 0.0;
    double altitude_  = 0.0;
};

