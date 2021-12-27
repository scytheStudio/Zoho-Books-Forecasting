# <copyright company="Scythe Studio Sp. z o.o.">
#     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
#</copyright>

QT += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VERSION = 1.0.0

INCLUDEPATH += $$PWD

SOURCES += \
    MainWindow.cpp \
    datasets/Bill.cpp \
    datasets/Expense.cpp \
    datasets/Invoice.cpp \
    LogicController.cpp \
    MainWidget.cpp \
    Settings.cpp \
    WebClient.cpp \
    delegates/CenteredCheckBoxDelegate.cpp \
    delegates/DateEditDelegate.cpp \
    main.cpp \
    models/BillsModel.cpp \
    models/ExpensesModel.cpp \
    models/ForecastingModel.cpp \
    models/InvoicesModel.cpp \
    plotting/Callout.cpp \
    plotting/CashFlowChart.cpp \
    plotting/CashFlowView.cpp \
    widgets/AboutDialog.cpp \
    widgets/BillsListWidget.cpp \
    widgets/ExpensesListWidget.cpp \
    widgets/ForecastingWidget.cpp \
    widgets/InvoicesListWidget.cpp

HEADERS += \
    MainWindow.h \
    datasets/Bill.h \
    datasets/Expense.h \
    datasets/Invoice.h \
    LogicController.h \
    MainWidget.h \
    Settings.h \
    WebClient.h \
    delegates/CenteredCheckBoxDelegate.h \
    delegates/DateEditDelegate.h \
    models/BillsModel.h \
    models/ExpensesModel.h \
    models/ForecastingModel.h \
    models/InvoicesModel.h \
    plotting/Callout.h \
    plotting/CashFlowChart.h \
    plotting/CashFlowView.h \
    widgets/AboutDialog.h \
    widgets/BillsListWidget.h \
    widgets/ExpensesListWidget.h \
    widgets/ForecastingWidget.h \
    widgets/InvoicesListWidget.h

FORMS += \
    MainWidget.ui \
    MainWindow.ui \
    widgets/AboutDialog.ui \
    widgets/BillsListWidget.ui \
    widgets/ExpensesListWidget.ui \
    widgets/ForecastingWidget.ui \
    widgets/InvoicesListWidget.ui

RESOURCES += \
    assets.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx {
QMAKE_INFO_PLIST = platforms/macos/Info.plist
ICON = platforms/macos/AppIcon.icns
}
