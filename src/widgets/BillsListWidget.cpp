// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "BillsListWidget.h"
#include "ui_BillsListWidget.h"
#include <models/BillsModel.h>
#include <delegates/CenteredCheckBoxDelegate.h>
#include <QDebug>

BillsListWidget::BillsListWidget(BillsModel *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BillsListWidget)
    , m_model(model)
{
    ui->setupUi(this);

    BillsProxyModel *proxyModel = new BillsProxyModel(this);
    proxyModel->setSourceModel(m_model);

    ui->billsTableView->setModel(proxyModel);

    ui->billsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->billsTableView->setSortingEnabled(true);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, proxyModel, &BillsProxyModel::setFilteringPattern);
    connect(ui->hideNormalCheckBox, &QCheckBox::stateChanged, proxyModel, &BillsProxyModel::setHideNormal);
    connect(ui->hideRecurringCheckBox, &QCheckBox::stateChanged, proxyModel, &BillsProxyModel::setHideRecurring);

    connect(ui->hideNormalCheckBox, &QCheckBox::stateChanged, this, [&] {
       emit filteringApplied(ui->searchLineEdit->text(), ui->hideRecurringCheckBox->isChecked(), ui->hideNormalCheckBox->isChecked());
    });
    connect(ui->hideRecurringCheckBox, &QCheckBox::stateChanged, this, [&] {
       emit filteringApplied(ui->searchLineEdit->text(), ui->hideRecurringCheckBox->isChecked(), ui->hideNormalCheckBox->isChecked());
    });
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [&] {
       emit filteringApplied(ui->searchLineEdit->text(), ui->hideRecurringCheckBox->isChecked(), ui->hideNormalCheckBox->isChecked());
    });

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [&](const QString &text) {
        emit filteringApplied(text, ui->hideRecurringCheckBox->isChecked(), ui->hideNormalCheckBox->isChecked());
    });

    ui->billsTableView->setItemDelegateForColumn(2, new CenteredCheckBoxDelegate(this));
}

void BillsListWidget::updatePLNAmountLabel(const QString& text)
{
    ui->amountPlnLabel->setText(text);
}

void BillsListWidget::resetCheckBoxesAndClearSearch()
{
    ui->hideNormalCheckBox->setChecked(false);
    ui->hideRecurringCheckBox->setChecked(false);
    ui->searchLineEdit->clear();
}

BillsListWidget::~BillsListWidget()
{
    delete ui;
}
