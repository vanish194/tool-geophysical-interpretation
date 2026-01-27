#include "commands.h"

// AddGPSCommand
AddGPSCommand::AddGPSCommand(DataStorage* storage, const GPSMeasurement& measurement, QUndoCommand* parent)
    : QUndoCommand("Add GPS Measurement", parent), storage_(storage), measurement_(measurement)
{
}

void AddGPSCommand::redo()
{
    storage_->addGPS(measurement_);
}

void AddGPSCommand::undo()
{
    storage_->removeGPS(storage_->gps().size() - 1);
}

// RemoveGPSCommand
RemoveGPSCommand::RemoveGPSCommand(DataStorage* storage, int index, QUndoCommand* parent)
    : QUndoCommand("Remove GPS Measurement", parent), storage_(storage), index_(index)
{
    removedMeasurement_ = storage_->gpsAt(index);
}

void RemoveGPSCommand::redo()
{
    storage_->removeGPS(index_);
}

void RemoveGPSCommand::undo()
{
    storage_->addGPS(removedMeasurement_);
}

// AddMagneticCommand
AddMagneticCommand::AddMagneticCommand(DataStorage* storage, const MagneticMeasurement& measurement, QUndoCommand* parent)
    : QUndoCommand("Add Magnetic Measurement", parent), storage_(storage), measurement_(measurement)
{
}

void AddMagneticCommand::redo()
{
    storage_->addMagnetic(measurement_);
}

void AddMagneticCommand::undo()
{
    storage_->removeMagnetic(storage_->magnetic().size() - 1);
}

// RemoveMagneticCommand
RemoveMagneticCommand::RemoveMagneticCommand(DataStorage* storage, int index, QUndoCommand* parent)
    : QUndoCommand("Remove Magnetic Measurement", parent), storage_(storage), index_(index)
{
    removedMeasurement_ = storage_->magneticAt(index);
}

void RemoveMagneticCommand::redo()
{
    storage_->removeMagnetic(index_);
}

void RemoveMagneticCommand::undo()
{
    storage_->addMagnetic(removedMeasurement_);
}

// AddReliefCommand
AddReliefCommand::AddReliefCommand(DataStorage* storage, const ReferenceReliefPoint& point, QUndoCommand* parent)
    : QUndoCommand("Add Relief Point", parent), storage_(storage), point_(point)
{
}

void AddReliefCommand::redo()
{
    storage_->addRelief(point_);
}

void AddReliefCommand::undo()
{
    storage_->removeRelief(storage_->relief().size() - 1);
}

// RemoveReliefCommand
RemoveReliefCommand::RemoveReliefCommand(DataStorage* storage, int index, QUndoCommand* parent)
    : QUndoCommand("Remove Relief Point", parent), storage_(storage), index_(index)
{
    removedPoint_ = storage_->reliefAt(index);
}

void RemoveReliefCommand::redo()
{
    storage_->removeRelief(index_);
}

void RemoveReliefCommand::undo()
{
    storage_->addRelief(removedPoint_);
}