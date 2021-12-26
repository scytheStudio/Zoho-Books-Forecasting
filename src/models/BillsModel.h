// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef BILLSMODEL_H
#define BILLSMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

class LogicController;

/*!
 * \brief Class representing a wrapper for sorting and filtering functionality of the bills model.
 */
class BillsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param QObject *parent -- parent.
     */
    explicit BillsProxyModel(QObject *parent = nullptr);

    /*!
     * \brief Functionality for filtering bills.
     * \param int sourceRow -- row.
     * \param QModelIndex &sourceParent -- source parent.
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

protected:
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

public slots:

    /*!
     * \brief Sets filtering pattern.
     * \param const QString &pattern -- pattern to use for filtering.
     */
    void setFilteringPattern(const QString &pattern);

    /*!
     * \brief Sets if normal bills have to be hidden.
     * \param bool value -- value to set.
     */
    void setHideNormal(bool value);

    /*!
     * \brief Sets if recurring bills have to be hidden.
     * \param bool value -- value to set.
     */
    void setHideRecurring(bool value);

private:
    QRegExp m_filteringRegExp;
    bool m_hideNormal = false;
    bool m_hideRecurring = false;
};

class Bill;

/*!
 * \brief Class representing a bills model.
 */
class BillsModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param LogicController *logicController -- logic controller responsible for manipulating different parts of the application.
     * \param QObject *parent -- parent.
     */
    explicit BillsModel(LogicController *logicController, QObject *parent = nullptr);

    /*!
     * \brief Enum representing a column of the table displaying bills.
     */
    enum Column {
        NumberColumn,
        PartyColumn,
        RecurrentColumn,
        StatusColumn,
        RecurrenceFrequency,
        DateColumn,
        DueDateColumn,
        NextBillDate,
        TotalColumn,
        PlnTotalColumn,
        ColumnCount
    };

    /*!
     * \brief Returns number of rows of the table the model is used for.
     * \param const QModelIndex &parent -- model index to use for getting row count.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Returns number of columns of the table the model is used for.
     * \param const QModelIndex &parent -- model index to use for getting column count.
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Returns data of the table at the given index and with the given role.
     * \param const QModelIndex &index -- index of a cell in the bills table.
     * \param int role -- role of a cell in the bills table.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /*!
     * \brief Returns column name of the specified section.
     * \param int section -- header section.
     * \param Qt::Orientation orientation -- orientation.
     * \param int role -- role of a cell in the bills table.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /*!
     * \brief Returns a list of bills between set 'from date' and 'to date'.
     */
    QList<Bill> rangedBills() const;

public slots:

    /*!
     * \brief Resets the model.
     */
    void loadData();

signals:

private:
    LogicController *m_logicController = nullptr;
};

#endif // BILLSMODEL_H
