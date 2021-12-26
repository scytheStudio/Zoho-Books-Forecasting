// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include <cmath>
#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "LogicController.h"
#include "models/InvoicesModel.h"
#include "models/BillsModel.h"
#include "models/ExpensesModel.h"
#include "models/ForecastingModel.h"
#include "plotting/CashFlowChart.h"
#include "widgets/InvoicesListWidget.h"
#include "widgets/BillsListWidget.h"
#include "widgets/ExpensesListWidget.h"
#include "widgets/ForecastingWidget.h"
#include <QStyle>

MainWidget::MainWidget(LogicController *logicController, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , m_logicController(logicController)
    , m_chart(new CashFlowChart(m_logicController))
    , m_invoicesModel(new InvoicesModel(m_logicController, this))
    , m_billsModel(new BillsModel(m_logicController, this))
    , m_expensesModel(new ExpensesModel(m_logicController, this))
    , m_forecastingModel(new ForecastingModel(m_logicController, this))
{
    ui->setupUi(this);

    ui->chartView->setChart(m_chart);

    ui->errorLabel->setVisible(false);
    ui->datesErrorLabel->setVisible(false);
    ui->updateButton->setEnabled(false);

    ui->expensesCheckBox->setCheckable(false);
    ui->incomesCheckBox->setCheckable(false);
    ui->cashFlowCheckBox->setCheckable(false);

    ui->expensesPointsCheckBox->setCheckable(false);
    ui->incomePointsCheckBox->setCheckable(false);
    ui->cashFlowPointsCheckBox->setCheckable(false);

    connect(ui->updateButton, &QPushButton::clicked, this, &MainWidget::onUpdateButtonClicked);
    connect(ui->grantTokenButton, &QPushButton::clicked, this, &MainWidget::onGrantTokenButtonClicked);

    connect(m_logicController, &LogicController::errorLabelVisibilityRequested,
            this, &MainWidget::onErrorLabelVisibilityRequested);

    connect(m_logicController, &LogicController::invoicesReady, m_invoicesModel, &InvoicesModel::loadData);
    connect(m_logicController, &LogicController::billsReady, m_billsModel, &BillsModel::loadData);
    connect(m_logicController, &LogicController::expensesReady, m_expensesModel, &ExpensesModel::loadData);

    connect(m_logicController, &LogicController::invoicesReady, this, [&] {
        updateInvoicesTotalLabel("");
    });
    connect(m_logicController, &LogicController::expensesReady, this, [&] {
        updateExpensesTotalLabel("", false, false);
    });
    connect(m_logicController, &LogicController::billsReady, this, [&] {
        updateBillsTotalLabel("", false, false);
    });

    connect(m_logicController, &LogicController::invoicesReady, this, &MainWidget::updateIncomeSeries);
    connect(m_logicController, &LogicController::expensesReady, this, &MainWidget::expensesArrived);
    connect(m_logicController, &LogicController::billsReady, this, &MainWidget::billsArrived);

    connect(ui->incomesCheckBox, &QCheckBox::stateChanged, m_chart, &CashFlowChart::setIncomeSeriesVisible);
    connect(ui->expensesCheckBox, &QCheckBox::stateChanged, m_chart, &CashFlowChart::setExpensesSeriesVisible);
    connect(ui->cashFlowCheckBox, &QCheckBox::stateChanged, m_chart, &CashFlowChart::setCashFlowSeriesVisisble);

    connect(ui->incomePointsCheckBox, &QCheckBox::stateChanged, m_chart, &CashFlowChart::setIncomePointsVisible);
    connect(ui->expensesPointsCheckBox, &QCheckBox::stateChanged, m_chart, &CashFlowChart::setExpensesPointsVisible);
    connect(ui->cashFlowPointsCheckBox, &QCheckBox::stateChanged, m_chart, &CashFlowChart::setCashFlowPointsVisible);

    connect(m_forecastingModel, &ForecastingModel::modelChanged, this, &MainWidget::updateChart);

    connect(m_chart, &CashFlowChart::axesPrepared, this, [&](){
       ui->updateButton->setEnabled(true);
    });

    connect(m_logicController, &LogicController::exchangeRateSReceived, this, [&](){
       ui->updateButton->setEnabled(true);
       ui->ratesWaitingLabel->setVisible(false);
    });

    connect(m_logicController, &LogicController::modeChanged, this, &MainWidget::onModeChanged);

    setupDisplayWidgets();

    connect(ui->themeButton, &QPushButton::clicked, this, &MainWidget::onThemeButtonClicked);
    setDarkMode(m_logicController->isDarkMode()); // set visual mode of the application.
    
    connect(m_forecastingWidget->enableForecastingCheckBox(), &QCheckBox::stateChanged, this, [=](int value){
        m_logicController->setForecastingEnabled(value);
        m_forecastingWidget->setForecastingTableEnabled(value);
        m_forecastingWidget->setButtonsEnabled(value);
        updateChart();
    });

    onModeChanged(); // sets a lot of settings according to the current mode of the application.
}

MainWidget::~MainWidget()
{
    delete ui;
}

LogicController *MainWidget::logicController()
{
    return m_logicController;
}

void MainWidget::onUpdateButtonClicked()
{
    ui->datesErrorLabel->setVisible(false);
    if (ui->fromDateEdit->date() >= ui->toDateEdit->date()) {
        ui->datesErrorLabel->setVisible(true);
        return;
    }
    ui->updateButton->setEnabled(false);
    m_logicController->setFirstDate(ui->fromDateEdit->date());
    m_logicController->setLastDate(ui->toDateEdit->date());
    m_logicController->clearContainers();
    m_logicController->setFromDate(ui->fromDateEdit->date());
    m_logicController->setToDate(ui->toDateEdit->date());
    ui->chartView->chart()->setVisible(true);
    resetCheckBoxesToDefault();
    m_chart->setSeriesVisible(false);
    m_chart->resetYAxeRanges();
    m_chart->setDates(ui->fromDateEdit->date(), ui->toDateEdit->date());
    m_logicController->setRequestMade(true);
    if (m_logicController->isDemoMode()) {
        // read files with mock-data.
        m_logicController->readFiles();
    } else {
        // make request for the data.
        m_logicController->requestAllData();
    }
    // REQUEST MADE parameter is used for upcoming requests.
    m_billsListWidget->resetCheckBoxesAndClearSearch();
    m_expensesListWidget->resetCheckBoxesAndClearSearch();
    m_invoicesListWidget->clearSearchLine();
}

void MainWidget::onGrantTokenButtonClicked()
{
    m_logicController->requestToken(ui->grantTokenInput->text());
}

void MainWidget::onErrorLabelVisibilityRequested(bool visible)
{
    ui->errorLabel->setVisible(visible);
}

void MainWidget::updateChart()
{
    // chart cannot be updated if no requests have been made.
    if (m_logicController->requestMade()) {
        resetCheckBoxesToDefault();
        m_chart->setSeriesVisible(false);
        m_chart->resetYAxeRanges();
        m_chart->prepareIncomeSeries(m_logicController->invoices(), m_logicController->forecasts());
        m_chart->prepareExpensesSeries(m_logicController->expenses(), m_logicController->bills(), m_logicController->forecasts());
    }
}

void MainWidget::setupDisplayWidgets()
{
    m_invoicesListWidget = new InvoicesListWidget(m_invoicesModel, this);
    ui->tabWidget->addTab(m_invoicesListWidget, "Invoices");
    m_billsListWidget = new BillsListWidget(m_billsModel, this);
    ui->tabWidget->addTab(m_billsListWidget, "Bills");
    m_expensesListWidget = new ExpensesListWidget(m_expensesModel, this);
    ui->tabWidget->addTab(m_expensesListWidget, "Expenses");
    m_forecastingWidget = new ForecastingWidget(m_forecastingModel, this);
    ui->tabWidget->addTab(m_forecastingWidget, "Forecasting");

    connect(m_billsListWidget, &BillsListWidget::filteringApplied, this, &MainWidget::updateBillsTotalLabel);
    connect(m_invoicesListWidget, &InvoicesListWidget::filteringApplied, this, &MainWidget::updateInvoicesTotalLabel);
    connect(m_expensesListWidget, &ExpensesListWidget::filteringApplied, this, &MainWidget::updateExpensesTotalLabel);
}

void MainWidget::updateInvoicesTotalLabel(const QString &text)
{
    double sum = 0;
    for (int i = 0; i < m_logicController->rangedInvoices().size(); ++i) {
        if (!text.isEmpty()) {
            if (m_logicController->rangedInvoices().at(i).invoiceNumber().toLower().contains(text.toLower()) ||
                m_logicController->rangedInvoices().at(i).party().toLower().contains(text.toLower()) ||
                m_logicController->rangedInvoices().at(i).status().toLower().contains(text.toLower()) ||
                m_logicController->rangedInvoices().at(i).date().toString().contains(text.toLower()) ||
                m_logicController->rangedInvoices().at(i).dueDate().toString().contains(text.toLower()) ||
                m_logicController->rangedInvoices().at(i).currencyCode().contains(text.toLower()) ||
                QString::number(m_logicController->rangedInvoices().at(i).plnTotal()).toLower().contains(text.toLower())){
                    sum += m_logicController->rangedInvoices().at(i).plnTotal();
            } else {
                continue;
            }
        } else {
            sum += m_logicController->rangedInvoices().at(i).plnTotal();
        }
    }
    QString stringSum = std::to_string(std::round(sum * 100.0) / 100.0).c_str();
    stringSum.truncate(stringSum.lastIndexOf('.') + 3);
    m_invoicesListWidget->updatePLNAmountLabel(stringSum + QString(" PLN"));
}

void MainWidget::updateExpensesTotalLabel(const QString &text, bool hideNormal, bool hideRecurrent)
{
    double sum = 0;
    for (int i = 0; i != m_logicController->rangedExpenses().size(); ++i) {
        if (hideRecurrent && m_logicController->rangedExpenses().at(i).isRecurrent()) {
            continue;
        }

        if (hideNormal && !m_logicController->rangedExpenses().at(i).isRecurrent()) {
            continue;
        }

        if (!text.isEmpty()) {
            if (m_logicController->rangedExpenses().at(i).expenseId().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).status().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).category().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).partyName().toLower().contains(text.toLower()) ||
                QVariant(m_logicController->rangedExpenses().at(i).isRecurrent()).toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).recurrenceFrequency().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).date().toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).nextExpenseDate().toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedExpenses().at(i).currencyCode().toLower().contains(text.toLower()) ||
                QString::number(m_logicController->rangedExpenses().at(i).plnTotal()).toLower().contains(text.toLower())) {
                    sum += m_logicController->rangedExpenses().at(i).plnTotal();
            } else {
                continue;
            }
        } else {
            sum += m_logicController->rangedExpenses().at(i).plnTotal();
        }
    }
    QString stringSum = std::to_string(std::round(sum * 100.0) / 100.0).c_str();
    stringSum.truncate(stringSum.lastIndexOf('.') + 3);
    m_expensesListWidget->updatePLNAmountLabel(stringSum + QString(" PLN"));
}

