// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef FORECASTINGWIDGET_H
#define FORECASTINGWIDGET_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class ForecastingWidget;
}

class ForecastingModel;

/*!
 * \brief Class representing a widget with a table of forecasts.
 */

class ForecastingWidget : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor.
     * \param ForecastingModel *model -- forecasting model to use for displaying in the table.
     * \param QWidget *parent -- parent.
     */
    explicit ForecastingWidget(ForecastingModel *model, QWidget *parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~ForecastingWidget();

    /*!
     * \brief Returns pointer to checkbox responsible for enabling and disabling forecasting.
     */
    QCheckBox *enableForecastingCheckBox();

public slots:

    /*!
     * \brief Sets enability of the table of forecasts.
     * \param bool value -- value to set.
     */
    void setForecastingTableEnabled(bool value);

    /*!
     * \brief Sets enability of all buttons on the widget.
     * \param bool value -- value to set.
     */
    void setButtonsEnabled(bool value);

private slots:
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void onClearButtonClicked();

private:
    Ui::ForecastingWidget *ui;
    ForecastingModel *m_model = nullptr;

};

#endif // FORECASTINGWIDGET_H
