// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "LogicController.h"
#include <QDate>
#include <QFile>
#include <QStyleFactory>

#define NUMBER_OF_SUPPORTED_EXCHANGE_RATES 11

LogicController::LogicController(QObject *parent)
    : QObject(parent)
    , m_settings(new Settings(this))
    , m_webClient(new WebClient(this))
{
    connect(m_webClient, &WebClient::invoicesReceived, this, [&](QList<Invoice> list){
        setInvoices(list);
    });

    // It is important to keep consistency here and set '...Arrived' variable to true before adding objects.
    connect(m_webClient, &WebClient::normalExpensesReceived, this, [this] {
        m_normalExpensesArrived = true;
    });
    connect(m_webClient, &WebClient::normalExpensesReceived, this, &LogicController::addExpenses);

    connect(m_webClient, &WebClient::recurringExpensesReceived, this, [this] {
        m_recurrentExpensesArrived = true;
    });
    connect(m_webClient, &WebClient::recurringExpensesReceived, this, &LogicController::addExpenses);

    connect(m_webClient, &WebClient::normalBillsReceived, this, [this] {
        m_normalBillsArrived = true;
    });
    connect(m_webClient, &WebClient::normalBillsReceived, this, &LogicController::addBills);

    connect(m_webClient, &WebClient::recurringBillsReceived, this, [&] {
        m_recurrentBillsArrived = true;
    });
    connect(m_webClient, &WebClient::recurringBillsReceived, this, &LogicController::addBills);

    connect(m_webClient, &WebClient::accessTokenRefreshed, m_settings, &Settings::setAccessToken);
    connect(m_webClient, &WebClient::accessAndRefreshTokensRefreshed, this, &LogicController::updateAccessAndRefreshTokens);
    connect(m_webClient, &WebClient::exchangeRateReceived, this, &LogicController::addRate);

    connect(m_webClient, &WebClient::mockSignalSuccessful, this, [this]() {
        emit errorLabelVisibilityRequested(false);
    });
    connect(m_webClient, &WebClient::accessTokenRefreshingFailed, this, [this]() {
        emit errorLabelVisibilityRequested(true);
    });
    connect(m_webClient, &WebClient::accessAndRefreshTokensRefreshed, this, [this]() {
        emit errorLabelVisibilityRequested(false);
    });
    connect(m_webClient, &WebClient::accessTokenRefreshed, this, [this]() {
        emit errorLabelVisibilityRequested(false);
    });
}

void LogicController::makeCurrenciesRequest()
{
    m_webClient->getListOfCurrenciesRequest(m_settings->accessToken());
}

void LogicController::requestAllData()
{
    // web client requests to call
    m_webClient->getInvoicesRequest(m_settings->accessToken());
    m_webClient->getBillsRequest(m_settings->accessToken());
    m_webClient->getRecurringBillsRequest(m_settings->accessToken());
    m_webClient->getExpensesRequest(m_settings->accessToken());
    m_webClient->getRecurringExpensesRequest(m_settings->accessToken());
}

void LogicController::requestToken(const QString &grantToken)
{
    m_webClient->postNewAccessAndRefreshTokensRequest(grantToken);
}

void LogicController::checkAccessToken()
{
    if (m_settings->accessToken() == "") { // access token IS NOT in the file.
        if (m_settings->refreshToken() == "") // refresh token IS NOT in the file.
        {
            qWarning() << "Both access token and refresh token are missing";
            emit errorLabelVisibilityRequested(true);
        }
        else // refresh token is in the file.
        {
            m_webClient->postRefreshAccessTokenRequest(m_settings->refreshToken());
        }
    }
    else // access token IS NOT in the file.
    {
        qDebug() << "Current token: " << m_settings->accessToken();
        m_webClient->getMocRequest(m_settings->accessToken(), m_settings->refreshToken());
    }
}

QList<Invoice> LogicController::invoices() const
{
    return m_invoices;
}

QList<Invoice> LogicController::rangedInvoices() const
{
    // list of invoices in the range provided by the user.
    QList<Invoice> list;
    for (auto &invoice : m_invoices) {
        QDate date = invoice.dueDate().isValid() ? invoice.dueDate() : invoice.date();
        if (m_fromDate <= date && date <= m_toDate) {
            list.append(invoice);
        }
    }
    return list;
}

void LogicController::setInvoices(QList<Invoice> &invoices)
{
    for (auto &invoice : invoices) {
        if (m_exchangeRates.contains(invoice.currencyCode())) { // not Zlote.
            invoice.setPlnTotal(m_exchangeRates[invoice.currencyCode()] * invoice.total());
        } else {
            invoice.setPlnTotal(invoice.total()); // Zlote.
        }
    }

    m_invoices = invoices;

    // sorting according to the due date.
    std::sort(m_invoices.begin(), m_invoices.end(), [](const Invoice& i1, const Invoice& i2){
        return i1.dueDate() < i2.dueDate();
    });

    // bellow is calculation of the first and of the last date of records. (not the bounds of a chart)
    QDate firstDateToMeasure = invoices.first().dueDate().isValid() ? invoices.first().dueDate() : invoices.first().date();
    if (firstDateToMeasure < m_firstDate) {
        m_firstDate = firstDateToMeasure;
    }

    QDate lastDateToMeasure = invoices.last().dueDate().isValid() ? invoices.last().dueDate() : invoices.last().date();
    if (lastDateToMeasure > m_lastDate) {
        m_lastDate = lastDateToMeasure;
    }

    emit invoicesReady();
}

