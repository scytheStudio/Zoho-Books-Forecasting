// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef INVOICE_H
#define INVOICE_H

#include <QDate>
#include <QVariant>

/*!
 * \brief Class representing an invoice entity.
 */
class Invoice
{
public:

    /*!
     * \brief Constructor.
     */
    explicit Invoice();

    /*!
     * \brief Returns an invoice number.
     */
    QString invoiceNumber() const;

    /*!
     * \brief Returns a party the invoice is paid by.
     */
    QString party() const; //!<

    /*!
     * \brief Returns a status of the invoice (i.e. paid, overdue etc.).
     */
    QString status() const;

    /*!
     * \brief Returns a date of the invoice.
     */
    QDate date() const;

    /*!
     * \brief Returns a due date of the invoice.
     */
    QDate dueDate() const;

    /*!
     * \brief Returns a total amount of the bill (no conversion performed).
     */
    double total() const;

    /*!
     * \brief Returns a code of a currency the expense was paid in.
     */
    QString currencyCode() const;

    /*!
     * \brief Returns a total amount of the expense in Polish Zlote.
     */
    double plnTotal() const;

    /*!
     * \brief Sets the value of total amount of the expense in Polish Zlote.
     * \param const double &amount -- amount to set.
     */
    void setPlnTotal(const double &amount);

    /*!
     * \brief Deserializes QVariantMap to create an invoice.
     * \param const QVariantMap &map -- a map describing an invoice object.
     */
    static Invoice parseInvoice(const QVariantMap &map);

    /*!
     * \brief Deserializes QStringList to create an invoic.
     * \param const QStringList &list -- a list describing an invoice objects.
     */
    static Invoice parseInvoice(const QStringList &list);

private:
    QString m_invoiceNumber = "";
    QString m_party = "";
    QString m_status = "";
    QDate m_date = QDate();
    QDate m_dueDate = QDate();
    double m_total = 0.0;
    QString m_currencyCode = "PLN";
    double m_plnTotal = 0.0;
};

#endif // INVOICE_H
