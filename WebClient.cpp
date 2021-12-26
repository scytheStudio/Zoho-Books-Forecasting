// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "WebClient.h"
#include "Settings.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QUrl>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonObject>

#define CLIENT_ID "1000.6F1ZTL3L8Q04ZCOH4XNI4T04U0TL7F"
#define CLIENT_SECRET "05758cb8651c830c570d2fb3e69c7291909a8449ca"
#define REDIRECT_URI "https://scythe-studio.com"
#define GRANT_TYPE_REFRESH_TOKEN "refresh_token"
#define GRANT_TYPE_AUTHORIZATION_CODE "authorization_code"
#define SCOPE "ZohoBooks.fullaccess.READ"
#define SERVER_ADDRESS "https://books.zoho.eu/api/v3/"
#define CONTENT_TYPE "application/x-www-form-urlencoded;charset=UTF-8"
#define ORGANIZATION_ID "20077575557"
#define NEXT_EXPENSE_DATE "next_expense_date"
#define NEXT_BILL_DATE "next_bill_date"
#define EXCLUDE_BASE "Currencies.ExcludeBaseCurrency"

WebClient::WebClient(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
{
}

//requests

void WebClient::getInvoicesRequest(const QString &accessToken)
{
    QUrl url(QString(SERVER_ADDRESS "invoices"));
    QUrlQuery query;
    query.addQueryItem("organization_id", ORGANIZATION_ID);
    query.addQueryItem("sort_column", "due_date");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
    request.setRawHeader("Content-type", CONTENT_TYPE);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            parseGetInvoicesResponse(reply->readAll());
        }
        reply->deleteLater();
    });
}

void WebClient::getExpensesRequest(const QString &accessToken)
{
     QUrl url(QString(SERVER_ADDRESS "expenses"));
     QUrlQuery query;
     query.addQueryItem("organization_id", ORGANIZATION_ID);
     url.setQuery(query);

     QNetworkRequest request(url);
     request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
     QNetworkReply *reply = m_manager->get(request);

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parseGetExpensesResponse(reply->readAll());
         }
         reply->deleteLater();
     });
}

void WebClient::getRecurringExpensesRequest(const QString &accessToken)
{
     QUrl url(QString(SERVER_ADDRESS "recurringexpenses"));
     QUrlQuery query;
     query.addQueryItem("organization_id", ORGANIZATION_ID);
     url.setQuery(query);

     QNetworkRequest request(url);
     request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
     QNetworkReply *reply = m_manager->get(request);

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parseGetRecurringExpensesResponse(reply->readAll());
         }
         reply->deleteLater();
     });
}

void WebClient::getBillsRequest(const QString &accessToken)
{
     QUrl url(QString(SERVER_ADDRESS "bills"));
     QUrlQuery query;
     query.addQueryItem("organization_id", ORGANIZATION_ID);
     url.setQuery(query);

     QNetworkRequest request(url);
     request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
     QNetworkReply *reply = m_manager->get(request);

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parseGetBillsResponse(reply->readAll());
         }
         reply->deleteLater();
     });
}

void WebClient::getRecurringBillsRequest(const QString &accessToken)
{
     m_recurringBillsNumbers.clear();
     m_recurringBills.clear();
     QUrl url(QString(SERVER_ADDRESS "recurringbills"));
     QUrlQuery query;
     query.addQueryItem("organization_id", ORGANIZATION_ID);
     url.setQuery(query);

     QNetworkRequest request(url);
     request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
     QNetworkReply *reply = m_manager->get(request);

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parseGetRecurringBillsResponse(reply->readAll(), accessToken);
         }
         reply->deleteLater();
     });
}

