// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef FORECASTINGMODEL_H
#define FORECASTINGMODEL_H

#include <QSortFilterProxyModel>
#include <QAbstractTableModel>
#include <QDate>
#include <QDebug>

class LogicController;

/*!
 * \brief Class representing a forecasting model.
 */
class ForecastingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor.
     * \param LogicController *logicController -- logic controller responsible for manipulating different parts of the application.
     * \param QObject *parent -- parent.
     */
    explicit ForecastingModel(LogicController *logicController, QObject *parent = nullptr); //!<

    /*!
     * \brief Enum representing a column of the table displaying forecasts.
     */
    enum Column {
        Name,
        Price,
        Date,
        IsIncome,
        IsRecurrent,
        ColumnCount
    };

    /*!
     * \brief Class representing a forecast entity.
     */
    struct Forecast {
        QString name;
        double price;
        QDate date;
        bool isIncome;
        bool isRecurrent;

        /*!
         * \brief Comparison operator for Forecast.
         * \param const Forecast &forecastEntry -- forecast entry to make comparison with.
         */
        bool operator==(const Forecast &forecastEntry)
        {
            return name == forecastEntry.name
                && price == forecastEntry.price
                && date == forecastEntry.date
                && isIncome == forecastEntry.isIncome
                && isRecurrent == forecastEntry.isRecurrent;
        }
    };

    /*!
     * \brief Returns number of rows of the table the model is used for.
     * \param const QModelIndex &parent -- model index to use for getting row count.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Returns number of columns of the table the model is used for.
     * \param const QModelIndex &parent -- model index to use for getting column count.
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Returns data of the table at the given index and at the given role.
     * \param const QModelIndex &index -- index of a cell in the forecasts table
     * \param int role -- role of a cell in the forecasts table.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /*!
     * \brief Returns column name of the specified section.
     * \param int section -- header section.
     * \param Qt::Orientation orientation -- orientation.
     * \param int role -- role of a cell in the forecasts table.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /*!
     * \brief Returns flags of the specified index of the table.
     * \param const QModelIndex &index -- index of a cell in the forecasts table.
     */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /*!
     * \brief Sets data of the specified table index with value.
     * \param const QModelIndex &index -- index of a cell in the forecasts table.
     * \param const QVariant &value - value to set.
     * \param int role -- role of a cell in the forecasts table.
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*!
     * \brief Provides sorting functionality.
     * \param int column -- column in the forecasts table.
     * \param Qt::SortOrder order -- sorting order.
     */
    void sort(int column, Qt::SortOrder order) override;

    /*!
     * \brief Adds an entry to the list of forecasts.
     * \param const Forecast &forecastEntry -- forecast to add.
     */
    void addEntry(const Forecast &forecastEntry);

    /*!
     * \brief Removes an entry from the list of forecast.
     * \param const Forecast &forecastEntry -- forecast to remove.
     */
    void removeEntry(const Forecast &forecastEntry);

    /*!
     * \brief Removes an entry with a specified row.
     * \param int row -- row the forecast is in the table to remove.
     */
    void removeEntry(int row);

    /*!
     * \brief Clears list of forecasts.
     */
    void clearEntries();

    /*!
     * \brief Resets the model.
     */
    void loadData();

signals:

    /*!
     * \brief This signal is emitted after some entry has been changed.
     */
    void rowChanged();

    /*!
     * \brief This signal is emitted after some new entry has been added to the table.
     */
    void rowInserted();

    /*!
     * \brief This signal is emitted after some entry has been removed from the table.
     */
    void rowsRemoved();

    /*!
     * \brief This signal is emitted after any change has been perforemed on the model.
     */
    void modelChanged();

private:
    LogicController *m_logicController = nullptr;
};

#endif // FORECASTINGMODEL_H
