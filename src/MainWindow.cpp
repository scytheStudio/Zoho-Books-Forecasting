// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "MainWindow.h"
#include "MainWidget.h"
#include "LogicController.h"
#include "ui_MainWindow.h"
#include "widgets/AboutDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_logicController(new LogicController(this))
{
    ui->setupUi(this);

    m_mainWidget = new MainWidget(m_logicController, this);
    setCentralWidget(m_mainWidget);
    setWindowIcon(QIcon(":/assets/logo.png"));

    if (m_logicController->isDemoMode()) {
        setWindowTitle("Zoho Forecasting - Demo mode");
    } else {
        setWindowTitle("Zoho Forecasting");
    }

    setupMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenu()
{
    m_helpMenu = menuBar()->addMenu(tr("&Help"));

    m_aboutAction = new QAction(tr("&About"), this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);

    m_disableDemoModeAction = new QAction(tr("&Disable demo mode"), this);
    connect(m_disableDemoModeAction, &QAction::triggered, this, &MainWindow::onDisableDemoModeActionTriggered);

    m_enableDemoModeAction = new QAction(tr("&Enable demo mode"), this);
    connect(m_enableDemoModeAction, &QAction::triggered, this, &MainWindow::onEnableDemoModeActionTriggered);

    m_helpMenu->addAction(m_aboutAction);

    if (m_logicController->isDemoMode()) {
        m_helpMenu->addAction(m_disableDemoModeAction);
    } else {
        m_helpMenu->addAction(m_enableDemoModeAction);
    }
}

void MainWindow::onAboutActionTriggered()
{
    AboutDialog aboutDialog(m_logicController, this);
    aboutDialog.setFixedSize(aboutDialog.width(), aboutDialog.height());
    aboutDialog.exec();
}

void MainWindow::onEnableDemoModeActionTriggered()
{
    m_logicController->setIsDemoMode(true);
    m_helpMenu->removeAction(m_enableDemoModeAction);
    m_helpMenu->addAction(m_disableDemoModeAction);
    setWindowTitle("Zoho Forecasting - Demo mode");
}

void MainWindow::onDisableDemoModeActionTriggered()
{
    m_logicController->setIsDemoMode(false);
    m_helpMenu->removeAction(m_disableDemoModeAction);
    m_helpMenu->addAction(m_enableDemoModeAction);
    setWindowTitle("Zoho Forecasting");
}

bool MainWindow::isDemoMode() const
{
    return m_logicController->isDemoMode();
}
