// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class CashFlowChart;
class LogicController;

class InvoicesListWidget;
class InvoicesModel;

class BillsListWidget;
class BillsModel;

class ExpensesListWidget;
class ExpensesModel;

class ForecastingWidget;
class ForecastingModel;

/*!
 * \brief Class representing a main widget with all the main elements.
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param LogicController *logicController -- logic controller responsible for manipulating different parts of the application.
     * \param QWidget *parent -- parent.
     */
    MainWidget(LogicController *logicController, QWidget *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~MainWidget();

    /*!
     * \brief Returns pointer to the logic controller.
     */
    LogicController *logicController();

private slots:
    void onUpdateButtonClicked();
    void onGrantTokenButtonClicked();
    void onErrorLabelVisibilityRequested(bool visible);
    void updateChart();
    void onThemeButtonClicked();
    void onModeChanged();

private:
    bool billsReady = false;
    bool expensesReady = false;
    void billsArrived();
    void expensesArrived();

    void setupDisplayWidgets();

    void updateInvoicesTotalLabel(const QString &text);
    void updateExpensesTotalLabel(const QString &text, bool hideRecurrent, bool hideNormal);
    void updateBillsTotalLabel(const QString &text, bool hideRecurrent, bool hideNormal);

    void updateIncomeSeries();
    void updateExpensesSeries();

    void resetCheckBoxesToDefault();
    void setDarkMode(bool isDarkMode);

    Ui::MainWidget *ui = nullptr;
    LogicController *m_logicController = nullptr;
    CashFlowChart *m_chart = nullptr;

    // ui elements
    InvoicesListWidget *m_invoicesListWidget = nullptr;
    BillsListWidget *m_billsListWidget = nullptr;
    ExpensesListWidget *m_expensesListWidget = nullptr;
    ForecastingWidget *m_forecastingWidget = nullptr;

    // models
    InvoicesModel *m_invoicesModel = nullptr;
    BillsModel *m_billsModel = nullptr;
    ExpensesModel *m_expensesModel = nullptr;
    ForecastingModel *m_forecastingModel = nullptr;
};

#endif // MAINWIDGET_H
