// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "CashFlowChart.h"
#include "LogicController.h"
#include <QLineSeries>
#include <QtCore>

#define MAX_NUMBER_OF_FUTURE_EVENTS 400

CashFlowChart::CashFlowChart(LogicController *logicalController, QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(parent, wFlags)
    , m_logicController(logicalController)
{
    connect(this, &CashFlowChart::expensesSeriesDrawn, this, &CashFlowChart::seriesDrawn);
    connect(this, &CashFlowChart::incomesSeriesDrawn, this, &CashFlowChart::seriesDrawn);
    connect(this, &CashFlowChart::cashFlowSeriesDrawn, this, &CashFlowChart::cashFlowDrawn);
    connect(this, &CashFlowChart::axesPrepared, this, [&](){
        setSeriesVisible(true);
    });
    setup();
}

void CashFlowChart::prepareAxis()
{
    if (m_xTimeAxis) {
        delete m_xTimeAxis;
    }

    if (m_yValueAxis) {
        delete m_yValueAxis;
    }

    m_xTimeAxis = new QDateTimeAxis(this);
    m_xTimeAxis->setRange(m_fromDate.startOfDay(), m_toDate.startOfDay());
    m_xTimeAxis->setFormat("d/M/yyyy");
    addAxis(m_xTimeAxis, Qt::AlignBottom);
    m_cashFlowSeries->attachAxis(m_xTimeAxis);
    m_expensesSeries->attachAxis(m_xTimeAxis);
    m_incomeSeries->attachAxis(m_xTimeAxis);

    m_yValueAxis = new QValueAxis(this);
    addAxis(m_yValueAxis, Qt::AlignLeft);
    m_cashFlowSeries->attachAxis(m_yValueAxis);
    m_expensesSeries->attachAxis(m_yValueAxis);
    m_incomeSeries->attachAxis(m_yValueAxis);
    m_yValueAxis->setRange(m_minValue, m_maxValue);
    m_yValueAxis->setTickCount(5);
    m_yValueAxis->applyNiceNumbers();
    emit axesPrepared();
}

void CashFlowChart::setup()
{
    setTitle("Cash flow");
    setAcceptHoverEvents(true);
}

void CashFlowChart::prepareIncomeSeries(const QList<Invoice> &invoices,
                                        const QList<ForecastingModel::Forecast> &forecasts)
{
    qDebug() << "Income series";
    if (m_incomeSeries) {
        delete m_incomeSeries;
    }

    m_incomeSeries = new QLineSeries(this);
    m_incomeSeries->setName("Income");
    m_incomeSeries->setPen(QPen(QColor(95, 25, 186), 2.5));

    connect(m_incomeSeries, &QLineSeries::hovered, this, &CashFlowChart::seriesHovered);

    /*
     * The idea is to collect all unique dates of the incomes. If some income has the same date as another, no unique points are added.
     * Only the general value of amount on this date is increased.
     */

    QVector<DateAmount> invoicesDateAmounts; //temporary storage of invoices.
    for (const auto &invoice: invoices) {
        QDate date = invoice.dueDate().isValid() ? invoice.dueDate() : invoice.date();
        invoicesDateAmounts.append(DateAmount {date, invoice.plnTotal(), true, false});
    }

    QDate limit = m_logicController->lastDate() > m_toDate ? m_logicController->lastDate() : m_toDate;
    QVector<DateAmount> forecastsDateAmounts; //temporary storage of forecasts.
    if (m_logicController->isForecastingEnabled()) {
        for (const auto &forecast : forecasts) {
            if (forecast.date.isValid() && m_logicController->firstDate() <= forecast.date) {
                if (forecast.isIncome) {
                    if (forecast.isRecurrent) {
                        // recurrent incomes follow the same pattern as usual once, but only added every month till the last data.
                        for (int i = 0; ; ++i) {
                            if (forecast.date.addMonths(i) > limit) {
                                forecastsDateAmounts.append(DateAmount {forecast.date.addMonths(i), forecast.price, forecast.isIncome, forecast.isRecurrent});
                                break;
                            }
                            forecastsDateAmounts.append(DateAmount {forecast.date.addMonths(i), forecast.price, forecast.isIncome, forecast.isRecurrent});
                        }
                    } else {
                        forecastsDateAmounts.append(DateAmount {forecast.date, forecast.price, forecast.isIncome, forecast.isRecurrent});
                    }
                }
            }
        }
    }

    // if main storage of invoices has an entry on this date, increase amount on this date. Otherwise, add new point.

    for (DateAmount &invoiceDateAmount : invoicesDateAmounts) {
        bool found = false;
        for (DateAmount &dateAmount : m_dateAmounts) {
            if (invoiceDateAmount.date == dateAmount.date && invoiceDateAmount.isIncome && dateAmount.isIncome) {
                dateAmount.amount += invoiceDateAmount.amount;
                found = true;
                break;
            }
        }
        if (!found) {
            m_dateAmounts.append(invoiceDateAmount);
        }
    }

    for (DateAmount &forecastDateAmount : forecastsDateAmounts) {
        bool found = false;
        for (DateAmount &dateAmount : m_dateAmounts) {
            if (forecastDateAmount.date == dateAmount.date && forecastDateAmount.isIncome && dateAmount.isIncome) {
                dateAmount.amount += forecastDateAmount.amount;
                found = true;
                break;
            }
        }
        if (!found) {
            m_dateAmounts.append(forecastDateAmount);
        }
    }

    std::sort(m_dateAmounts.begin(), m_dateAmounts.end(), [](const DateAmount &a, const DateAmount &b) {
        return a.date < b.date;
    });

    QDate dateAfterLast = m_logicController->firstDate();

    for (DateAmount &dateAmount : m_dateAmounts) {
        if (dateAmount.isIncome) {
            m_incomeSeries->append(dateAmount.date.startOfDay().toMSecsSinceEpoch(), dateAmount.amount);
            if (m_fromDate <= dateAmount.date && dateAmount.date <= m_toDate) {
                if (dateAmount.amount > m_maxValue) {
                   m_maxValue = dateAmount.amount;
                } else if (dateAmount.amount < m_minValue) {
                   m_minValue = dateAmount.amount;
                }
            }

            if (dateAmount.date > dateAfterLast) {
                dateAfterLast = dateAmount.date;
            }
        }
    }

    if (m_logicController->isForecastingEnabled()) {
        m_incomeSeries->append(dateAfterLast.startOfDay().toMSecsSinceEpoch(), 0);
        m_incomeSeries->append(limit.startOfDay().toMSecsSinceEpoch(), 0);
    }

    addSeries(m_incomeSeries);
    m_incomeSeries->setVisible(false);

    m_incomesDrawn = true;
    emit incomesSeriesDrawn();
    qDebug() << "end income series";
}

void CashFlowChart::prepareExpensesSeries(const QList<Expense> &expenses,
                                          const QList<Bill> &bills,
                                          const QList<ForecastingModel::Forecast> &forecasts)
{
    qDebug() << "Expenses series";
    if (m_expensesSeries) {
        delete m_expensesSeries;
    }

    m_expensesSeries = new QLineSeries();
    m_expensesSeries->setName("Expenses");
    m_expensesSeries->setPen(QPen(QColor(255, 165, 0), 2.5));

    connect(m_expensesSeries, &QLineSeries::hovered, this, &CashFlowChart::seriesHovered);

    // The idea is to collect all unique dates of the expenses.
    // If some expense has the same date as another, no unique points are added.
    // Only the general value of amount on this date is increased.

    QDate limit = m_logicController->lastDate() > m_toDate ? m_logicController->lastDate() : m_toDate;
    QVector<DateAmount> expensesDateAmounts; // temporaray storage of expenses.
    for (const auto &expense : expenses) {
        if (expense.isRecurrent() && m_logicController->isForecastingEnabled()) {
            for (int i = 0; ; ++i) {
                DateAmount dateAmount {expense.nextExpenseDate(), expense.plnTotal(), false, expense.isRecurrent()};
                if (expense.recurrenceFrequency() == "weeks") { // might some new periods will appear (i.e. days, years etc.)
                    dateAmount.date = dateAmount.date.addDays(7 * i);
                } else if (expense.recurrenceFrequency() == "months") {
                    dateAmount.date = dateAmount.date.addMonths(i);
                }

                if (dateAmount.date >= limit || i >= MAX_NUMBER_OF_FUTURE_EVENTS) {
                    break;
                }
                expensesDateAmounts.append(dateAmount);
            }
        } else {
            expensesDateAmounts.append(DateAmount {expense.date(), expense.plnTotal(), false, expense.isRecurrent()});
        }
    }

    QVector<DateAmount> billsDateAmounts; // temporaray storage of bills.
    for (const auto &bill : bills) {
        if (bill.isRecurrent() && m_logicController->isForecastingEnabled()) {
            for (int i = 0; ; ++i) {
                DateAmount dateAmount {bill.nextBillDate(), bill.total(), false, bill.isRecurrent()};
                if (bill.recurrence_frequency() == "weeks") { // might some new periods will appear (i.e. days, years etc.)
                    dateAmount.date = dateAmount.date.addDays(7 * i);
                } else if (bill.recurrence_frequency() == "months") {
                    dateAmount.date = dateAmount.date.addMonths(i);
                }

                if (dateAmount.date > limit) {
                    break;
                }
                billsDateAmounts.append(dateAmount);
            }
        } else {
            billsDateAmounts.append(DateAmount {bill.dueDate().isValid() ? bill.dueDate() : bill.date(), bill.plnTotal(), false, bill.isRecurrent()});
        }
    }

    QVector<DateAmount> forecastsDateAmounts; // temporaray storage of forecasts.
    if (m_logicController->isForecastingEnabled()) {
        for (const auto &forecast : forecasts) {
            if (forecast.date.isValid() && m_logicController->firstDate() <= forecast.date) {
                if (!forecast.isIncome) {
                    if (forecast.isRecurrent) {
                        // recurrent incomes follow the same pattern as usual once, but only added every month till the last data.
                        for (int i = 0; ; ++i) {
                            if (forecast.date.addMonths(i) > limit) {
                                forecastsDateAmounts.append(DateAmount{forecast.date.addMonths(i), forecast.price, forecast.isIncome, forecast.isRecurrent});
                                break;
                            }
                            forecastsDateAmounts.append(DateAmount{forecast.date.addMonths(i), forecast.price, forecast.isIncome, forecast.isRecurrent});
                        }
                    } else {
                        forecastsDateAmounts.append(DateAmount {forecast.date, forecast.price, forecast.isIncome, forecast.isRecurrent});
                    }
                }
            }
        }
    }

    // if main storage of expenses and bills has an entry on this date, increase amount on this date. Otherwise, add new point.

    for (DateAmount &expenseDateAmount: expensesDateAmounts) {
        bool found = false;
        for (DateAmount &dateAmount : m_dateAmounts) {
            if (expenseDateAmount.date == dateAmount.date && !expenseDateAmount.isIncome && !dateAmount.isIncome) {
                dateAmount.amount += expenseDateAmount.amount;
                found = true;
                break;
            }
        }
        if (!found) {
            m_dateAmounts.append(expenseDateAmount);
        }
    }

    for (DateAmount &billDateAmount: billsDateAmounts) {
        bool found = false;
        for (DateAmount &dateAmount : m_dateAmounts) {
            if (billDateAmount.date == dateAmount.date && !billDateAmount.isIncome && !dateAmount.isIncome) {
                dateAmount.amount += billDateAmount.amount;
                found = true;
                break;
            }
        }
        if (!found) {
            m_dateAmounts.append(billDateAmount);
        }
    }

    std::sort(m_dateAmounts.begin(), m_dateAmounts.end(), [](const DateAmount &a, const DateAmount &b) {
        return a.date < b.date;
    });

    for (DateAmount &forecastDateAmount: forecastsDateAmounts) {
        bool found = false;
        for (DateAmount &dateAmount : m_dateAmounts) {
            if (forecastDateAmount.date == dateAmount.date && !forecastDateAmount.isIncome && !dateAmount.isIncome) {
                dateAmount.amount += forecastDateAmount.amount;
                found = true;
                break;
            }
        }
        if (!found) {
            m_dateAmounts.append(forecastDateAmount);
        }
    }

    std::sort(m_dateAmounts.begin(), m_dateAmounts.end(), [](const DateAmount &a, const DateAmount &b) {
        return a.date < b.date;
    });

    QDate dateAfterLast = m_logicController->firstDate();

    for (DateAmount &dateAmount : m_dateAmounts) {
        if (!dateAmount.isIncome) {
            m_expensesSeries->append(dateAmount.date.startOfDay().toMSecsSinceEpoch(), dateAmount.amount);
            // calculating bounds of Y axe.
            if (m_fromDate <= dateAmount.date && dateAmount.date <= m_toDate) {
                if (dateAmount.amount > m_maxValue) {
                   m_maxValue = dateAmount.amount;
                } else if (dateAmount.amount < m_minValue) {
                   m_minValue = dateAmount.amount;
                }
            }

            if (dateAmount.date > dateAfterLast) {
                dateAfterLast = dateAmount.date;
            }
        }
    }

    if (m_logicController->isForecastingEnabled()) {
        m_expensesSeries->append(dateAfterLast.startOfDay().toMSecsSinceEpoch(), 0);
        m_expensesSeries->append(limit.startOfDay().toMSecsSinceEpoch(), 0);
    }

    addSeries(m_expensesSeries);
    m_expensesSeries->setVisible(false);

    m_expensesDrawn = true;
    emit expensesSeriesDrawn();
    qDebug() << "end Expenses series";
}

void CashFlowChart::seriesDrawn()
{
    if (m_expensesDrawn && m_incomesDrawn) {
        m_expensesDrawn = false;
        m_incomesDrawn = false;
        prepareCashFlowSeries();
    }
}

void CashFlowChart::prepareCashFlowSeries()
{
    qDebug() << "Cashflow series";
    setupPeriods();
    if (m_cashFlowSeries) {
        delete m_cashFlowSeries;
    }

    m_cashFlowSeries = new QLineSeries(this);
    m_cashFlowSeries->setName("CashFlow");
    m_cashFlowSeries->setPen(QPen(QColor(0, 240, 112), 3));

    connect(m_cashFlowSeries, &QLineSeries::hovered, this, &CashFlowChart::seriesHovered);

    // setting balance and cashflow for all periods. First period has balance equal to cashflow as there's no previous operations.
    int i = 0;
    for (auto pItr = m_periods.begin(); pItr != m_periods.end(); ++pItr) {
        for (auto dItr = m_dateAmounts.begin(); dItr != m_dateAmounts.end(); ++dItr) {
            if (pItr->startDate <= dItr->date && dItr->date <= pItr->endDate) {
                if (dItr->isIncome) {
                    pItr->balance += dItr->amount;
                } else {
                    pItr->balance -= dItr->amount;
                }
            }
        }

        if (i == 0) {
            pItr->cashFlow = pItr->balance;
            ++i;
        } else {
            pItr->cashFlow = (pItr-1)->cashFlow + pItr->balance;
        }
    }

    for (auto it = m_periods.begin(); it != m_periods.end(); ++it) {
        // calculating bounds of Y axe.
        if (m_fromDate <= it->startDate && it->endDate <= m_toDate) {
            if (it->balance > m_maxValue) {
                m_maxValue = it->balance;
            } else if (it->balance < m_minValue) {
                m_minValue = it->balance;
            }
        }
    }

    for (auto itr = m_periods.begin(); itr != m_periods.end(); ++itr) {
        m_cashFlowSeries->append(itr->startDate.startOfDay().addDays(14).toMSecsSinceEpoch(), itr->cashFlow);
        // calculating bounds of Y axe.
        if (m_fromDate <= itr->startDate && itr->endDate <= m_toDate) {
            if (itr->cashFlow > m_maxValue) {
                m_maxValue = itr->cashFlow;
            } else if (itr->cashFlow < m_minValue) {
                m_minValue = itr->cashFlow;
            }
        }
    }

    addSeries(m_cashFlowSeries);
    m_cashFlowSeries->setVisible(false);
    m_dateAmounts.clear();
    emit cashFlowSeriesDrawn();
}

void CashFlowChart::cashFlowDrawn()
{
    prepareAxis();
}

void CashFlowChart::setIncomeSeriesVisible(bool state) {
    if (m_incomeSeries) {
        m_incomeSeries->setVisible(state);
    }
}

void CashFlowChart::setExpensesSeriesVisible(bool state) {
    if (m_expensesSeries) {
        m_expensesSeries->setVisible(state);
    }
}

void CashFlowChart::setCashFlowSeriesVisisble(bool state) {
    if (m_cashFlowSeries) {
        m_cashFlowSeries->setVisible(state);
    }
}

void CashFlowChart::setIncomePointsVisible(bool state) {
    if (m_incomeSeries) {
        m_incomeSeries->setPointsVisible(state);
    }
}

void CashFlowChart::setExpensesPointsVisible(bool state) {
    if (m_expensesSeries) {
        m_expensesSeries->setPointsVisible(state);
    }
}

void CashFlowChart::setCashFlowPointsVisible(bool state) {
    if (m_cashFlowSeries) {
        m_cashFlowSeries->setPointsVisible(state);
    }
}

void CashFlowChart::setupPeriods() {
    // creating periods from first date to last date of records. Each period is one month.
    m_periods.clear();
    QDate start = m_logicController->firstDate() < m_fromDate ? m_logicController->firstDate() : m_fromDate;
    QDate limit = m_logicController->lastDate() > m_toDate ? m_logicController->lastDate() : m_toDate;
    QDate date(start.year(), start.month(), 1);
    for (int i = 0; ; ++i) {
        Period period;
        period.startDate = date;
        period.endDate = date.addMonths(1).addDays(-1);
        if (limit <= period.startDate || period.endDate >= limit) {
            m_periods.append(period);
            break;
        }
        m_periods.append(period);
        date = period.endDate.addDays(1);
    }
}

void CashFlowChart::setDates(const QDate &fromDate, const QDate &toDate) {
    m_fromDate = fromDate;
    m_toDate = toDate;
}

void CashFlowChart::resetYAxeRanges() {
    m_minValue = 0.0;
    m_maxValue = 0.0;
}

void CashFlowChart::setSeriesVisible(bool state)
{
    if (m_expensesSeries) {
        m_expensesSeries->setVisible(state);
    }

    if (m_incomeSeries) {
        m_incomeSeries->setVisible(state);
    }

    if (m_cashFlowSeries) {
        m_cashFlowSeries->setVisible(state);
    }
}
