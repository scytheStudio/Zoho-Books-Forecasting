// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef INVOICESLISTWIDGET_H
#define INVOICESLISTWIDGET_H

#include <QWidget>

namespace Ui {
class InvoicesListWidget;
}

class InvoicesModel;

/*!
 * \brief Class representing a widget with a table of invoices.
 */
class InvoicesListWidget : public QWidget
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param InvoicesModel *model -- invoices model to use for displaying in the table.
     * \param QWidget *parent -- parent.
     */
    explicit InvoicesListWidget(InvoicesModel *model, QWidget *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~InvoicesListWidget();

    /*!
     * \brief Sets text for total amount label.
     * \param const QString &text -- text to set.
     */
    void updatePLNAmountLabel(const QString& text);

    /*!
     * \brief Clears search line.
     */
    void clearSearchLine();

signals:
    void filteringApplied(const QString &fitleringPattern);

private:
    Ui::InvoicesListWidget *ui;
    InvoicesModel *m_model = nullptr;
};

#endif // INVOICESLISTWIDGET_H
