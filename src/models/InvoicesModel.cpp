// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "InvoicesModel.h"
#include "LogicController.h"
#include <QDebug>

InvoicesModel::InvoicesModel(LogicController *logicController, QObject *parent)
    : QAbstractTableModel(parent)
    , m_logicController(logicController)
{

}

int InvoicesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_logicController->rangedInvoices().count();
}

int InvoicesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant InvoicesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < m_logicController->rangedInvoices().count()) {
        if (index.isValid()) {
            if (role == Qt::DisplayRole) {
                const int row = index.row();
                // Since the application makes request for all financial history,
                // we need only those data between from date and to date.
                const auto &invoice = m_logicController->rangedInvoices().at(row);
                switch (index.column()) {
                case NumberColumn:
                    return invoice.invoiceNumber().isEmpty() ? "-" : invoice.invoiceNumber();
                case PartyColumn:
                    return invoice.party().isEmpty() ? "-" : invoice.party();
                case StatusColumn:
                    return invoice.status().isEmpty() ? "-" : invoice.status();
                case DateColumn:
                    return !invoice.date().isValid() ? "-" : invoice.date().toString("d/M/yyyy");
                case DueDateColumn:
                    return !invoice.dueDate().isValid() ? "-" : invoice.dueDate().toString("d/M/yyyy");
                case TotalColumn:
                    // Second parameter of toCurrencyString() method is intentionally " "
                    // in order not to display currency symbol.
                    return QLocale().toCurrencyString(invoice.total(), " ") + " " + invoice.currencyCode();
                case PlnTotalColumn:
                    // Since the application makes a request for the whole financial history,
                    // we need the data between 'from date' and 'to date' only.
                    return QLocale().toCurrencyString(invoice.plnTotal(), " ") + " PLN";
                }
            } else if (role == Qt::TextAlignmentRole) {
                switch (index.column()) {
                case TotalColumn:
                    return {Qt::AlignRight | Qt::AlignVCenter};
                case PlnTotalColumn:
                    return {Qt::AlignRight | Qt::AlignVCenter};
                }
                return Qt::AlignCenter;
            }
        }
    }
    return {};
}

QVariant InvoicesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && -1 < section && section < ColumnCount) {
        switch (section) {
        case NumberColumn:
            return "Number";
        case PartyColumn:
            return "Party";
        case StatusColumn:
            return "Status";
        case DateColumn:
            return "Date";
        case DueDateColumn:
            return "Due date";
        case TotalColumn:
            return "Amount";
        case PlnTotalColumn:
            return "PLN amount";
        }
    }

    return {};
}

void InvoicesModel::loadData()
{
    beginResetModel();
    endResetModel();
}

InvoicesProxyModel::InvoicesProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_filteringRegExp.setCaseSensitivity(Qt::CaseInsensitive);
}

bool InvoicesProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    QString number = sourceModel()->index(sourceRow, 0).data().toString();
    QString party = sourceModel()->index(sourceRow, 1).data().toString();
    QString status = sourceModel()->index(sourceRow, 2).data().toString();
    QString date = sourceModel()->index(sourceRow, 3).data().toString();
    QString dueDate = sourceModel()->index(sourceRow, 4).data().toString();
    QString amount = sourceModel()->index(sourceRow, 5).data().toString();
    QString currency = sourceModel()->index(sourceRow, 6).data().toString();
    QString plnAmount = sourceModel()->index(sourceRow, 7).data().toString();

    return number.contains(m_filteringRegExp) || party.contains(m_filteringRegExp) || status.contains(m_filteringRegExp)
        || date.contains(m_filteringRegExp) || dueDate.contains(m_filteringRegExp) || amount.contains(m_filteringRegExp)
        || currency.contains(m_filteringRegExp) || plnAmount.contains(m_filteringRegExp);
}

void InvoicesProxyModel::setFilteringPattern(const QString &pattern)
{
    m_filteringRegExp.setPattern(pattern);
    invalidateFilter();
}

bool InvoicesProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    const QVariant &leftData = sourceModel()->data(sourceLeft);
    const QVariant &rightData = sourceModel()->data(sourceRight);

    if (sourceLeft.column() == InvoicesModel::NumberColumn) {
        return leftData.toDouble() < rightData.toDouble();
    }

    if (sourceLeft.column() == InvoicesModel::TotalColumn || sourceLeft.column() == InvoicesModel::PlnTotalColumn) {
        double left = QLocale().toDouble(leftData.toString().split(" ")[1]);
        double right = QLocale().toDouble(rightData.toString().split(" ")[1]);
        return left < right;
    }

    if (leftData.userType() == QMetaType::Double) {
        return leftData.toDouble() < rightData.toDouble();
    }

    // In order to represent dates in d/M/yyyy format and use sorting, the following trick has to be applied.
    if (leftData.userType() == QMetaType::QString) {
        if (QDate::fromString(leftData.toString(), "d/M/yyyy").isValid()) {
            return QDate::fromString(leftData.toString(), "d/M/yyyy") < QDate::fromString(rightData.toString(), "d/M/yyyy");
        } else {
            return leftData.toString() < rightData.toString();
        }
    }

    return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
}
