// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "Invoice.h"
#include <QDebug>

#define INVOICE_NUMBER "invoice_number"
#define PARTY "customer_name"
#define STATUS "status"
#define DATE "date"
#define DUE_DATE "due_date"
#define TOTAL "total"
#define CURRENCY_CODE "currency_code"

Invoice::Invoice()
{

}


QString Invoice::status() const
{
    return m_status;
}

QString Invoice::invoiceNumber() const
{
    return m_invoiceNumber;
}

QString Invoice::party() const
{
    return m_party;
}

QDate Invoice::date() const
{
    return m_date;
}

QDate Invoice::dueDate() const
{
    return m_dueDate;
}

QString Invoice::currencyCode() const
{
    return m_currencyCode;
}

double Invoice::total() const
{
    return m_total;
}

double Invoice::plnTotal() const
{
    return m_plnTotal;
}

void Invoice::setPlnTotal(const double &amount)
{
    m_plnTotal = amount;
}

Invoice Invoice::parseInvoice(const QVariantMap &map)
{
    Invoice invoice;

    invoice.m_status = map[STATUS].toString();
    invoice.m_invoiceNumber = map[INVOICE_NUMBER].toString();
    invoice.m_party = map[PARTY].toString();
    invoice.m_date = map[DATE].toDate();
    invoice.m_dueDate = map[DUE_DATE].toDate();
    invoice.m_currencyCode = map[CURRENCY_CODE].toString();
    invoice.m_total = map[TOTAL].toDouble();
    return invoice;
}

Invoice Invoice::parseInvoice(const QStringList &list)
{
    Invoice invoice;
    invoice.m_invoiceNumber = list.at(0);
    invoice.m_party = list.at(1);
    invoice.m_status = list.at(2);
    invoice.m_date = QDate::fromString(list.at(3), "d-M-yyyy");
    invoice.m_dueDate = QDate::fromString(list.at(4), "d-M-yyyy");
    invoice.m_total = list.at(5).toDouble();
    invoice.m_currencyCode = list.at(6);
    return invoice;
}
