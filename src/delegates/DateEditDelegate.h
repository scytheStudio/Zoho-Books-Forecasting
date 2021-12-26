#ifndef DATEEDITDELEGATE_H
#define DATEEDITDELEGATE_H

#include <QItemDelegate>

/*!
 * \brief Delegate for date edit in the forcasting table.
 */
class DateEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor.
     * \param QObject *parent -- parent.
     */
    DateEditDelegate(QObject *parent);

    /*!
     * \brief Creates editor widget.
     * \param QWidget *parent -- parent for date editor.
     * \param const QStyleOptionViewItem &option -- option.
     * \param const QModelIndex &index -- index of the cell in the table.
     */
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /*!
     * \brief Sets date to the date editor.
     * \param QWidget *editor -- date editor.
     * \param const QModelIndex &index -- index of the cell in the table.
     */
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    /*!
     * \brief Sets date to the model.
     * \param QWidget &editor -- date editor.
     * \param QAbstractItemMode *model -- model to set date to.
     * \param const QModelIndex &index -- index of the cell in the table.
     */
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    /*!
     * \brief Updates geometry of the widget.
     * \param QWidget *editor -- date editor.
     * \param const QStyleOptionViewItem &option -- option.
     * \param const QModelIndex &index -- index of the cell in the table.
     */
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DATEEDITDELEGATE_H
