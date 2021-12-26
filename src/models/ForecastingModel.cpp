// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include <QDate>
#include "ForecastingModel.h"
#include "LogicController.h"

ForecastingModel::ForecastingModel(LogicController *logicController, QObject *parent)
    : QAbstractTableModel(parent)
    , m_logicController(logicController)
{

}

int ForecastingModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_logicController->forecasts().size();
}

int ForecastingModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant ForecastingModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < m_logicController->forecasts().size()) {
        if (role == Qt::DisplayRole && index.isValid()) {
            const int row = index.row();
            const auto &forecast = m_logicController->forecasts().at(row);
            switch(index.column()) {
            case Name:
                return forecast.name.isEmpty() ? "-" : forecast.name;
            case Price:
                return forecast.price;
            case Date:
                return forecast.date.isValid() ? forecast.date.toString("d/M/yyyy") : "-";
            }
        } else if (index.isValid() && role == Qt::TextAlignmentRole) {
            switch (index.column()) {
            case Price:
                return {Qt::AlignRight | Qt::AlignVCenter};
            }
            return Qt::AlignCenter;
        } else if (role == Qt::CheckStateRole && index.isValid()) { // for cells with checkboxes.
            const int row = index.row();
            const auto &forecast = m_logicController->forecasts().at(row);
            switch(index.column()) {
            case Column::IsIncome:
                return forecast.isIncome ? Qt::Checked : Qt::Unchecked;
            case Column::IsRecurrent:
                return forecast.isRecurrent ? Qt::Checked : Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

QVariant ForecastingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && -1 < section && section < ColumnCount) {
        switch(section) {
        case Name:
            return "Name";
        case Price:;
            return "Price";
        case Date:
            return "Date";
        case IsIncome:
            return "Income";
        case IsRecurrent:
            return "Recurring";
        }
    }
    return {};
}

Qt::ItemFlags ForecastingModel::flags(const QModelIndex &index) const
{
    if (index.column() == Column::IsIncome || index.column() == Column::IsRecurrent) {
        return Qt::ItemIsUserCheckable | QAbstractTableModel::flags(index);
    }

    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool ForecastingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        auto& forecast = m_logicController->forecasts()[index.row()];
        switch (index.column()) {
        case Name:
            forecast.name = value.toString();
            break;
        case Price:
            forecast.price = value.toDouble();
            break;
        case Date:
            auto &forecast = m_logicController->forecasts()[index.row()];
            forecast.date = value.toDate();
            break;
        }
        emit dataChanged(index, index, {role});
        emit modelChanged();
        return true;
    } else if (index.isValid() && role == Qt::CheckStateRole) {
        auto& forecast = m_logicController->forecasts()[index.row()];
        switch(index.column()) {
        case IsIncome:
            forecast.isIncome = value.toBool();
            break;
        case IsRecurrent:
            forecast.isRecurrent = value.toBool();
            break;
        }
        emit dataChanged(index, index, {role});
        emit modelChanged();
        return true;
    }
    return false;
}

void ForecastingModel::addEntry(const Forecast &forecastEntry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_logicController->forecasts().append(forecastEntry);
    endInsertRows();
    emit modelChanged();
}

void ForecastingModel::removeEntry(const Forecast &forecastEntry)
{
    beginRemoveRows(QModelIndex(), m_logicController->forecasts().indexOf(forecastEntry), m_logicController->forecasts().indexOf(forecastEntry));
    m_logicController->forecasts().removeOne(forecastEntry);
    endRemoveRows();
    emit modelChanged();
}

void ForecastingModel::removeEntry(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_logicController->forecasts().removeAt(row);
    endRemoveRows();
    emit modelChanged();
}

void ForecastingModel::clearEntries()
{
    beginResetModel();
    m_logicController->forecasts().clear();
    endResetModel();
    emit modelChanged();
}

void ForecastingModel::sort(int column, Qt::SortOrder order)
{
    switch (column) {
    case Name:
        if (order == Qt::AscendingOrder) {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.name < b.name; });
        } else {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.name > b.name; });
        }
        break;

    case Price:
        if (order == Qt::AscendingOrder) {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.price < b.price; });
        } else {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.price > b.price; });
        }
        break;

    case Date:
        if (order == Qt::AscendingOrder) {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.date < b.date; });
        } else {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.date > b.date; });
        }
        break;

    case IsIncome:
        if (order == Qt::AscendingOrder) {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.isIncome < b.isIncome; });
        } else {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.isIncome > b.isIncome; });
        }
        break;

    case IsRecurrent:
        if (order == Qt::AscendingOrder) {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.isIncome < b.isIncome; });
        } else {
            std::sort(m_logicController->forecasts().begin(), m_logicController->forecasts().end(),
                      [](const Forecast &a, const Forecast &b) { return a.isRecurrent > b.isRecurrent; });
        }
        break;
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}

void ForecastingModel::loadData()
{
    beginResetModel();
    endResetModel();
}
