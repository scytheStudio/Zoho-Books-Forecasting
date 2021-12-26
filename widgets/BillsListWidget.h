// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef BILLSLISTWIDGET_H
#define BILLSLISTWIDGET_H

#include <QWidget>

namespace Ui {
class BillsListWidget;
}

class BillsModel;

/*!
 * \brief Class representing a widget with a table of bills.
 */
class BillsListWidget : public QWidget
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     * \param BillsModel *model -- bills model to use for displaying in the table.
     * \param QWidget *parent -- parent.
     */
    explicit BillsListWidget(BillsModel *model, QWidget *parent);

    /*!
     * \brief Destructor.
     */
    ~BillsListWidget();

    /*!
     * \brief Sets text for total amount label.
     * \param const QString &text -- text to set.
     */
    void updatePLNAmountLabel(const QString &text);

    /*!
     * \brief Resets state of 'hide' checkboxes and clears search line..
     */
    void resetCheckBoxesAndClearSearch();

signals:
    void filteringApplied(const QString &fitleringPattern, bool hideRecurrent, bool hideNormal);

private:
    Ui::BillsListWidget *ui;
    BillsModel *m_model = nullptr;
};

#endif // BILLSLISTWIDGET_H
