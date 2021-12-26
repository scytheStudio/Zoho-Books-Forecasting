// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QDate>

#include "datasets/Invoice.h"
#include "datasets/Expense.h"
#include "datasets/Bill.h"

/*!
 * \brief Class representing web-client making requests for data and tokens.
 */
class WebClient : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     */
    explicit WebClient(QObject *parent = nullptr);

    // getting data

    /*!
     * \brief Makes GET request for invoices.
     * \param const QString &accessToken -- access token for making request.
     */
    void getInvoicesRequest(const QString &accessToken);

    /*!
     * \brief Makes GET request for expenses.
     * \param const QString &accessToken -- access token for making request.
     */
    void getExpensesRequest(const QString &accessToken);

    /*!
     * \brief Makes GET request for recurring expenses.
     * \param const QString &accessToken -- access token for making request.
     */
    void getRecurringExpensesRequest(const QString &accessToken);

    /*!
     * \brief Makes GET request for bills.
     * \param const QString &accessToken -- access token for making request.
     */
    void getBillsRequest(const QString &accessToken);

    /*!
     * \brief Makes GET request for recurring bills.
     * \param const QString &accessToken -- access token for making request.
     */
    void getRecurringBillsRequest(const QString &accessToken);

    void getRecurringBillRequest(const QString &accessToken, const QString &recurring_bill_id);

    /*!
     * \brief Makes GET request for list of supported currencies.
     * \param const QString &accessToken -- access token for making request.
     */
    void getListOfCurrenciesRequest(const QString &accessToken);

    /*!
     * \brief Makes GET request for exchange rates.
     * \param const QString &accessToken -- access token for making request.
     * \param const QString &currency_id -- id of a currency to get exchange retes for.
     */
    void getExchangeRateRequest(const QString &accessToken, const QString &currency_id);

    // authentication related requests

    /*!
     * \brief Makes POST request for refreshing access token.
     * \param const QString &refreshToken -- refresh token to use for getting new access token.
     */
    void postRefreshAccessTokenRequest(const QString &refreshToken);

    /*!
     * \brief Makes GET request for updating both access and refresh tokens.
     * \param const QString &grantCode -- grant token provided by the user.
     */
    void postNewAccessAndRefreshTokensRequest(const QString &grantCode);

    /*!
     * \brief Makes GET mock-request.
     * \param const QString &accessToken -- access token for making request.
     * \param const QString &refreshToken -- refresh token to use for getting new access token.
     */
    void getMocRequest(const QString &accessToken, const QString &refreshToken);

    void parseGetRecurringBillResponse(const QByteArray &repsonse);
signals:

    /*!
     * \brief This signal is emitted when access token has been successfully refreshed.
     * \param const QString &accessToken -- access token for making request.
     */
    void accessTokenRefreshed(const QString &accessToken);

    /*!
     * \brief This signal is emitted  when invoices have been proceeded and can be utilized.
     * \param QList<Invoice> &invoices -- list of invoices.
     */
    void invoicesReceived(QList<Invoice> &invoices);

    /*!
     * \brief This signal is emitted when recurring expenses have been proceeded and can be utilized.
     * \param QList<Expense> &expenses -- list of expenses.
     */
    void recurringExpensesReceived(QList<Expense> &recurringExpenses);

    /*!
     * \brief This signal is emitted when normal expenses have been proceeded and can be utilized.
     * \param QList<Expense> &expenses -- list of expenses.
     */
    void normalExpensesReceived(QList<Expense> &expenses);

    /*!
     * \brief This signal is emitted when recurring bills have been proceeded and can be utilized.
     * \param QList<Bill> &bills -- list of bills.
     */
    void recurringBillsReceived(QList<Bill> &recurringBills);

    /*!
     * \brief This signal is emitted when normal bills have been proceeded and can be utilized.
     * \param QList<Bill> &bills -- list of bills.
     */
    void normalBillsReceived(QList<Bill> &bills);

    /*!
     * \brief This signal is emitted when GET mock-request had positive response.
     */
    void mockSignalSuccessful();

    /*!
     * \brief This signal is emitted when refreshing access token failed.
     */
    void accessTokenRefreshingFailed();

    /*!
     * \brief This signal is emitted when both access and refresh token have been successfully refreshed.
     * \param const QString &accessToken -- access token for making request.
     * \param const QString &refreshToken -- refresh token to use for getting new access token.
     */
    void accessAndRefreshTokensRefreshed(const QString &accessToken, const QString &refreshToken);

    /*!
     * \brief This signal is emitted when currency exchange rates have been received.
     * \param const QString &currency_code -- code of a currency.
     * \param -- exchange rate for the currency.
     */
    void exchangeRateReceived(const QString &currency_code, const double &exchangeRate);

private:
    void parseGetInvoicesResponse(const QByteArray &response);
    void parseGetExpensesResponse(const QByteArray &response);
    void parseGetRecurringExpensesResponse(const QByteArray &response);
    void parseGetBillsResponse(const QByteArray &response);
    void parseGetRecurringBillsResponse(const QByteArray &response, const QString &accessToken);
    void parsePostRefreshAccessTokenResponse(const QByteArray &response);
    void parsePostNewAccessAndRefreshTokenRequest(const QByteArray &response);
    void parseGetListOfCurrencies(const QByteArray &response, const QString &accessToken);
    void parseGetExchageRate(const QByteArray &response);

private:
    QNetworkAccessManager *m_manager = nullptr;
    QStringList m_recurringBillsNumbers; // Used for recurring bills issue.
    QList<Bill> m_recurringBills;
};

#endif // WEBCLIENT_H
