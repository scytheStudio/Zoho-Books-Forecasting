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
    a.setStyle("Fusion");
    MainWindow window;

    auto screen = a.screens().at(0);
    int width = 1280;
    int height = 720;
    int xPos = (screen->geometry().width()-width)/2;
    int yPos = (screen->geometry().height()-height)/2;

    window.setGeometry(xPos, yPos, width, height);
    window.show();
    if (window.isDemoMode()) {
        QMessageBox::information(&window, "Zoho Forecasting - Demo mode",
                                    "To utilize all features of Zoho Forecasting "
                                    "application you need to generate a token in "
                                    "Zoho API console.\nSince the application is in demo mode by default, "
                                    "it uses artificial data to display the features of the application. "
                                    "This can be easily changed.");
    }

    return a.exec();
}
