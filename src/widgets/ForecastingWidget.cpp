// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "ForecastingWidget.h"
#include <QMessageBox>
#include "ui_ForecastingWidget.h"
#include "models/ForecastingModel.h"
#include <delegates/DateEditDelegate.h>
#include <delegates/CenteredCheckBoxDelegate.h>

ForecastingWidget::ForecastingWidget(ForecastingModel *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForecastingWidget)
    , m_model(model)
{
    ui->setupUi(this);
    ui->forecastingTableView->setModel(m_model);
    ui->forecastingTableView->setSortingEnabled(true);
    ui->forecastingTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->addButton, &QPushButton::clicked, this, &ForecastingWidget::onAddButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ForecastingWidget::onDeleteButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &ForecastingWidget::onClearButtonClicked);

    ui->forecastingTableView->setItemDelegateForColumn(2, new DateEditDelegate(this));
    ui->forecastingTableView->setItemDelegateForColumn(3, new CenteredCheckBoxDelegate(this));
    ui->forecastingTableView->setItemDelegateForColumn(4, new CenteredCheckBoxDelegate(this));
}

ForecastingWidget::~ForecastingWidget()
{
    delete ui;
}

void ForecastingWidget::onAddButtonClicked()
{
    m_model->addEntry(ForecastingModel::Forecast());
}

void ForecastingWidget::onDeleteButtonClicked()
{
    if (ui->forecastingTableView->selectionModel()->selectedRows().size() == 0) {
        return;
    }

    int ret = QMessageBox::warning(this, "Zoho Books Forecasting", tr("Are you sure you want to remove selected rows?"),
                                QMessageBox::No | QMessageBox::Yes,
                                QMessageBox::No);

    if (ret == QMessageBox::Yes) {
       auto selectionModel = ui->forecastingTableView->selectionModel();
       auto selectedRows = selectionModel->selectedRows();

       for (int i = 0; i < selectedRows.size(); ++i) {
           m_model->removeEntry(selectedRows.at(i).row());
       }
    }
}

void ForecastingWidget::onClearButtonClicked()
{
    int ret = QMessageBox::warning(this, "Zoho Books Forecasting", tr("Are you sure you want to clear forecasting table?"),
                                QMessageBox::No | QMessageBox::Yes,
                                QMessageBox::No);

    if (ret == QMessageBox::Yes) {
       m_model->clearEntries();
    }
    emit m_model->rowChanged();
}

QCheckBox *ForecastingWidget::enableForecastingCheckBox()
{
    return ui->enableForecastingCheckBox;
}

void ForecastingWidget::setForecastingTableEnabled(bool value)
{
    ui->forecastingTableView->setEnabled(value);
    ui->futureItemsLabel->setEnabled(value);
}

void ForecastingWidget::setButtonsEnabled(bool value)
{
    ui->addButton->setEnabled(value);
    ui->deleteButton->setEnabled(value);
    ui->clearButton->setEnabled(value);
}
