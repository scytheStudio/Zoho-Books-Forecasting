// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#include "CashFlowView.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include "Callout.h"

CashFlowView::CashFlowView(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_chart(nullptr),
      m_tooltip(nullptr)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    this->setMouseTracking(true);
}

void CashFlowView::resizeEvent(QResizeEvent *event)
{
    if (m_chart && scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        m_chart->resize(event->size());
        const auto callouts = m_callouts;
        for (Callout *callout : callouts) {
            callout->updateGeometry();
        }
    }

    QGraphicsView::resizeEvent(event);
}

void CashFlowView::setChart(CashFlowChart *chart)
{
    m_chart = chart;
    scene()->addItem(m_chart);

    connect(m_chart, &CashFlowChart::seriesHovered, this, &CashFlowView::tooltip);
}

void CashFlowView::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void CashFlowView::tooltip(const QPointF &point, bool state)
{
    if (m_tooltip == 0) {
        m_tooltip = new Callout(m_chart);
    }

    if (state) {
        const QString date = QDateTime::fromMSecsSinceEpoch(point.x()).date().toString();

        m_tooltip->setText(QString("Date: %1 \nValue: %2 ").arg(date).arg(QString::number(point.y(), 'f', 2)));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

CashFlowChart *CashFlowView::chart() const
{
    return m_chart;
}