void MainWidget::updateBillsTotalLabel(const QString &text, bool hideRecurrent, bool hideNormal)
{
    double sum = 0;
    for (int i = 0; i != m_logicController->rangedBills().size(); ++i) {
        if (hideRecurrent && m_logicController->rangedBills().at(i).isRecurrent()) {
            continue;
        }

        if (hideNormal && !m_logicController->rangedBills().at(i).isRecurrent()) {
            continue;
        }

        if (!text.isEmpty()) {
            if (m_logicController->rangedBills().at(i).billNumber().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).party().toLower().contains(text.toLower()) ||
                QVariant(m_logicController->rangedBills().at(i).isRecurrent()).toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).status().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).recurrence_frequency().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).date().toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).dueDate().toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).nextBillDate().toString().toLower().contains(text.toLower()) ||
                m_logicController->rangedBills().at(i).currencyCode().toLower().contains(text.toLower()) ||
                QString::number(m_logicController->rangedBills().at(i).plnTotal()).toLower().contains(text.toLower())) {
                    sum += m_logicController->rangedBills().at(i).plnTotal();
            } else {
                continue;
            }
        } else {
            sum += m_logicController->rangedBills().at(i).plnTotal();
        }
    }
    QString stringSum = std::to_string(std::round(sum * 100.0) / 100.0).c_str();
    stringSum.truncate(stringSum.lastIndexOf('.') + 3);
    m_billsListWidget->updatePLNAmountLabel(stringSum + QString(" PLN"));
}

