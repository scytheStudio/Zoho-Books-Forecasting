// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "Bill.h"

#define BILL_NUMBER "bill_number"
#define RECURRING_BILL_NUMBER "recurring_bill_id"
#define PARTY "vendor_name"
#define RECURRENCE_FREQUENCY "recurrence_frequency"
#define STATUS "status"
#define DATE "date"
#define DUE_DATE "due_date"
#define NEXT_BILL_DATE "next_bill_date"
#define CURRENCY_CODE "currency_code"
#define CURRENCY_SYMBOL "currency_symbol"
#define TOTAL "total"

Bill::Bill()
{

}

QString Bill::billNumber() const
{
    return m_billNumber;
}

QString Bill::party() const
{
    return m_party;
}

bool Bill::isRecurrent() const
{
    return m_isRecurrent;
}

QString Bill::status() const
{
    return m_status;
}

QString Bill::recurrence_frequency() const
{
    return m_recurrence_frequency;
}

QDate Bill::date() const
{
    return m_date;
}

QDate Bill::dueDate() const
{
    return m_dueDate;
}

QDate Bill::nextBillDate() const
{
    return m_nextBillDate;
}

QString Bill::currencyCode() const
{
    return m_currencyCode;
}

double Bill::total() const
{
    return m_total;
}

double Bill::plnTotal() const
{
    return m_plnTotal;
}

void Bill::setPlnTotal(const double &amount)
{
    m_plnTotal = amount;
}

Bill Bill::parseNormalBill(const QVariantMap &map)
{
    Bill bill;
    bill.m_billNumber = map[BILL_NUMBER].toString();
    bill.m_party = map[PARTY].toString();
    bill.m_isRecurrent = false;
    bill.m_status = map[STATUS].toString();
    bill.m_date = map[DATE].toDate();
    bill.m_dueDate = map[DUE_DATE].toDate();
    bill.m_nextBillDate = map[NEXT_BILL_DATE].toDate();
    bill.m_currencyCode = map[CURRENCY_CODE].toString();
    bill.m_currencySymbol = map[CURRENCY_SYMBOL].toString();
    bill.m_total = map[TOTAL].toDouble();
    return bill;
}

Bill Bill::parseRecurringBill(const QVariantMap &map)
{
    Bill bill;
    bill.m_billNumber = map[RECURRING_BILL_NUMBER].toString();
    bill.m_party = map[PARTY].toString();
    bill.m_isRecurrent = true;
    bill.m_status = map[STATUS].toString();
    bill.m_recurrence_frequency = map[RECURRENCE_FREQUENCY].toString();
    bill.m_date = map[DATE].toDate();
    bill.m_dueDate = map[DUE_DATE].toDate();
    bill.m_nextBillDate = map[NEXT_BILL_DATE].toDate();
    bill.m_currencyCode = map[CURRENCY_CODE].toString();
    bill.m_currencySymbol = map[CURRENCY_SYMBOL].toString();
    bill.m_total = map[TOTAL].toDouble();
    return bill;
}

Bill Bill::parseNormalBill(const QStringList &list)
{
    Bill bill;
    bill.m_billNumber = list.at(0);
    bill.m_party = list.at(1);
    bill.m_isRecurrent = false;
    bill.m_status = list.at(3);
    bill.m_date = QDate::fromString(list.at(5), "d-M-yyyy");
    bill.m_dueDate = QDate::fromString(list.at(6), "d-M-yyyy");
    bill.m_total = list.at(8).toDouble();
    bill.m_currencyCode = list.at(9);
    return bill;
}

Bill Bill::parseRecurringBill(const QStringList &list)
{
    Bill bill;
    bill.m_billNumber = list.at(0);
    bill.m_party = list.at(1);
    bill.m_isRecurrent = true;
    bill.m_status = list.at(3);
    bill.m_recurrence_frequency = list.at(4);
    bill.m_nextBillDate = QDate::fromString(list.at(7), "d-M-yyyy");
    bill.m_total = list.at(8).toDouble();
    bill.m_currencyCode = list.at(9);
    return bill;
}
