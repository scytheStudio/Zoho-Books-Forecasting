// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "ExpensesModel.h"
#include "LogicController.h"

ExpensesModel::ExpensesModel(LogicController *logicController, QObject *parent)
    : QAbstractTableModel(parent)
    , m_logicController(logicController)
{

}

int ExpensesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_logicController->rangedExpenses().size();
}

int ExpensesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ExpensesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < m_logicController->rangedExpenses().size()) {
        if (index.isValid() && role == Qt::DisplayRole) {
            const int row = index.row();
            // As long as the application makes request for all financial history,
            // we need only those data between from date and to date.
            const auto &expense = m_logicController->rangedExpenses().at(row);
            switch (index.column()) {
            case IdColumn:
                return expense.expenseId().isEmpty() ? "-" : expense.expenseId();
            case StatusColumn:
                return expense.status().isEmpty() ? "-" : expense.status();
            case CategoryColumn:
                return expense.category().isEmpty() ? "-" : expense.category();
            case PartyColumn:
                return expense.partyName().isEmpty() ? "-" : expense.partyName();
            case RecurrenceFrequency:
                return expense.recurrenceFrequency().isEmpty() ? "-" : expense.recurrenceFrequency();
            case DateColumn:
                return !expense.date().isValid() ? "-" : expense.date().toString("d/M/yyyy");
            case NextExpenseDateColumn:
                return !expense.nextExpenseDate().isValid() ? "-" : expense.nextExpenseDate().toString("d/M/yyyy");
            case TotalColumn:
                // Second parameter of toCurrencyString() method is intentionally " "
                // in order not to display currency symbol.
                return QLocale().toCurrencyString(expense.total(), " ") + " " + expense.currencyCode();
            case PlnTotalColumn:
                // Second parameter of toCurrencyString() method is intentionally " "
                // in order not to display currency symbol.
                return QLocale().toCurrencyString(expense.plnTotal(), " ") + " PLN";
            }
        } else if (index.isValid() && role == Qt::CheckStateRole) {
            if (index.column() == RecurrentColumn) {
                const int row = index.row();
                // Since the application makes a request for the whole financial history,
                // we need the data between 'from date' and 'to date' only.
                const auto &expense = m_logicController->rangedExpenses().at(row);
                return expense.isRecurrent() ? Qt::Checked : Qt::Unchecked;
            }
        } else if (index.isValid() && role == Qt::TextAlignmentRole) {
            switch (index.column()) {
            case IdColumn:
                return Qt::AlignCenter;
            case TotalColumn:
                return {Qt::AlignRight | Qt::AlignVCenter};
            case PlnTotalColumn:
                return {Qt::AlignRight | Qt::AlignVCenter};
            case PartyColumn:
                return Qt::AlignCenter;
            case RecurrenceFrequency:
                return Qt::AlignCenter;
            case NextExpenseDateColumn:
                return Qt::AlignCenter;
            case DateColumn:
                return Qt::AlignCenter;
            case CategoryColumn:
                return Qt::AlignCenter;
            case StatusColumn:
                return Qt::AlignCenter;
            return Qt::AlignCenter;
            }
        }
    }
    return {};
}

QVariant ExpensesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && -1 < section && section < ColumnCount) {
        switch (section) {
        case IdColumn:
            return "Id";
        case StatusColumn:
            return "Status";
        case CategoryColumn:
            return "Category";
        case PartyColumn:
            return "Party";
        case RecurrentColumn:
            return "Recurrent";
        case RecurrenceFrequency:
            return "Recurrence Frequency";
        case DateColumn:
            return "Date";
        case NextExpenseDateColumn:
            return "Next expense date";
        case TotalColumn:
            return "Amount";
        case PlnTotalColumn:
            return "PLN amount";
        }
    }
    return {};
}

QList<Expense> ExpensesModel::rangedExpenses() const
{
    return m_logicController->rangedExpenses();
}

void ExpensesModel::loadData()
{
    beginResetModel();
    endResetModel();
}

ExpensesProxyModel::ExpensesProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_filteringRegExp.setCaseSensitivity(Qt::CaseInsensitive);
}

bool ExpensesProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    QString id = sourceModel()->index(sourceRow, 0).data().toString();
    QString status = sourceModel()->index(sourceRow, 1).data().toString();
    QString category = sourceModel()->index(sourceRow, 2).data().toString();
    QString party = sourceModel()->index(sourceRow, 3).data().toString();
    QString recurrenceFrequency = sourceModel()->index(sourceRow, 5).data().toString();
    QString date = sourceModel()->index(sourceRow, 6).data().toString();
    QString nextExpenseDate = sourceModel()->index(sourceRow, 7).data().toString();
    QString amount = sourceModel()->index(sourceRow, 8).data().toString();
    QString currency = sourceModel()->index(sourceRow, 9).data().toString();
    QString plnAmount = sourceModel()->index(sourceRow, 10).data().toString();

    // Casting is required because delegate is used for 'Recurrent' column,
    // so there's no way to get the data in other way.
    ExpensesModel *expensesModel = static_cast<ExpensesModel*>(sourceModel());
    bool expenseIsRecurrent = expensesModel->rangedExpenses().at(sourceRow).isRecurrent();

    if (expenseIsRecurrent && m_hideRecurring) {
        return false;
    } else if (!expenseIsRecurrent && m_hideNormal) {
        return false;
    }

    return id.contains(m_filteringRegExp)                  || status.contains(m_filteringRegExp) ||
           category.contains(m_filteringRegExp)            || party.contains(m_filteringRegExp) ||
           recurrenceFrequency.contains(m_filteringRegExp) || date.contains(m_filteringRegExp) ||
           nextExpenseDate.contains(m_filteringRegExp)     || amount.contains(m_filteringRegExp) ||
           currency.contains(m_filteringRegExp)            || plnAmount.contains(m_filteringRegExp);
}

void ExpensesProxyModel::setFilteringPattern(const QString &pattern)
{
   m_filteringRegExp.setPattern(pattern);
   invalidateFilter();
}

void ExpensesProxyModel::setHideNormal(bool value)
{
    m_hideNormal = value;
    invalidateFilter();
}

void ExpensesProxyModel::setHideRecurring(bool value)
{
    m_hideRecurring = value;
    invalidateFilter();
}

bool ExpensesProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    const QVariant &leftData = sourceModel()->data(sourceLeft);
    const QVariant &rightData = sourceModel()->data(sourceRight);

    if (sourceLeft.column() == ExpensesModel::IdColumn) {
        if (leftData.userType() == QMetaType::Double) {
            return leftData.toDouble() < rightData.toDouble();
        }
    }

    if (sourceLeft.column() == ExpensesModel::RecurrentColumn) {
        ExpensesModel *expensesModel = static_cast<ExpensesModel*>(sourceModel());
        bool left = expensesModel->rangedExpenses().at(sourceLeft.row()).isRecurrent();
        bool right = expensesModel->rangedExpenses().at(sourceRight.row()).isRecurrent();
        return left < right;
    }

    if (sourceLeft.column() == ExpensesModel::TotalColumn || sourceLeft.column() == ExpensesModel::PlnTotalColumn) {
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