QList<Expense> LogicController::expenses() const
{
    return m_expenses;
}

QList<Expense> LogicController::rangedExpenses() const
{
    // list of expenses in the range provided by the user.
    QList<Expense> list;
    for (auto &expense : m_expenses) {
        QDate date = expense.nextExpenseDate().isValid() ? expense.nextExpenseDate() : expense.date();
        if (m_fromDate <= date && date <= m_toDate) {
            list.append(expense);
        }
    }
    return list;
}

QList<ForecastingModel::Forecast> &LogicController::forecasts()
{
    return m_forecasts;
}

void LogicController::setExpenses(const QList<Expense> &expenses)
{
    m_expenses = expenses;
    emit expensesReady();
}

void LogicController::addExpenses(QList<Expense> &expenses)
{
    for (auto &expense : expenses) {
        if (m_exchangeRates.contains(expense.currencyCode())) {
            expense.setPlnTotal(m_exchangeRates[expense.currencyCode()] * expense.total());
        } else {
            expense.setPlnTotal(expense.total());
        }
    }

    m_expenses.append(expenses);

    if (m_normalExpensesArrived && m_recurrentExpensesArrived) {
        m_normalExpensesArrived = false;
        m_recurrentExpensesArrived = false;

        std::sort(m_expenses.begin(), m_expenses.end(), [](const Expense& e1, const Expense& e2){
            QDate d1 = e1.nextExpenseDate().isValid() ? e1.nextExpenseDate() : e1.date();
            QDate d2 = e2.nextExpenseDate().isValid() ? e2.nextExpenseDate() : e2.date();
            return d1 < d2;
        });

        // bellow is calculation of the first and of the last date of records. (not the bounds of a chart)

        QDate firstDateToMeasure = m_expenses.first().nextExpenseDate().isValid() ? m_expenses.first().nextExpenseDate() : m_expenses.first().date();

        if (firstDateToMeasure < m_firstDate) {
            m_firstDate = firstDateToMeasure;
        }
        QDate lastDateToMeasure = m_expenses.last().nextExpenseDate().isValid() ? m_expenses.last().nextExpenseDate() : m_expenses.last().date();

        if (lastDateToMeasure > m_lastDate) {
            m_lastDate = lastDateToMeasure;
        }
        emit expensesReady();
    }
}

QList<Bill> LogicController::bills() const
{
    return m_bills;
}

QList<Bill> LogicController::rangedBills() const
{
    // list of bills in the range provided by the user.
    QList<Bill> list;
    for (auto &bill : m_bills) {
        QDate date = bill.nextBillDate().isValid() ? bill.nextBillDate() : bill.dueDate().isValid() ? bill.dueDate() : bill.date();
        if (m_fromDate <= date && date <= m_toDate) {
            list.append(bill);
        }
    }
    return list;
}

void LogicController::clearExchangeRates()
{
    m_exchangeRates.clear();
}

QMap<QString, double> LogicController::exchangeRates() const
{
    return m_exchangeRates;
}

bool LogicController::isForecastingEnabled() const
{
    return m_forecastingEnabled;
}

void LogicController::setBills(const QList<Bill> &bills)
{
    m_bills = bills;
    emit billsReady();
}

void LogicController::addBills(QList<Bill> &bills)
{
    for (auto &bill : bills) {
        if (m_exchangeRates.contains(bill.currencyCode())) {
            bill.setPlnTotal(m_exchangeRates[bill.currencyCode()] * bill.total());
        } else {
            bill.setPlnTotal(bill.total());
        }
    }

    m_bills.append(bills);

    if (m_normalBillsArrived && m_recurrentBillsArrived) {
        m_normalBillsArrived = false;
        m_recurrentBillsArrived = false;
        std::sort(m_bills.begin(), m_bills.end(), [](const Bill& b1, const Bill& b2){
            QDate d1 = b1.nextBillDate().isValid() ? b1.nextBillDate() : b1.date();
            QDate d2 = b2.nextBillDate().isValid() ? b2.nextBillDate() : b2.date();
            return d1 < d2;
        });

        // bellow is calculation of the first and of the last date of records. (not the bounds of a chart)
        QDate firstDateToMeasure = m_bills.first().dueDate().isValid() ? m_bills.first().dueDate() : m_bills.first().date();
        if (m_firstDate < firstDateToMeasure) {
            m_firstDate = firstDateToMeasure;
        }

        QDate lastDateToMeasure = m_bills.last().dueDate().isValid() ? m_bills.last().dueDate() : m_bills.last().date();
        if (lastDateToMeasure > m_lastDate) {
            m_lastDate = lastDateToMeasure;
        }

        emit billsReady();
    }
}

