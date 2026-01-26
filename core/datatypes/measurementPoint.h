#pragma once

#include <stdexcept>

class MeasurementPoint
{
public:
    MeasurementPoint() = default;

    MeasurementPoint(int profile, int picket)
    {
        setProfile(profile);
        setPicket(picket);
    }

    int profile() const noexcept { return profile_; }
    int picket()  const noexcept { return picket_; }

    bool setProfile(int profile) noexcept
    {
        profile_ = profile;
        return true;
    }

    bool setPicket(int picket) noexcept
    {
        picket_ = picket;
        return true;
    }

    bool isValid() const noexcept
    {
        return true;
    }

    bool operator==(const MeasurementPoint& other) const noexcept
    {
        return profile_ == other.profile_ && picket_ == other.picket_;
    }

    bool operator<(const MeasurementPoint& other) const noexcept
    {
        return (profile_ < other.profile_) ||
               (profile_ == other.profile_ && picket_ < other.picket_);
    }

private:
    int profile_ = 0;
    int picket_  = 0;
};

