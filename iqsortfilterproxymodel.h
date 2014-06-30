#ifndef IQSORTFILTERPROXYMODEL_H
#define IQSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class IQSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(QString sortRoleName READ sortRoleName WRITE setSortRoleName NOTIFY sortRoleNameChanged)
public:
    explicit IQSortFilterProxyModel(QObject *parent = 0);

public slots:
    void sort();

private:
    inline Qt::SortOrder sortOrder() const {return _sortOrder;}
    void setSortOrder(const Qt::SortOrder sortOrder);

    inline QString sortRoleName() const {return _sortRoleName;}
    void setSortRoleName(const QString &sortRoleName);

signals:
    void sortOrderChanged();
    void sortRoleNameChanged();

private:
    Qt::SortOrder _sortOrder;
    QString _sortRoleName;
};

#endif // IQSORTFILTERPROXYMODEL_H
