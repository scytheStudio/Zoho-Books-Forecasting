// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "ExpensesListWidget.h"
#include "ui_ExpensesListWidget.h"
#include "models/ExpensesModel.h"
#include <QDebug>
#include <delegates/CenteredCheckBoxDelegate.h>

ExpensesListWidget::ExpensesListWidget(ExpensesModel *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExpensesListWidget)
    , m_model(model)
{
    ui->setupUi(this);

    ExpensesProxyModel *proxyModel = new ExpensesProxyModel(this);
    proxyModel->setSourceModel(m_model);

    ui->expensesTableView->setModel(proxyModel);

    ui->expensesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->expensesTableView->setSortingEnabled(true);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, proxyModel, &ExpensesProxyModel::setFilteringPattern);
    connect(ui->hideNormalCheckBox, &QCheckBox::stateChanged, proxyModel, &ExpensesProxyModel::setHideNormal);
    connect(ui->hideRecurringCheckBox, &QCheckBox::stateChanged, proxyModel, &ExpensesProxyModel::setHideRecurring);

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

    ui->expensesTableView->setItemDelegateForColumn(4, new CenteredCheckBoxDelegate(this));
}

void ExpensesListWidget::updatePLNAmountLabel(const QString& text)
{
    ui->amountPlnLabel->setText(text);
}

void ExpensesListWidget::resetCheckBoxesAndClearSearch()
{
    ui->hideNormalCheckBox->setChecked(false);
    ui->hideRecurringCheckBox->setChecked(false);
    ui->searchLineEdit->clear();
}

ExpensesListWidget::~ExpensesListWidget()
{
    delete ui;
}
