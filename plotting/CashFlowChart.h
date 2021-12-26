// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef CASHFLOWCHART_H
#define CASHFLOWCHART_H

#include <QDebug>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QGesture>
#include "datasets/Invoice.h"
#include "datasets/Bill.h"
#include "datasets/Expense.h"
#include "models/ForecastingModel.h"

QT_CHARTS_USE_NAMESPACE

/*!
 * \brief Class representing a chart all the data is drawn on.
 */
class CashFlowChart : public QChart
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     * \param LogicController *logicController -- logic controller responsible for manipulating different parts of the application.
     * \param QGraphicsItem *parent -- parent.
     * \param Qt::WindowFlags wFlags -- window flags.
     */
    explicit CashFlowChart(LogicController *logicController, QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());

    /*!
     * \brief Prepares income series and makes it ready to be displayed.
     * \param const QList<Invoice> &invoices -- list of invoices.
     * \param const QList<ForecastingModel::Forecast> &forecasts list of forecasts.
     */
    void prepareIncomeSeries(const QList<Invoice> &invoices, const QList<ForecastingModel::Forecast> &forecasts);

    /*!
     * \brief Prepares income series and makes it ready to be displayed.
     * \param const QList<Expense> &expenses -- list of expenses.
     * \param const QList<Bill> &bills -- list of bills.
     * \param const QList<ForecastingModel::Forecast> &forecasts list of forecasts.
     */
    void prepareExpensesSeries(const QList<Expense> &expenses, const QList<Bill> &bills, const QList<ForecastingModel::Forecast> &forecasts);

    /*!
     * \brief Sets visibility of the income series.
     * \param bool state -- value to set.
     */
    void setIncomeSeriesVisible(bool state);

    /*!
     * \brief Sets visibility of the expenses series.
     * \param bool state -- value to set.
     */
    void setExpensesSeriesVisible(bool state);

    /*!
     * \brief Sets visibility of the cashflow series.
     * \param bool state -- value to set.
     */
    void setCashFlowSeriesVisisble(bool state);

    /*!
     * \brief Sets visibility of the points of the income series.
     * \param bool state -- value to set.
     */
    void setIncomePointsVisible(bool state);

    /*!
     * \brief Sets visibility of the points of the expenses series.
     * \param bool state -- value to set.
     */
    void setExpensesPointsVisible(bool state);

    /*!
     * \brief Sets visibility of the points of the cashflow series.
     * \param bool state -- value to set.
     */
    void setCashFlowPointsVisible(bool state);

    /*!
     * \brief Sets bounds of the part of the chart to display.
     * \param const QDate &fromDate -- value of 'from limit' of the displayed chart.
     * \param const QDate &toDate -- value of 'to limit' of the displayed chart.
     */
    void setDates(const QDate &fromDate, const QDate &toDate);

    /*!
     * \brief Resets bounds of Y axe.
     */
    void resetYAxeRanges();

    /*!
     * \brief Sets visibility of all series.
     */
    void setSeriesVisible(bool value);

signals:

    /*!
     * \brief This signal is emitted when any of the series are hovered.
     * \param const QPointF &point -- point where the series is hovered.
     * \param bool state -- state.
     */
    void seriesHovered(const QPointF &point, bool state);

    /*!
     * \brief This signal is emitted after expenses series has been drawn.
     */
    void expensesSeriesDrawn();

    /*!
     * \brief This signal is emitted after income series has been drawn.
     */
    void incomesSeriesDrawn();

    /*!
     * \brief This signal is emitted after cashflow series has been drawn.
     */
    void cashFlowSeriesDrawn();

    /*!
     * \brief This signal is emitted after axes have been prepared.
     */
    void axesPrepared();

private:
    QLineSeries *m_expensesSeries = nullptr;
    QLineSeries *m_incomeSeries = nullptr;
    QLineSeries *m_cashFlowSeries = nullptr;

    QDateTimeAxis *m_xTimeAxis = nullptr;
    QValueAxis *m_yValueAxis = nullptr;

    struct DateAmount {
        QDate date;
        double amount;
        bool isIncome;
        bool isRecurrent;
    };

    QList<DateAmount> m_dateAmounts;

    struct Period {
        QDate startDate;
        QDate endDate;

        double balance =  0.0;
        double incomeSum = 0.0;
        double expensesSum = 0.0;
        double cashFlow = 0.0;
    };

    QList<Period> m_periods;

    bool m_incomesDrawn = false;
    bool m_expensesDrawn = false;

    QDate m_fromDate;
    QDate m_toDate;

    double m_maxValue = 0.0;
    double m_minValue = 0.0;

    LogicController* m_logicController = nullptr;

    void prepareCashFlowSeries();
    void prepareAxis();

    void setup();
    void seriesDrawn();
    void cashFlowDrawn();
    void setupPeriods();
};

#endif // CASHFLOWCHART_H
