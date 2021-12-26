// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class LogicController;

/*!
 * \brief Class representing an about dialog.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:

    /*!
     * \brief AboutDialog\param LogicController *logicController -- logic controller responsible for manipulating different parts of the application.
     * \param QWidget *parent -- parent.
     */
    explicit AboutDialog(LogicController *logicController, QWidget *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~AboutDialog();

public slots:

    /*!
     * \brief Closes and deletes the dialog.
     */
    void onCloseButtonClicked();

private:
    Ui::AboutDialog *ui;
    LogicController *m_logicController;
};

#endif // ABOUTDIALOG_H
