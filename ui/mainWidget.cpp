#include "mainWidget.h"
#include <QHeaderView>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , storage_(new DataStorage(this))
    , undoStack_(new QUndoStack(this))
    , gpsModel_(new GPSMeasurementModel(storage_, this))
    , magneticModel_(new MagneticMeasurementModel(storage_, this))
    , reliefModel_(new ReferenceReliefModel(storage_, this))
{
    setupUI();

    // Пример добавления тестовых данных
    GPSMeasurement gps1(
        MeasurementPoint(1, 100),
        GeoCoordinate(55.7558, 37.6176, 156.0),
        QDateTime::currentDateTime(),
        GPSMeasurement::Source::GPS_Receiver
        );
    gpsModel_->addMeasurement(gps1);

    MagneticMeasurement magnetic1(
        MeasurementPoint(1, 100),
        25.5,
        MagneticMeasurement::Source::Field_Sensor,
        QDateTime::currentDateTime()
        );
    magneticModel_->addMeasurement(magnetic1);

    ReferenceReliefPoint relief1(
        GeoCoordinate(55.7558, 37.6176, 156.0),
        150.0,
        ReferenceReliefPoint::Source::Survey
        );
    reliefModel_->addPoint(relief1);
}

MainWidget::~MainWidget()
{
}

void MainWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Undo/Redo buttons
    QHBoxLayout* undoRedoLayout = new QHBoxLayout();
    QPushButton* undoButton = new QPushButton(tr("Undo"), this);
    QPushButton* redoButton = new QPushButton(tr("Redo"), this);
    connect(undoButton, &QPushButton::clicked, undoStack_, &QUndoStack::undo);
    connect(redoButton, &QPushButton::clicked, undoStack_, &QUndoStack::redo);
    undoRedoLayout->addWidget(undoButton);
    undoRedoLayout->addWidget(redoButton);
    undoRedoLayout->addStretch();

    mainLayout->addLayout(undoRedoLayout);

    // GPS Table
    QGroupBox* gpsGroup = new QGroupBox(tr("GPS Measurements"), this);
    QVBoxLayout* gpsLayout = new QVBoxLayout(gpsGroup);

    gpsTableView_ = new QTableView(this);
    gpsTableView_->setModel(gpsModel_);
    gpsTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    gpsTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    gpsTableView_->horizontalHeader()->setStretchLastSection(true);
    gpsTableView_->verticalHeader()->setVisible(true);

    QHBoxLayout* gpsButtonLayout = new QHBoxLayout();
    QPushButton* addGPSButton = new QPushButton(tr("Add GPS"), this);
    QPushButton* removeGPSButton = new QPushButton(tr("Remove GPS"), this);
    connect(addGPSButton, &QPushButton::clicked, this, &MainWidget::onAddGPS);
    connect(removeGPSButton, &QPushButton::clicked, this, &MainWidget::onRemoveGPS);

    gpsButtonLayout->addWidget(addGPSButton);
    gpsButtonLayout->addWidget(removeGPSButton);
    gpsButtonLayout->addStretch();

    gpsLayout->addWidget(gpsTableView_);
    gpsLayout->addLayout(gpsButtonLayout);

    // Magnetic Table
    QGroupBox* magneticGroup = new QGroupBox(tr("Magnetic Measurements"), this);
    QVBoxLayout* magneticLayout = new QVBoxLayout(magneticGroup);

    magneticTableView_ = new QTableView(this);
    magneticTableView_->setModel(magneticModel_);
    magneticTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    magneticTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    magneticTableView_->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout* magneticButtonLayout = new QHBoxLayout();
    QPushButton* addMagneticButton = new QPushButton(tr("Add Magnetic"), this);
    QPushButton* removeMagneticButton = new QPushButton(tr("Remove Magnetic"), this);
    connect(addMagneticButton, &QPushButton::clicked, this, &MainWidget::onAddMagnetic);
    connect(removeMagneticButton, &QPushButton::clicked, this, &MainWidget::onRemoveMagnetic);

    magneticButtonLayout->addWidget(addMagneticButton);
    magneticButtonLayout->addWidget(removeMagneticButton);
    magneticButtonLayout->addStretch();

    magneticLayout->addWidget(magneticTableView_);
    magneticLayout->addLayout(magneticButtonLayout);

    // Relief Table
    QGroupBox* reliefGroup = new QGroupBox(tr("Reference Relief Points"), this);
    QVBoxLayout* reliefLayout = new QVBoxLayout(reliefGroup);

    reliefTableView_ = new QTableView(this);
    reliefTableView_->setModel(reliefModel_);
    reliefTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    reliefTableView_->setSelectionMode(QAbstractItemView::SingleSelection);
    reliefTableView_->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout* reliefButtonLayout = new QHBoxLayout();
    QPushButton* addReliefButton = new QPushButton(tr("Add Relief Point"), this);
    QPushButton* removeReliefButton = new QPushButton(tr("Remove Relief Point"), this);
    connect(addReliefButton, &QPushButton::clicked, this, &MainWidget::onAddRelief);
    connect(removeReliefButton, &QPushButton::clicked, this, &MainWidget::onRemoveRelief);

    reliefButtonLayout->addWidget(addReliefButton);
    reliefButtonLayout->addWidget(removeReliefButton);
    reliefButtonLayout->addStretch();

    reliefLayout->addWidget(reliefTableView_);
    reliefLayout->addLayout(reliefButtonLayout);

    // Add all to main layout
    mainLayout->addWidget(gpsGroup);
    mainLayout->addWidget(magneticGroup);
    mainLayout->addWidget(reliefGroup);

    setLayout(mainLayout);
    resize(800, 600);
}

void MainWidget::onAddGPS()
{
    // Создаем диалог для ввода данных GPS
    GPSMeasurement gps(
        MeasurementPoint(1, 1),
        GeoCoordinate(0.0, 0.0, 0.0),
        QDateTime::currentDateTime(),
        GPSMeasurement::Source::Manual
        );
    undoStack_->push(new AddGPSCommand(storage_, gps));
}

void MainWidget::onRemoveGPS()
{
    QModelIndexList selected = gpsTableView_->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a row to remove"));
        return;
    }

    int row = selected.first().row();
    undoStack_->push(new RemoveGPSCommand(storage_, row));
}

void MainWidget::onAddMagnetic()
{
    MagneticMeasurement magnetic(
        MeasurementPoint(1, 1),
        0.0,
        MagneticMeasurement::Source::Manual,
        QDateTime::currentDateTime()
        );
    undoStack_->push(new AddMagneticCommand(storage_, magnetic));
}

void MainWidget::onRemoveMagnetic()
{
    QModelIndexList selected = magneticTableView_->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a row to remove"));
        return;
    }

    int row = selected.first().row();
    undoStack_->push(new RemoveMagneticCommand(storage_, row));
}

void MainWidget::onAddRelief()
{
    ReferenceReliefPoint relief(
        GeoCoordinate(0.0, 0.0, 0.0),
        0.0,
        ReferenceReliefPoint::Source::Survey
        );
    undoStack_->push(new AddReliefCommand(storage_, relief));
}

void MainWidget::onRemoveRelief()
{
    QModelIndexList selected = reliefTableView_->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select a row to remove"));
        return;
    }

    int row = selected.first().row();
    undoStack_->push(new RemoveReliefCommand(storage_, row));
}