void LogicController::addRate(const QString &currency_code, const double &rate)
{
    // theoretically number of exchange rates supported by Zoho might change in future. This macro can be changed.
    m_exchangeRates.insert(currency_code, rate);
    if (m_exchangeRates.size() == NUMBER_OF_SUPPORTED_EXCHANGE_RATES) {
        emit exchangeRateSReceived();
    }
}

void LogicController::updateAccessAndRefreshTokens(const QString &accessToken, const QString &refreshToken)
{
    m_settings->setAccessToken(accessToken);
    m_settings->setRefreshToken(refreshToken);
}

void LogicController::setIsDarkMode(bool isDarkMode)
{
    m_settings->setIsDarkMode(isDarkMode);
}

void LogicController::prepareFakeRates()
{
    m_exchangeRates.insert("EUR", 4.24);
    m_exchangeRates.insert("USD", 3.91);
    m_exchangeRates.insert("GBP", 5.5);
    m_exchangeRates.insert("AUS", 2.87);
    emit exchangeRateSReceived();
}

bool LogicController::isDarkMode() const
{
     return m_settings->isDarkMode();
}

void LogicController::setForecastingEnabled(bool value)
{
    m_forecastingEnabled = value;
}

QDate LogicController::firstDate() const
{
    return m_firstDate;
}

void LogicController::setFirstDate(const QDate& date)
{
    m_firstDate = date;
}

QDate LogicController::lastDate() const
{
    return m_lastDate;
}

void LogicController::setLastDate(const QDate &date)
{
    m_lastDate = date;
}

QDate LogicController::fromDate() const
{
    return m_fromDate;
}

void LogicController::setFromDate(const QDate& fromDate)
{
    m_fromDate = fromDate;
}

QDate LogicController::toDate() const
{
    return m_toDate;
}

void LogicController::setToDate(const QDate& toDate)
{
    m_toDate = toDate;
}

bool LogicController::requestMade() const
{
    return m_requestMade;
}

void LogicController::setRequestMade(bool value)
{
    m_requestMade = value;
}

void LogicController::readFiles()
{
    readInvoicesFile();
    readNormalExpensesFile();
    readRecurrentExpensesFile();
    readNormalBillsFile();
    readRecurrentBillsFile();
}

void LogicController::readInvoicesFile() const
{
    QFile file(":/assets/invoices.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine(); // skipping first line
        line = in.readLine();
        QList<Invoice> invoices;
        while (!line.isNull()) {
            QList<QString> splittedLine = line.split(",");
            invoices.append(Invoice::parseInvoice(splittedLine));
            line = in.readLine();
        }
        file.close();
        emit m_webClient->invoicesReceived(invoices);
    }
}

void LogicController::readNormalExpensesFile() const
{
    QFile file(":/assets/normalExpenses.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine(); // skipping first line
        line = in.readLine();
        QList<Expense> expenses;
        while (!line.isNull()) {
            QStringList splittedLine = line.split(",");
            expenses.append(Expense::parseNormalExpense(splittedLine));
            line = in.readLine();
        }
        file.close();
        emit m_webClient->normalExpensesReceived(expenses);
    }
}

void LogicController::readRecurrentExpensesFile() const
{
    QFile file(":/assets/recurrentExpenses.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine(); // skipping first line
        line = in.readLine();
        QList<Expense> expenses;
        while (!line.isNull()) {
            QStringList splittedLine = line.split(",");
            expenses.append(Expense::parseRecurrentExpense(splittedLine));
            line = in.readLine();
        }
        file.close();
        emit m_webClient->recurringExpensesReceived(expenses);
    }
}

void LogicController::readNormalBillsFile() const
{
    QFile file(":/assets/normalBills.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine(); // skipping first line
        line = in.readLine();
        QList<Bill> bills;
        while (!line.isNull()) {
            QList<QString> splittedLine = line.split(",");
            bills.append(Bill::parseNormalBill(splittedLine));
            line = in.readLine();
        }
        file.close();
        emit m_webClient->normalBillsReceived(bills);
    }
}

void LogicController::readRecurrentBillsFile() const
{
    QFile file(":/assets/recurrentBills.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine(); // skipping first line
        line = in.readLine();
        QList<Bill> bills;
        while (!line.isNull()) {
            QList<QString> splittedLine = line.split(",");
            bills.append(Bill::parseRecurringBill(splittedLine));
            line = in.readLine();
        }
        file.close();
        emit m_webClient->recurringBillsReceived(bills);
    }
}

void LogicController::clearContainers()
{
    m_invoices.clear();
    m_expenses.clear();
    m_bills.clear();
}

bool LogicController::isDemoMode() const
{
    return m_settings->isDemoMode();
}

void LogicController::setIsDemoMode(bool valueIsDemoMode)
{
   m_settings->setIsDemoMode(valueIsDemoMode);
   emit modeChanged();
}
