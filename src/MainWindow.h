// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWidget;
class LogicController;

/*!
 * \brief Class representing a main window which is displayed to user.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param QWidget *parent -- parent.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~MainWindow();

    /*!
     * \brief Returns current mode of the application (demo or normal mode).
     */
    bool isDemoMode() const;

private slots:
    void setupMenu();

    void onAboutActionTriggered();
    void onEnableDemoModeActionTriggered();
    void onDisableDemoModeActionTriggered();

private:
    Ui::MainWindow *ui;
    MainWidget *m_mainWidget = nullptr;
    LogicController *m_logicController = nullptr;

    QMenu *m_helpMenu = nullptr;
    QAction *m_aboutAction = nullptr;
    QAction *m_disableDemoModeAction = nullptr;
    QAction *m_enableDemoModeAction = nullptr;
};

#endif // MAINWINDOW_H
