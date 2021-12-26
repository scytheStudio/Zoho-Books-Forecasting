// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include "Settings.h"
#include "WebClient.h"
#include "models/ForecastingModel.h"
#include <QObject>
#include <QApplication>

/*!
 * \brief Class representing a logic controller responsible for all the manipulations between components of the application.
 */
class LogicController : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     * \param QObject *parent -- parent.
     */
    explicit LogicController(QObject *parent = nullptr);

    /*!
     * \brief Makes a request to Zoho API to get all the data.
     */
    void requestAllData();

    /*!
     * \brief Checks if an access token is present in the file.
     */
    void checkAccessToken();

    /*!
     * \brief Makes a request to get a new access token.
     * \param const QString &grantToken -- grant token provided by the user.
     */
    void requestToken(const QString &grantToken);

    /*!
     * \brief Makes a request to get all the currency rates.
     */
    void makeCurrenciesRequest();

    /*!
     * \brief Returns list of invoices.
     */
    QList<Invoice> invoices() const;

    /*!
     * \brief Returns a list of invoices between set 'from date' and 'to date'.
     */
    QList<Invoice> rangedInvoices() const;

    /*!
     * \brief Returns a list of expenses.
     */
    QList<Expense> expenses() const;

    /*!
     * \brief Returns a list of expenses between set 'from date' and 'to date'.
     */
    QList<Expense> rangedExpenses() const;

    /*!
     * \brief Returns a list of bills.
     */
    QList<Bill> bills() const;

    /*!
     * \brief Returns a list of bills between set 'from date' and 'to date'.
     */
    QList<Bill> rangedBills() const;

    /*!
     * \brief Returns a list of forecasts.
     */
    QList<ForecastingModel::Forecast>& forecasts();

    /*!
     * \brief Returns exchange rates.
     */
    QMap<QString, double> exchangeRates() const;
    
    /*!
     * \brief Returns true if forecasting functionality is enabled. Otherwise false.
     */
    bool isForecastingEnabled() const;

    /*!
     * \brief Sets state of the forecasting functionality enabled.
     * \param bool value -- value to set.
     */
    void setForecastingEnabled(bool value);

    /*!
     * \brief Returns true if the application is in the demo mode. Otherwise false.
     */
    bool isDemoMode() const;

    /*!
     * \brief Sets state of the demo mode.
     * \param bool valueIsDemoMode -- value to set.
     */
    void setIsDemoMode(bool valueIsDemoMode);

    /*!
     * \brief Reads all the files from local storage. Utilized for the demo mode.
     */
    void readFiles();

    /*!
     * \brief Reads invoices data data file.
     */
    void readInvoicesFile() const;

    /*!
     * \brief Reads normal expenses mock-data file.
     */
    void readNormalExpensesFile() const;

    /*!
     * \brief Reads recurrent expenses mock-data file.
     */
    void readRecurrentExpensesFile() const;

    /*!
     * \brief Reads normal bills mock-data file.
     */
    void readNormalBillsFile() const;

    /*!
     * \brief Reads recurrent bills mock-data file.
     */
    void readRecurrentBillsFile() const;

    /*!
     * \brief Clears lists of invoices, expenses and bills.
     */
    void clearContainers();

    /*!
     * \brief Returns first data of the financial history.
     */
    QDate firstDate() const;

    /*!
     * \brief Sets first date of the financial history.
     * \param const QDate &date -- value to set.
     */
    void setFirstDate(const QDate &date);

    /*!
     * \brief Returns last date of the financial history.
     */
    QDate lastDate() const;

    /*!
     * \brief Sets last date of the financial history.
     * \param const QDate &date -- value to set.
     */
    void setLastDate(const QDate &date);

    /*!
     * \brief Returns 'from date' bouding displaying chart.
     */
    QDate fromDate() const;

    /*!
     * \brief Sets 'from date' bounding displaying chart.
     * \param const QDate &fromDate -- value to set.
     */
    void setFromDate(const QDate &fromDate);

    /*!
     * \brief Returns 'to date' bouding displaying chart.
     */
    QDate toDate() const;

    /*!
     * \brief Sets 'to date' bounding displaying chart.
     * \param const QDate &toDate -- value to set.
     */
    void setToDate(const QDate &toDate);

    /*!
     * \brief Returns true if at least one request during this session has already been made. Otherwise false. Changing the mode of the application sets this variable to false.
     */
    bool requestMade() const;

    /*!
     * \brief Sets state of a condition if at least one request during this session has already been made.
     * \param bool value -- value to set.
     */
    void setRequestMade(bool value);

    /*!
     * \brief Returns true if the application is in the dark mode. Otherwise false.
     */
    bool isDarkMode() const;

    /*!
     * \brief Sets the state of the dark mode.
     * \param bool isDarkMode -- value to set.
     */
    void setIsDarkMode(bool isDarkMode);

    /*!
     * \brief Prepares fake currency rates for demo mode.
     */
    void prepareFakeRates();

    /*!
     * \brief Clears exchange rates map.
     */
    void clearExchangeRates();

signals:

    /*!
     * \brief This signal is emitted when requested invoices have been proceeded and are ready to be displayed.
     */
    void invoicesReady();

    /*!
     * \brief This signal is emitted when requested expenses have been proceeded and are ready to be displayed.
     */
    void expensesReady();

    /*!
     * \brief This signal is emitted when requested bills have been proceeded and are ready to be displayed.
     */
    void billsReady();

    /*!
     * \brief This signal is emitted for changing visibility of the error label.
     * \param bool visible -- value to set.
     */
    void errorLabelVisibilityRequested(bool visible);

    /*!
     * \brief This signal is emitted when requested exchange rates have been proceeded and are ready to be utilized.
     */
    void exchangeRateSReceived();

    /*!
     * \brief This signal is emitted when application enters or exits demo mode.
     */
    void modeChanged();

private slots:
    void setInvoices(QList<Invoice> &invoices);
    void setExpenses(const QList<Expense> &expenses);
    void addExpenses(QList<Expense> &expenses);
    void addRate(const QString &currency_code, const double &rate);
    void setBills(const QList<Bill> &bills);
    void addBills(QList<Bill> &bills);
    void updateAccessAndRefreshTokens(const QString &accessToken, const QString &refreshToken);

private:
    friend class InvoicesModel;
    friend class BillsModel;
    friend class ExpensesModel;
    friend class ForecastingModel;

    Settings* m_settings = nullptr;
    WebClient* m_webClient = nullptr;

    QList<Invoice> m_invoices = {};
    QList<Expense> m_expenses = {};
    QList<Bill> m_bills = {};
    QList<ForecastingModel::Forecast> m_forecasts = {};

    bool m_normalExpensesArrived = false;
    bool m_recurrentExpensesArrived = false;
    bool m_normalBillsArrived = false;
    bool m_recurrentBillsArrived = false;

    QMap<QString, double> m_exchangeRates;

    bool m_forecastingEnabled = true;
    QDate m_firstDate;
    QDate m_lastDate;

    QDate m_fromDate;
    QDate m_toDate;

    bool m_requestMade = false; //need for adding forecasts before update button clicked
};

#endif // LOGICCONTROLLER_H
