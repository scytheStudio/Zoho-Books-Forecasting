// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "InvoicesListWidget.h"
#include "ui_InvoicesListWidget.h"
#include "models/InvoicesModel.h"

InvoicesListWidget::InvoicesListWidget(InvoicesModel *model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InvoicesListWidget)
    , m_model(model)
{
    ui->setupUi(this);

    InvoicesProxyModel *proxyModel = new InvoicesProxyModel(this);
    proxyModel->setSourceModel(m_model);

    ui->invoicesTableView->setModel(proxyModel);

    ui->invoicesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->invoicesTableView->setSortingEnabled(true);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, proxyModel, &InvoicesProxyModel::setFilteringPattern);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [&](const QString &text) {
        emit filteringApplied(text);
    });
}

void InvoicesListWidget::updatePLNAmountLabel(const QString& text)
{
    ui->amountPlnLabel->setText(text);
}

void InvoicesListWidget::clearSearchLine()
{
    ui->searchLineEdit->clear();
}

InvoicesListWidget::~InvoicesListWidget()
{
    delete ui;
}
