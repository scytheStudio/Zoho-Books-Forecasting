// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "Expense.h"
#include <QDebug>

#define EXPENSE_ID "expense_id"
#define RECURRING_EXPENSE_ID "recurring_expense_id"
#define STATUS "status"
#define CATEGORY "account_name"
#define PARTY_NAME "vendor_name"
#define RECURRENCE_FREQUENCY "recurrence_frequency"
#define DATE "date"
#define NEXT_EXPENSE_DATE "next_expense_date"
#define CURRENCY_CODE "currency_code"
#define TOTAL "total"

Expense::Expense()
{

}

QString Expense::expenseId() const
{
    return m_expenseId;
}

QString Expense::status() const
{
    return m_status;
}

QString Expense::category() const
{
    return m_category;
}

QString Expense::partyName() const
{
    return m_partyName;
}

bool Expense::isRecurrent() const
{
    return m_isRecurrent;
}

QString Expense::recurrenceFrequency() const
{
    return m_recurrenceFrequency;
}

QDate Expense::date() const
{
    return m_date;
}

QDate Expense::nextExpenseDate() const
{
    return m_nextExpenseDate;
}

QString Expense::currencyCode() const
{
    return m_currencyCode;
}

double Expense::total() const
{
    return m_total;
}

double Expense::plnTotal() const
{
    return m_plnTotal;
}

void Expense::setPlnTotal(const double &amount)
{
    m_plnTotal = amount;
}

Expense Expense::parseNormalExpense(const QVariantMap &map)
{
    Expense expense;
    expense.m_expenseId = map[EXPENSE_ID].toString();
    expense.m_status = map[STATUS].toString();
    expense.m_category = map[CATEGORY].toString();
    expense.m_partyName = map[PARTY_NAME].toString();
    expense.m_isRecurrent = false;
    expense.m_date = map[DATE].toDate();
    expense.m_nextExpenseDate = map[NEXT_EXPENSE_DATE].toDate();
    expense.m_currencyCode = map[CURRENCY_CODE].toString();
    expense.m_total = map[TOTAL].toDouble();
    return expense;
}

Expense Expense::parseRecurrentExpense(const QVariantMap &map)
{
    Expense expense;
    expense.m_expenseId = map[EXPENSE_ID].toString();
    expense.m_status = map[STATUS].toString();
    expense.m_category = map[CATEGORY].toString();
    expense.m_partyName = map[PARTY_NAME].toString();
    expense.m_isRecurrent = true;
    expense.m_recurrenceFrequency = map[RECURRENCE_FREQUENCY].toString();
    expense.m_date = map[DATE].toDate();
    expense.m_nextExpenseDate = map[NEXT_EXPENSE_DATE].toDate();
    expense.m_currencyCode = map[CURRENCY_CODE].toString();
    expense.m_total = map[TOTAL].toDouble();
    return expense;
}

Expense Expense::parseNormalExpense(const QStringList &list)
{
    Expense expense;
    expense.m_expenseId = list.at(0);
    expense.m_status = list.at(1);
    expense.m_category = list.at(2);
    expense.m_partyName = list.at(3);
    expense.m_isRecurrent = false;
    expense.m_date = QDate::fromString(list.at(6), "d-M-yyyy");
    expense.m_total = list.at(8).toDouble();
    expense.m_currencyCode = list.at(9);
    return expense;
}

Expense Expense::parseRecurrentExpense(const QStringList &list)
{
    Expense expense;
    expense.m_expenseId = list.at(0);
    expense.m_status = list.at(1);
    expense.m_category = list.at(2);
    expense.m_partyName = list.at(3);
    expense.m_isRecurrent = true;
    expense.m_recurrenceFrequency = list.at(5);
    expense.m_nextExpenseDate = QDate::fromString(list.at(7), "d-M-yyyy");
    expense.m_total = list.at(8).toDouble();
    expense.m_currencyCode = list.at(9);
    return expense;
}
