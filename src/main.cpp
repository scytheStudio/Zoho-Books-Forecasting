// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "MainWindow.h"
#include <QApplication>
#include <QChart>
#include <QMessageBox>
#include <QScreen>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Zoho Books Forecasting");
    a.setStyle("Fusion");


    MainWindow window;
    window.show();

    if (window.isDemoMode()) {
        QMessageBox::information(&window, "Zoho Books Forecasting - Demo mode",
                                    "To utilize all features of Zoho Books Forecasting "
                                    "application you need to generate a token in "
                                    "Zoho API console.\nSince the application is in demo mode by default, "
                                    "it uses artificial data to display the features of the application. "
                                    "This can be easily changed.");
    }

    return a.exec();
}