void MainWidget::updateIncomeSeries()
{
    m_chart->prepareIncomeSeries(m_logicController->invoices(), m_logicController->forecasts());
}

void MainWidget::updateExpensesSeries()
{
    m_chart->prepareExpensesSeries(m_logicController->expenses(), m_logicController->bills(), m_logicController->forecasts());
}

void MainWidget::resetCheckBoxesToDefault()
{
    ui->expensesCheckBox->setCheckable(true);
    ui->incomesCheckBox->setCheckable(true);
    ui->cashFlowCheckBox->setCheckable(true);
    ui->expensesCheckBox->setChecked(true);
    ui->incomesCheckBox->setChecked(true);
    ui->cashFlowCheckBox->setChecked(true);
    ui->expensesPointsCheckBox->setCheckable(true);
    ui->incomePointsCheckBox->setCheckable(true);
    ui->cashFlowPointsCheckBox->setCheckable(true);
    ui->expensesPointsCheckBox->setChecked(false);
    ui->incomePointsCheckBox->setChecked(false);
    ui->cashFlowPointsCheckBox->setChecked(false);
}

void MainWidget::billsArrived()
{
    // we check for bills and expenses to arrive both, becasuse they are the data for expenses series.
    billsReady = true;
    if (billsReady && expensesReady) {
        updateExpensesSeries();
        billsReady = false;
        expensesReady = false;
    }
}

