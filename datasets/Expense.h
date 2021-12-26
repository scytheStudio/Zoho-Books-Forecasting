// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef EXPENSE_H
#define EXPENSE_H

#include <QDate>
#include <QVariant>

/*!
 * \brief Class representing an expense entity.
 */
class Expense
{
public:

    /*!
     * \brief Constructor.
     */
    explicit Expense();

    /*!
     * \brief Returns an expense Id.
     */
    QString expenseId() const;

    /*!
     * \brief Returns a status of the expense (i.e. paid, overdue etc.).
     */
    QString status() const;

    /*!
     * \brief Returns a category the expense belongs to.
     */
    QString category() const;

    /*!
     * \brief Returns a party the expense was paid to.
     */
    QString partyName() const;

    /*!
     * \brief Returns true if the expense is recurrent. Otherwise returns false.
     */
    bool isRecurrent() const;

    /*!
     * \brief Returns a recurrence frequency of the expense.
     */
    QString recurrenceFrequency() const;

    /*!
     * \brief Returns a date of the expense.
     */
    QDate date() const;

    /*!
     * \brief Returns a next expense date of the expense.
     */
    QDate nextExpenseDate() const;

    /*!
     * \brief Returns a code of a currency the expense was paid in.
     */
    QString currencyCode() const;

    /*!
     * \brief Returns a total amount of the bill (no conversion performed).
     */
    double total() const;

    /*!
     * \brief returns a total amount of the expense in Polish Zlote.
     */
    double plnTotal() const;

    /*!
     * \brief Sets the value of total amount of the expense in Polish Zlote.
     * \param const double &amount -- amount to set.
     */
    void setPlnTotal(const double &amount);

    /*!
     * \brief Deserializes QVariantMap to create a normal (non-recurrent) expense.
     * \param const QVariantMap &map -- a map describing an expense object.
     */

    static Expense parseNormalExpense(const QVariantMap &map);
    /*!
     * \brief Deserializes QVariantMap to create a recurrent expense.
     * \param const QVariantMap &map -- a map describing an expense object.
     */
    static Expense parseRecurrentExpense(const QVariantMap &map);

    /*!
     * \brief Deserializes QStringList to create a normal (non-recurrent) expense.
     * \param const QStringList &list -- a list describing an expense objects.
     */
    static Expense parseNormalExpense(const QStringList &list);

    /*!
     * \brief Deserializes QStringList to create a recurrent expense.
     * \param const QStringList &list -- a list describing an expense objects.
     */
    static Expense parseRecurrentExpense(const QStringList &list);

private:
    QString m_expenseId = "";
    QString m_status = "";
    QString m_category = "";
    QString m_partyName = "";
    bool m_isRecurrent = false;
    QString m_recurrenceFrequency = "";
    QDate m_date = QDate();
    QDate m_nextExpenseDate = QDate();
    QString m_currencyCode = "";
    double m_total = 0.0;
    double m_plnTotal = 0.0;
};

#endif // EXPENSE_H
