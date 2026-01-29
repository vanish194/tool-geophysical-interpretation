#pragma once

#include <QUndoCommand>
#include "core/storage/dataStorage.h"
#include "core/datatypes/GPSMeasurement.h"
#include "core/datatypes/magneticMeasurement.h"
#include "core/datatypes/referenceReliefPoint.h"

// Add GPS Command
class AddGPSCommand : public QUndoCommand
{
public:
    AddGPSCommand(DataStorage* storage, const GPSMeasurement& measurement, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    GPSMeasurement measurement_;
};

// Remove GPS Command
class RemoveGPSCommand : public QUndoCommand
{
public:
    RemoveGPSCommand(DataStorage* storage, int index, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    int index_;
    GPSMeasurement removedMeasurement_;
};

// Add Magnetic Command
class AddMagneticCommand : public QUndoCommand
{
public:
    AddMagneticCommand(DataStorage* storage, const MagneticMeasurement& measurement, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    MagneticMeasurement measurement_;
};

// Remove Magnetic Command
class RemoveMagneticCommand : public QUndoCommand
{
public:
    RemoveMagneticCommand(DataStorage* storage, int index, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    int index_;
    MagneticMeasurement removedMeasurement_;
};

// Add Relief Command
class AddReliefCommand : public QUndoCommand
{
public:
    AddReliefCommand(DataStorage* storage, const ReferenceReliefPoint& point, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    ReferenceReliefPoint point_;
};

// Remove Relief Command
class RemoveReliefCommand : public QUndoCommand
{
public:
    RemoveReliefCommand(DataStorage* storage, int index, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    int index_;
    ReferenceReliefPoint removedPoint_;
};

// Update GPS Command
class UpdateGPSCommand : public QUndoCommand
{
public:
    UpdateGPSCommand(DataStorage* storage, int index, const GPSMeasurement& oldMeasurement, const GPSMeasurement& newMeasurement, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    int index_;
    GPSMeasurement oldMeasurement_;
    GPSMeasurement newMeasurement_;
};

// Update Magnetic Command
class UpdateMagneticCommand : public QUndoCommand
{
public:
    UpdateMagneticCommand(DataStorage* storage, int index, const MagneticMeasurement& oldMeasurement, const MagneticMeasurement& newMeasurement, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    int index_;
    MagneticMeasurement oldMeasurement_;
    MagneticMeasurement newMeasurement_;
};

// Update Relief Command
class UpdateReliefCommand : public QUndoCommand
{
public:
    UpdateReliefCommand(DataStorage* storage, int index, const ReferenceReliefPoint& oldPoint, const ReferenceReliefPoint& newPoint, QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    DataStorage* storage_;
    int index_;
    ReferenceReliefPoint oldPoint_;
    ReferenceReliefPoint newPoint_;
};