void MainWidget::expensesArrived()
{
    // we check for bills and expenses to arrive both, becasuse they are the data for expenses series.
    expensesReady = true;
    if (billsReady && expensesReady) {
        updateExpensesSeries();
        billsReady = false;
        expensesReady = false;
    }
}

void MainWidget::onThemeButtonClicked()
{
    setDarkMode(!m_logicController->isDarkMode());
}

void MainWidget::setDarkMode(bool isDarkMode)
{
    if (isDarkMode) {
        QPalette palette;
        QColor color(45, 45, 45);
        QColor disabledColor(127, 127, 127);
        palette.setColor(QPalette::Window, color);
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(18,18,18));
        palette.setColor(QPalette::AlternateBase, color);
        palette.setColor(QPalette::ToolTipBase, Qt::black);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
        palette.setColor(QPalette::Button, color);
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
        palette.setColor(QPalette::BrightText, Qt::white);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));

        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);
        ui->themeButton->setIcon(QIcon(":/assets/moon.png"));
        m_chart->setTheme(QChart::ChartTheme::ChartThemeDark);
        qApp->setPalette(palette);
    } else {
        ui->themeButton->setIcon(QIcon(":/assets/sun.png"));
        m_chart->setTheme(QChart::ChartTheme::ChartThemeLight);
        qApp->setPalette(qApp->style()->standardPalette());
    }

    m_logicController->setIsDarkMode(isDarkMode);
}

void MainWidget::onModeChanged()
{
    ui->updateButton->setEnabled(false);
    ui->ratesWaitingLabel->setVisible(true);
    ui->errorLabel->setVisible(!m_logicController->isDemoMode());
    ui->grantTokenButton->setVisible(!m_logicController->isDemoMode());
    ui->grantTokenInput->setVisible(!m_logicController->isDemoMode());

    m_logicController->clearContainers();
    m_logicController->forecasts().clear();
    m_logicController->setRequestMade(false);
    ui->chartView->chart()->setVisible(false);

    if (m_logicController->isDemoMode()) {
        ui->fromDateEdit->setDate(QDate(2015, 6, 1));
        ui->toDateEdit->setDate(QDate(2016, 03, 31));
        m_logicController->prepareFakeRates();
    } else {
        ui->fromDateEdit->setDate(QDate::currentDate().addYears(-1));
        ui->toDateEdit->setDate(QDate(QDate::currentDate().year(), 12, 31));
        m_logicController->clearExchangeRates();
        m_logicController->checkAccessToken();
    }

    // clearing models and tables.
    m_billsModel->loadData();
    m_invoicesModel->loadData();
    m_expensesModel->loadData();
    m_forecastingModel->loadData();
}
