// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef EXPENSESLISTWIDGET_H
#define EXPENSESLISTWIDGET_H

#include <QWidget>

namespace Ui {
class ExpensesListWidget;
}

class ExpensesModel;

/*!
 * \brief Class representing a widget with a table of expenses.
 */
class ExpensesListWidget : public QWidget
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     * \param ExpensesModel *model -- expenses model to use for displaying in the table.
     * \param QWidget *parent -- parent.
     */
    explicit ExpensesListWidget(ExpensesModel *model, QWidget *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~ExpensesListWidget();

    /*!
     * \brief Sets text for total amount label.
     * \param const QString &text -- text to set.
     */
    void updatePLNAmountLabel(const QString& text);

    /*!
     * \brief Resets state of 'hide' checkboxes and clears search line..
     */
    void resetCheckBoxesAndClearSearch();

signals:
    void filteringApplied(const QString &fitleringPattern, bool hideRecurrent, bool hideNormal);

private:
    Ui::ExpensesListWidget *ui;
    ExpensesModel *m_model = nullptr;
};

#endif // EXPENSESLISTWIDGET_H
