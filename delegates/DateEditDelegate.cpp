#include "DateEditDelegate.h"

#include <QDateEdit>
#include <QDebug>

DateEditDelegate::DateEditDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *DateEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setCalendarPopup(true);
    editor->setDate(QDate::currentDate());
    editor->setDisplayFormat("d/M/yyyy");
    return editor;
}

void DateEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    dateEdit->setDate(index.model()->data(index).toDate());
}

void DateEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    model->setData(index, dateEdit->dateTime().date());
}

void DateEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

