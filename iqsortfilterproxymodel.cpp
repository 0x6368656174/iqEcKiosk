#include "iqsortfilterproxymodel.h"

IQSortFilterProxyModel::IQSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    _sortOrder(Qt::AscendingOrder),
    _sortRoleName("")
{
}

void IQSortFilterProxyModel::setSortOrder(const Qt::SortOrder sortOrder)
{
    if (_sortOrder != sortOrder)
    {
        _sortOrder = sortOrder;

        emit sortOrderChanged();
    }
}

void IQSortFilterProxyModel::setSortRoleName(const QString &sortRoleName)
{
    if (_sortRoleName != sortRoleName)
    {
        _sortRoleName = sortRoleName;

        emit sortRoleNameChanged();
    }
}

void IQSortFilterProxyModel::sort()
{
    //Найдем ид для заданой роли
    int roleId = roleNames().key(_sortRoleName.toLocal8Bit());
    if (roleId < 0)
        return;

    setSortRole(roleId);

    if (columnCount() > 0)
        QSortFilterProxyModel::sort(0, _sortOrder);
}
