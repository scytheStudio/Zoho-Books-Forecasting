// <copyright company="Scythe Studio Sp. z o.o.">
//     This file contains proprietary code. Copyright (c) 2021. All rights reserved.
// </copyright>

#ifndef INVOICESMODEL_H
#define INVOICESMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

class LogicController;

/*!
 * \brief Class representing a wrapper for sorting and filtering functionality of the invoices model.
 */
class InvoicesProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param QObject *parent -- parent.
     */
    explicit InvoicesProxyModel(QObject *parent = nullptr);
    /*!
     * \brief Functionality for filtering invoices.
     * \param int sourceRow -- row.
     * \param QModelIndex &sourceParent -- source parent.
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public slots:

    /*!
     * \brief Sets filtering pattern.
     * \param const QString &pattern -- pattern to use for filtering.
     */
    void setFilteringPattern(const QString &pattern);

protected:
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

private:
    QRegExp m_filteringRegExp;
};

/*!
 * \brief Class representing an invoices model.
 */
class InvoicesModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param LogicController *logicController -- logic controller responsible for manipulating different parts of the application.
     * \param QObject *parent -- parent.
     */
    explicit InvoicesModel(LogicController *logicController, QObject *parent = nullptr);

    /*!
     * \brief Enum representing a column of the table displaying invoices.
     */
    enum Column {
        NumberColumn,
        PartyColumn,
        StatusColumn,
        DateColumn,
        DueDateColumn,
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
     * \brief Returns data of the table at the given index and at the given role.
     * \param const QModelIndex &index -- index of a cell in the invoices table
     * \param int role -- role of a cell in the invoices table.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /*!
     * \brief Returns column name of the specified section.
     * \param Qt::Orientation orientation -- orientation.
     * \param int role -- role of a cell in the invoices table.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:

    /*!
     * \brief Resets the model.
     */
    void loadData();

private:
    LogicController *m_logicController = nullptr;
};

#endif // INVOICESMODEL_H
