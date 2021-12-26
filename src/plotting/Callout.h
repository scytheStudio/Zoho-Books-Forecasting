// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef CALLOUT_H
#define CALLOUT_H

#include "LogicController.h"
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

/*!
 * \brief Class representing a callout displayed after hovering over some series.
 */
class Callout : public QGraphicsItem
{
public:

    /*!
     * \brief Constructor.
     * \param QChart *parent -- parent.
     */
    Callout(QChart *parent);

    /*!
     * \brief Sets text of the callout.
     * \param const QString &text -- text to set.
     */
    void setText(const QString &text);

    /*!
     * \brief Sets anchor of the callout.
     * \param QPointF point -- point to use as an anchor.
     */
    void setAnchor(QPointF point);

    /*!
     * \brief Updates geometry of the callout.
     */
    void updateGeometry();

    /*!
     * \brief Returns a rectangle bouding the callout.
     */
    QRectF boundingRect() const;

    /*!
     * \brief Paints the callout.
     * \param QPainter *painter -- painter.
     * \param const QStyleOptionGraphicsItem *option -- graphics option.
     * \param Widget *widget -- widget.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_text;
    QRectF m_textRect;
    QRectF m_rect;
    QPointF m_anchor;
    QFont m_font;
    QChart *m_chart;
};

#endif // CALLOUT_H
