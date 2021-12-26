// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef CASHFLOWVIEW_H
#define CASHFLOWVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include "CashFlowChart.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

class Callout;

QT_CHARTS_USE_NAMESPACE

/*!
 * \brief Class representing a cashflow view the chart is attached to.
 */
class CashFlowView: public QGraphicsView
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param QWidget *parent -- parent.
     */
    CashFlowView(QWidget *parent = nullptr);

    /*!
     * \brief Returns an attached chart.
     */
    CashFlowChart *chart() const;

    /*!
     * \brief Sets the chart.
     * \param CashFlowChart *chart -- chart to set.
     */
    void setChart(CashFlowChart *chart);

public slots:

    /*!
     * \brief Saves current callout.
     */
    void keepCallout();

    /*!
     * \brief Creates a new callout if none exists. Otherwise hides or alters current.
     * \param const QPointF &point -- point where tooltip is displayed.
     * \param bool state -- state.
     */
    void tooltip(const QPointF &point, bool state);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    CashFlowChart *m_chart;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;
};

#endif