void WebClient::postRefreshAccessTokenRequest(const QString &refreshToken)
{
     QUrl url(QString("https://accounts.zoho.eu/oauth/v2/token"));
     QUrlQuery query;
     query.addQueryItem("refresh_token", refreshToken.toUtf8());
     query.addQueryItem("client_id", CLIENT_ID);
     query.addQueryItem("client_secret", CLIENT_SECRET);
     query.addQueryItem("redirect_uri", REDIRECT_URI);
     query.addQueryItem("grant_type", GRANT_TYPE_REFRESH_TOKEN);
     url.setQuery(query);

     QNetworkRequest request(url);
     QNetworkReply* reply = m_manager->post(request, new QHttpMultiPart());

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parsePostRefreshAccessTokenResponse(reply->readAll());
         }
         reply->deleteLater();
     });
}

void WebClient::postNewAccessAndRefreshTokensRequest(const QString &grantCode)
{
     QUrl url(QString("https://accounts.zoho.eu/oauth/v2/token"));
     QUrlQuery query;
     query.addQueryItem("client_id", CLIENT_ID);
     query.addQueryItem("client_secret", CLIENT_SECRET);
     query.addQueryItem("grant_type", GRANT_TYPE_AUTHORIZATION_CODE);
     query.addQueryItem("scope", SCOPE);
     query.addQueryItem("code", grantCode);
     query.addQueryItem("redirect_uri", REDIRECT_URI);
     url.setQuery(query);

     QNetworkRequest request(url);
     QNetworkReply* reply = m_manager->post(request, new QHttpMultiPart());

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parsePostNewAccessAndRefreshTokenRequest(reply->readAll());
         } else {
             reply->readAll();
             reply->errorString();
             reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute).toString();
             emit accessTokenRefreshingFailed();
         }
         reply->deleteLater();
     });
}

void WebClient::getMocRequest(const QString &accessToken, const QString &refreshToken)
{
    QUrl url(QString(SERVER_ADDRESS "organizations"));
    QUrlQuery query;
    query.addQueryItem("organization_id", ORGANIZATION_ID);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit mockSignalSuccessful();
            getListOfCurrenciesRequest(accessToken);
        } else {
            postRefreshAccessTokenRequest(refreshToken);
        }
        reply->deleteLater();
    });
}

void WebClient::getListOfCurrenciesRequest(const QString &accessToken)
{
    QUrl url(QString(SERVER_ADDRESS "settings/currencies?filter_by=Currencies.ExcludeBaseCurrency"));
    QUrlQuery query;
    query.addQueryItem("organization_id", ORGANIZATION_ID);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            parseGetListOfCurrencies(reply->readAll(), accessToken);
        }
        reply->deleteLater();
    });
}

void WebClient::getExchangeRateRequest(const QString &accessToken, const QString &currency_id)
{
    QUrl url(QString("https://books.zoho.eu/api/v3/settings/currencies/" + currency_id + "/exchangerates/"));
    QUrlQuery query;
    query.addQueryItem("organization_id", ORGANIZATION_ID);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            parseGetExchageRate(reply->readAll());
        }
        reply->deleteLater();
    });
}

//parsing

void WebClient::parseGetInvoicesResponse(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonInvoices = jsonResponse["invoices"].toArray();

    QList<Invoice> invoices;

    for (const auto &invoiceJson : jsonInvoices) {
        const auto &invoiceMap = invoiceJson.toVariant().toMap();
        invoices << Invoice::parseInvoice(invoiceMap);
    }

    emit invoicesReceived(invoices);
}

void WebClient::parseGetExpensesResponse(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonExpenses = jsonResponse["expenses"].toArray();

    QList<Expense> expenses;

    for (const auto &expenseJson : jsonExpenses)
    {
        const auto &expenseMap = expenseJson.toVariant().toMap();
        expenses << Expense::parseNormalExpense(expenseMap);
    }

    emit normalExpensesReceived(expenses);
}

void WebClient::parseGetRecurringExpensesResponse(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonRecurringExpenses = jsonResponse["recurring_expenses"].toArray();

    QList<Expense> recurringExpenses;

    for (const auto &recurringExpenseJson : jsonRecurringExpenses) {
        const auto &recurrentExpenseMap = recurringExpenseJson.toVariant().toMap();
        const Expense &expense = Expense::parseRecurrentExpense(recurrentExpenseMap);
        recurringExpenses << expense;
    }

    emit recurringExpensesReceived(recurringExpenses);
}

