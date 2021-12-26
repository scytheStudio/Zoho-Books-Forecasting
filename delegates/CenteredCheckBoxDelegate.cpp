#include "CenteredCheckBoxDelegate.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

CenteredCheckBoxDelegate::CenteredCheckBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void CenteredCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    const QWidget *widget = option.widget;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);
    if (opt.features & QStyleOptionViewItem::HasCheckIndicator) {
        switch (opt.checkState) {
        case Qt::Unchecked:
            opt.state |= QStyle::State_Off;
            break;
        case Qt::PartiallyChecked:
            opt.state |= QStyle::State_NoChange;
            break;
        case Qt::Checked:
            opt.state |= QStyle::State_On;
            break;
        }
        auto rect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &opt, widget);
        opt.rect = QStyle::alignedRect(opt.direction, Qt::AlignCenter, rect.size(), opt.rect);
        opt.state = opt.state & ~QStyle::State_HasFocus;
        style->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &opt, painter, widget);
    } else if (!opt.icon.isNull()) {
        // draw the icon
        QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, widget);
        iconRect = QStyle::alignedRect(opt.direction, Qt::AlignCenter, iconRect.size(), opt.rect);
        QIcon::Mode mode = QIcon::Normal;
        if (!(opt.state & QStyle::State_Enabled))
            mode = QIcon::Disabled;
        else if (opt.state & QStyle::State_Selected)
            mode = QIcon::Selected;
        QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
        opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool CenteredCheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_ASSERT(event);
    Q_ASSERT(model);
    // make sure that the item is checkable
    Qt::ItemFlags flags = model->flags(index);
    if (!(flags & Qt::ItemIsUserCheckable) ||
        !(option.state & QStyle::State_Enabled) ||
        !(flags & Qt::ItemIsEnabled)) {
        return false;
    }
    // make sure that we have a check state
    QVariant value = index.data(Qt::CheckStateRole);
    if (!value.isValid()) {
        return false;
    }
    const QWidget *widget = option.widget;
    QStyle *style = option.widget ? widget->style() : QApplication::style();
    // make sure that we have the right event type
    if ((event->type() == QEvent::MouseButtonRelease) ||
        (event->type() == QEvent::MouseButtonDblClick) ||
        (event->type() == QEvent::MouseButtonPress)) {
        QStyleOptionViewItem viewOpt(option);
        initStyleOption(&viewOpt, index);
        QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOpt, widget);
        checkRect = QStyle::alignedRect(viewOpt.direction, Qt::AlignCenter, checkRect.size(), viewOpt.rect);
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() != Qt::LeftButton || !checkRect.contains(me->pos())) {
            return false;
        }
        if ((event->type() == QEvent::MouseButtonPress) ||
            (event->type() == QEvent::MouseButtonDblClick)) {
            return true;
        }
    } else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent *>(event)->key() != Qt::Key_Space &&
            static_cast<QKeyEvent *>(event)->key() != Qt::Key_Select) {
            return false;
        }
    } else {
        return false;
    }
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    if (flags & Qt::ItemIsUserTristate)
        state = ((Qt::CheckState)((state + 1) % 3));
    else {
        state = (state == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
    }

    return model->setData(index, state, Qt::CheckStateRole);
}
