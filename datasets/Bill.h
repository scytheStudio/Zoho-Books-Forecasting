// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef BILL_H
#define BILL_H

#include <QDate>
#include <QVariant>

/*!
 * \brief Class representing a bill entity.
 */
class Bill
{
public:

    /*!
     * \brief Constructor.
     */
    explicit Bill();

    /*!
     * \brief Returns a bill number.
     */
    QString billNumber() const;

    /*!
     * \brief Returns a party the bill was paid to.
     */
    QString party() const;

    /*!
     * \brief Returns true if the bill is recurrent. Otherwise returns false.
     */
    bool isRecurrent() const;

    /*!
     * \brief Returns a status of the bill (i.e. paid, overdue etc.).
     */
    QString status() const;

    /*!
     * \brief Returns a recurrence frequency of the bill (i.e. weeks, months etc.).
     */
    QString recurrence_frequency() const;

    /*!
     * \brief Returns a date of the bill.
     */
    QDate date() const;

    /*!
     * \brief Returns a due date of the bill.
     */
    QDate dueDate() const;

    /*!
     * \brief Returns a next bill date of the bill.
     */
    QDate nextBillDate() const;

    /*!
     * \brief Returns a total amount of the bill (no conversion performed).
     */
    double total() const;

    /*!
     * \brief Returns a code of a currency the bill was paid in.
     */
    QString currencyCode() const;

    /*!
     * \brief Returns a total amount of the bill in Polish Zlote.
     */
    double plnTotal() const;

    /*!
     * \brief Sets the total amount of the bill in Polish Zlote.
     * \param const double &amount -- amount to set.
     */
    void setPlnTotal(const double &amount);

    /*!
     * \brief Deserializes QVariantMap to create a bill.
     * \param const QVariantMap &map -- a map describing a bill object.
     */
    static Bill parseNormalBill(const QVariantMap &map);

    static Bill parseRecurringBill(const QVariantMap &map);

    /*!
     * \brief Deserializes QStringList to create a normal (non-recurring) bill.
     * \param const QStringList &list -- a list describing a bill objects.
     */
    static Bill parseNormalBill(const QStringList &list);

    /*!
     * \brief Deserializes QStringList to create a recurring bill.
     * \param const QStringList &list -- a list describing a bill objects.
     */
    static Bill parseRecurringBill(const QStringList &list);

private:
    QString m_billNumber = "";
    QString m_party = "";
    bool m_isRecurrent = false;
    QString m_status = "";
    QString m_recurrence_frequency = "";
    QDate m_date = QDate();
    QDate m_dueDate = QDate();
    QDate m_nextBillDate = QDate();
    double m_total = 0.0;
    QString m_currencyCode = "";
    QString m_currencySymbol = "";
    double m_plnTotal = 0.0;
};

#endif // BILL_H