void WebClient::parseGetBillsResponse(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonBills = jsonResponse["bills"].toArray();

    QList<Bill> bills;

    for (const auto &billJson : jsonBills) {
        const auto &billMap = billJson.toVariant().toMap();
        bills << Bill::parseNormalBill(billMap);
    }

    emit normalBillsReceived(bills);
}

void WebClient::parseGetRecurringBillsResponse(const QByteArray &response, const QString &accessToken)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonRecurringBills = jsonResponse["recurring_bills"].toArray();

    for (const auto &jsonRecurrentBill : jsonRecurringBills) {
        const auto &recurrentBillMap = jsonRecurrentBill.toVariant().toMap();
        m_recurringBillsNumbers << recurrentBillMap["recurring_bill_id"].toString();
    }

    for (auto it = m_recurringBillsNumbers.begin(); it != m_recurringBillsNumbers.end(); ++it) {
        getRecurringBillRequest(accessToken, *it);
    }
}

void WebClient::getRecurringBillRequest(const QString &accessToken, const QString &recurring_bill_id)
{
     QUrl url(QString(SERVER_ADDRESS "recurringbills/" + recurring_bill_id));
     QUrlQuery query;
     query.addQueryItem("organization_id", ORGANIZATION_ID);
     url.setQuery(query);

     QNetworkRequest request(url);
     request.setRawHeader("Authorization", "Zoho-oauthtoken " + accessToken.toUtf8());
     QNetworkReply *reply = m_manager->get(request);

     connect(reply, &QNetworkReply::finished, this, [=]() {
         if (reply->error() == QNetworkReply::NoError) {
             parseGetRecurringBillResponse(reply->readAll());
         }
         reply->deleteLater();
     });
}
void WebClient::parseGetRecurringBillResponse(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonObject &jsonObject = jsonResponse["recurring_bill"].toObject();

    m_recurringBills << Bill::parseRecurringBill(jsonObject.toVariantMap());

    if (m_recurringBills.size() == m_recurringBillsNumbers.size()) {
        emit recurringBillsReceived(m_recurringBills);
    }
}

void WebClient::parsePostRefreshAccessTokenResponse(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QString &parsedAccessToken = jsonResponse["access_token"].toString();
    if (parsedAccessToken == "") {
        emit accessTokenRefreshingFailed();
    } else {
        qDebug() << "New access token: " << parsedAccessToken;
        emit accessTokenRefreshed(parsedAccessToken);
        getListOfCurrenciesRequest(parsedAccessToken);
    }
}

void WebClient::parsePostNewAccessAndRefreshTokenRequest(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QString &parsedAccessToken = jsonResponse["access_token"].toString();
    const QString &parsedRefreshToken = jsonResponse["refresh_token"].toString();
    emit accessAndRefreshTokensRefreshed(parsedAccessToken, parsedRefreshToken);
    getListOfCurrenciesRequest(parsedAccessToken);
}

void WebClient::parseGetListOfCurrencies(const QByteArray &response, const QString &accessToken)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonCurrencies = jsonResponse["currencies"].toArray();

    for (const auto& jsonCurrency : jsonCurrencies) {
        const auto &jsonCurrencyMap = jsonCurrency.toVariant().toMap();
        const QString currency_id = jsonCurrencyMap["currency_id"].toString();
        getExchangeRateRequest(accessToken, currency_id);
    }
}

void WebClient::parseGetExchageRate(const QByteArray &response)
{
    const QJsonDocument &jsonResponse = QJsonDocument::fromJson(response);
    const QJsonArray &jsonExchangeRates = jsonResponse["exchange_rates"].toArray();

    for (const auto& jsonExchangeRate : jsonExchangeRates) {
        const auto &jsonExchangeRateMap = jsonExchangeRate.toVariant().toMap();
        const double exchangeRate = jsonExchangeRateMap["rate"].toDouble();
        const QString currency_code = jsonExchangeRateMap["currency_code"].toString();
        emit exchangeRateReceived(currency_code, exchangeRate);
    }
}
