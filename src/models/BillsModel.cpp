// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "BillsModel.h"
#include "LogicController.h"

BillsModel::BillsModel(LogicController *logicController, QObject *parent)
    : QAbstractTableModel(parent)
    , m_logicController(logicController)
{
    connect(this, &QAbstractTableModel::layoutChanged, this, [&]{
       qDebug() << "A";
    });
}

int BillsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_logicController->rangedBills().count();
}

int BillsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant BillsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < m_logicController->rangedBills().size()) {
        if (index.isValid() && role == Qt::DisplayRole) {
            const int row = index.row();
            // Since the application makes request for all financial history,
            // we need only those data between from date and to date.
            const auto &bill = m_logicController->rangedBills().at(row);
            switch (index.column()) {
            case NumberColumn:
                return bill.billNumber().isEmpty() ? "-" : bill.billNumber();
            case PartyColumn:
                return bill.party().isEmpty() ? "-" : bill.party();
            case StatusColumn:
                return bill.status().isEmpty() ? "-" : bill.status();
            case RecurrenceFrequency:
                return bill.recurrence_frequency().isEmpty() ? "-" : bill.recurrence_frequency();
            case DateColumn:
                return !bill.date().isValid() ? "-" : bill.date().toString("d/M/yyyy");
            case DueDateColumn:
                return !bill.dueDate().isValid() ? "-" : bill.dueDate().toString("d/M/yyyy");
            case NextBillDate:
                return !bill.nextBillDate().isValid() ? "-" : bill.nextBillDate().toString("d/M/yyyy");
            case TotalColumn:
                // Second parameter of toCurrencyString() method is intentionally " "
                // in order not to display currency symbol.
                return QLocale().toCurrencyString(bill.total(), " ") + " " + bill.currencyCode();
            case PlnTotalColumn:
                // Second parameter of toCurrencyString() method is intentionally " "
                // in order not to display currency symbol.
                return QLocale().toCurrencyString(bill.plnTotal(), " ") + " PLN";
            }
        } else if (index.isValid() && role == Qt::TextAlignmentRole) {
            switch (index.column()) {
            case TotalColumn:
                return {Qt::AlignRight | Qt::AlignVCenter};
            case PlnTotalColumn:
                return {Qt::AlignRight | Qt::AlignVCenter};
            }
            return Qt::AlignCenter;
        } else if (index.isValid() && role == Qt::CheckStateRole) {
            const int row = index.row();
            // Since the application makes a request for the whole financial history,
            // we need the data between 'from date' and 'to date' only.
            const auto &bill = m_logicController->rangedBills().at(row);
            if (index.column() == RecurrentColumn) {
                return bill.isRecurrent() ? Qt::Checked : Qt::Unchecked;
            }
        }
    }
    return {};
}

QVariant BillsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && -1 < section && section < ColumnCount) {
        switch (section) {
        case NumberColumn:
            return "Number";
        case PartyColumn:
            return "Party";
        case RecurrentColumn:
            return "Recurrent";
        case StatusColumn:
            return "Status";
        case RecurrenceFrequency:
            return "Recurrence frequency";
        case DateColumn:
            return "Date";
        case DueDateColumn:
            return "Due date";
        case NextBillDate:
            return "Next bill date";
        case TotalColumn:
            return "Amount";
        case PlnTotalColumn:
            return "PLN amount";
        }
    }

    return {};
}

QList<Bill> BillsModel::rangedBills() const
{
    return m_logicController->rangedBills();
}

void BillsModel::loadData()
{
    beginResetModel();
    endResetModel();
}

BillsProxyModel::BillsProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_filteringRegExp.setCaseSensitivity(Qt::CaseInsensitive);
}

bool BillsProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    const QVariant &leftData = sourceModel()->data(sourceLeft);
    const QVariant &rightData = sourceModel()->data(sourceRight);

    if (sourceLeft.column() == BillsModel::NumberColumn) {
        if (leftData.userType() == QMetaType::Double) {
            return leftData.toDouble() < rightData.toDouble();
        }
    }

    if (sourceLeft.column() == BillsModel::RecurrentColumn) {
        BillsModel *billsModel = static_cast<BillsModel*>(sourceModel());
        bool left = billsModel->rangedBills().at(sourceLeft.row()).isRecurrent();
        bool right = billsModel->rangedBills().at(sourceRight.row()).isRecurrent();
        return left < right;
    }

    if (sourceLeft.column() == BillsModel::TotalColumn || sourceLeft.column() == BillsModel::PlnTotalColumn) {
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

void BillsProxyModel::setFilteringPattern(const QString &pattern)
{
    m_filteringRegExp.setPattern(pattern);
    invalidateFilter();
}

void BillsProxyModel::setHideNormal(bool value)
{
    m_hideNormal = value;
    invalidateFilter();
}

void BillsProxyModel::setHideRecurring(bool value)
{
    m_hideRecurring = value;
    invalidateFilter();
}

bool BillsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    QString number = sourceModel()->index(sourceRow, 0).data().toString();
    QString party = sourceModel()->index(sourceRow, 1).data().toString();
    QString status = sourceModel()->index(sourceRow, 3).data().toString();
    QString requrrence_frequency = sourceModel()->index(sourceRow, 4).data().toString();
    QString date = sourceModel()->index(sourceRow, 5).data().toString();
    QString dueDate = sourceModel()->index(sourceRow, 6).data().toString();
    QString nextBillDate = sourceModel()->index(sourceRow, 7).data().toString();
    QString amount = sourceModel()->index(sourceRow, 8).data().toString();
    QString plnAmount = sourceModel()->index(sourceRow, 9).data().toString();

    // Casting is required because delegate is used for 'Recurrent' column,
    // so there's no way to get the data in other way.
    BillsModel *billsModel = static_cast<BillsModel*>(sourceModel());
    bool billIsRecurrent = billsModel->rangedBills().at(sourceRow).isRecurrent();

    if (billIsRecurrent && m_hideRecurring) {
        return false;
    } else if (!billIsRecurrent && m_hideNormal) {
        return false;
    }

    return number.contains(m_filteringRegExp)  || party.contains(m_filteringRegExp)||
           status.contains(m_filteringRegExp)  || date.contains(m_filteringRegExp) ||
           dueDate.contains(m_filteringRegExp) || nextBillDate.contains(m_filteringRegExp) ||
           amount.contains(m_filteringRegExp)  || plnAmount.contains(m_filteringRegExp) ||
           requrrence_frequency.contains(m_filteringRegExp);
}
