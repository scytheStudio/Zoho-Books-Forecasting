#ifndef CENTEREDCHECKBOXDELEGATE_H
#define CENTEREDCHECKBOXDELEGATE_H

#include <QStyledItemDelegate>

/*!
 * \brief Delegate for centered checkbox in the forcasting table.
 */
class CenteredCheckBoxDelegate : public QStyledItemDelegate
{
public:

    /*!
     * \brief Constructor.
     * \param QObject *parent -- parent.
     */
    explicit CenteredCheckBoxDelegate(QObject *parent = nullptr);
    using QStyledItemDelegate::QStyledItemDelegate;

    /*!
     * \brief Draws the checkbox.
     * \param QPainter *painter -- painter.
     * \param const QStyleOptionViewItem &option -- option.
     * \param const QModelIndex &index -- index of the cell in the table.
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // CENTEREDCHECKBOXDELEGATE_H
