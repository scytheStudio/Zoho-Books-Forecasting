// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "LogicController.h"
#include <QPixmap>

AboutDialog::AboutDialog(LogicController *logicController, QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::AboutDialog),
    m_logicController(logicController)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &AboutDialog::onCloseButtonClicked);
    setWindowTitle("About");
    setWindowIcon(QIcon(":/assets/logo.png"));

    if (m_logicController->isDarkMode()) {
        ui->builtWithQtLabel->setPixmap(QPixmap(":/assets/builtWithQtWhite.png"));
    } else {
        ui->builtWithQtLabel->setPixmap(QPixmap(":/assets/buildWithQt.png"));
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::onCloseButtonClicked()
{
    close();
    deleteLater();
}
