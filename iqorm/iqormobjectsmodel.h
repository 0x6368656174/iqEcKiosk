#ifndef IQORMOBJECTSMODEL_H
#define IQORMOBJECTSMODEL_H

#define FIRST_QML_ROLE 1000

#include <QAbstractTableModel>
#include <QMetaProperty>
#include <QDebug>
#include <QStringList>
#include <typeinfo>
#include <QQuickItem>
#include "iqormfilter.h"
#include "iqormmodel.h"
#include "iqormcore.h"

class IQORMObjectsModelBase : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(IQORMAbstractFilter * filters READ filters WRITE setFilters NOTIFY filtersChanged)
    Q_PROPERTY(IQORMError * lastError READ lastError CONSTANT)
    Q_PROPERTY(qint64 count READ count NOTIFY countChanged)

public:
    explicit IQORMObjectsModelBase(QObject *parent = 0);

    virtual ~IQORMObjectsModelBase();

    inline IQORMAbstractFilter *filters() const {return _filters;}

    Q_INVOKABLE virtual QObject *get(const int row) const = 0;

    Q_INVOKABLE virtual bool load(const qint64 limit = -1,
                                  const qint64 offset = 0,
                                  IQORMAbstractDataSource *dataSource = 0) = 0;

    virtual qint64 count() const = 0;

    virtual void clear() = 0;

    virtual IQORMModel * childsOrmModel() const = 0;

    virtual QList<IQORMObject *> toObjectList() const = 0;

    virtual void removeObject(IQORMObject *object) = 0;

    virtual IQORMObject * createChildObject() const = 0;

    virtual void appendObject(IQORMObject *object) = 0;

    virtual QString childsTypeName() const = 0;

    void setFilters(IQORMAbstractFilter *filters);

    virtual IQORMAbstractDataSource* lastDataSource() const = 0;

    inline IQORMError *lastError() const {return _lastError;}

signals:
    void invisibleDataChanged(int row, const QString &property);
    void filtersChanged();
    void countChanged();

protected slots:
    void onObjectChanged();

    virtual void emitDataChangedSignal(QObject *object, const QString &property) = 0;

protected:
    int _indexOfOnObjectChangedMethod;
    QHash<int, QString> _propertySignalsIndex;

    IQORMAbstractFilter *_filters;
    IQORMError *_lastError;
};

template <typename T>
class IQORMObjectsModel : public IQORMObjectsModelBase
{
public:
    explicit IQORMObjectsModel(QObject *parent = 0);

    virtual ~IQORMObjectsModel();

    void setPropertyEditable(const QString &property, const bool editable = true);

    void showProperty(const QString &property, const QString &header = QString());

    void hideProperty(const QString &property);

    void setPropertyHeader(const QString &property, const QString &header = QString());

    void append(T* object);

    void insert(const int row, T* object);

    void remove(T* object);

    virtual void clear();

    int rowOf(T* object) const;

    T* atRow(const int row) const;




    virtual QList<IQORMObject *> toObjectList() const;

    virtual inline QList<T *> toList() const {return _items;}

    virtual void removeObject(IQORMObject *object);

    virtual IQORMObject * createChildObject() const;

    virtual void appendObject(IQORMObject *object);

    virtual QString childsTypeName() const;

    virtual IQORMModel * childsOrmModel() const;

    virtual bool load(IQORMAbstractDataSource *dataSource);

    virtual bool load(const qint64 limit = -1,
                      const qint64 offset = 0,
                      IQORMAbstractDataSource *dataSource = 0);

    virtual IQORMAbstractDataSource* lastDataSource() const;



    virtual QObject * get(const int row) const;

    virtual qint64 count() const {return rowCount();}


    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const;

private:
    QList<T *> _items;

    QStringList _propertyNames;

    QList<QString> _visibleProperties;
    QSet<QString> _editableProperties;
    //1 - имя свойства, 2 - его заголовок
    QHash<QString, QString> _propertyHeaders;

    virtual void emitDataChangedSignal(QObject *object, const QString &property);

    QPointer<IQORMAbstractDataSource> _lastDataSource;
};









template <typename T>
IQORMAbstractDataSource* IQORMObjectsModel<T>::lastDataSource() const
{
    return _lastDataSource;
}


template <typename T>
QString IQORMObjectsModel<T>::childsTypeName() const
{
    return QString(typeid(T).name());
}

template <typename T>
void IQORMObjectsModel<T>::appendObject(IQORMObject *object)
{
    if (!object)
        return;

    T* castObject = dynamic_cast<T *>(object);
    if (!castObject)
        return;

    append(castObject);
}


template <typename T>
IQORMObject * IQORMObjectsModel<T>::createChildObject() const
{
    return new T();
}

template <typename T>
void IQORMObjectsModel<T>::removeObject(IQORMObject *object)
{
    if (!object)
        return;

    T* castObject = dynamic_cast<T *>(object);
    if (!castObject)
        return;

    remove(castObject);
}

template <typename T>
QList<IQORMObject *> IQORMObjectsModel<T>::toObjectList() const
{
    QList<IQORMObject *> result;

    foreach (T* object, _items)
    {
        if (!object)
            continue;

        result.append(object);
    }

    return result;
}


template <typename T>
IQORMModel* IQORMObjectsModel<T>::childsOrmModel() const
{
    return IQORMCore::staticORMModel<T>();
}

template <typename T>
IQORMObjectsModel<T>::IQORMObjectsModel(QObject *parent) :
    IQORMObjectsModelBase(parent),
    _lastDataSource(NULL)
{
    for (int i = 0; i < T::staticMetaObject.propertyCount(); i++)
    {
        _propertyNames << QString(T::staticMetaObject.property(i).name());
    }

    _indexOfOnObjectChangedMethod = metaObject()->indexOfMethod(QMetaObject::normalizedSignature("onObjectChanged()").constData());

    for (int i = 0; i < T::staticMetaObject.propertyCount(); i++)
    {
        QMetaProperty objectProperty = T::staticMetaObject.property(i);
        if (!objectProperty.isValid())
            continue;

        _propertySignalsIndex[objectProperty.notifySignalIndex()] =
                QString(objectProperty.name());
    }

    //Зарегестрируем модель
    IQORMCore::synchronizer()->registerObjectsModel(this);
}

template <typename T>
IQORMObjectsModel<T>::~IQORMObjectsModel()
{
    //Разрегестрируем модель
    IQORMCore::synchronizer()->unregisterObjectsModel(this);
}

template <typename T>
int IQORMObjectsModel<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _items.count();
}

template <typename T>
int IQORMObjectsModel<T>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    //Хак для правильного отображения в QML
    //Если модель создана в QML, то всегда должна быть как минимут одна колонка
    if(dynamic_cast<QQuickItem *>(QObject::parent()))
        return _visibleProperties.count() > 0?_visibleProperties.count():1;

    return _visibleProperties.count();
}

template <typename T>
QHash<int, QByteArray> IQORMObjectsModel<T>::roleNames() const
{
    QHash<int, QByteArray> result;

    for(int i = 0; i < _propertyNames.count(); i++)
    {
        result[FIRST_QML_ROLE + i] = _propertyNames[i].toLocal8Bit();
    }

    return result;
}

template <typename T>
QVariant IQORMObjectsModel<T>::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (row >= rowCount() || row < 0)
        return QVariant();

    if (column >= _propertyNames.count())
        return QVariant();

    if (role >= FIRST_QML_ROLE + _propertyNames.count())
        return QVariant();

    T* item = _items[row];
    if (!item)
        return QVariant();

    QString propertyName = "";
    if (role < FIRST_QML_ROLE)
    {
        if (column < _visibleProperties.count())
            propertyName = _visibleProperties[column];
    }
    else
    {
        int firsQmlRole = FIRST_QML_ROLE;
        int propertyIndex = role - firsQmlRole;
        propertyName = _propertyNames[propertyIndex];
    }

    if (propertyName.isEmpty())
        return QVariant();
    if (role == Qt::DisplayRole
            || role == Qt::EditRole
            || role > FIRST_QML_ROLE)
        return item->property(propertyName.toLocal8Bit().constData());

    return QVariant();
}

template <typename T>
void IQORMObjectsModel<T>::append(T *object)
{
    insert(_items.count(), object);
}

template <typename T>
int IQORMObjectsModel<T>::rowOf(T *object) const
{
    return _items.indexOf(object);
}

template <typename T>
void IQORMObjectsModel<T>::insert(const int row, T *object)
{
    if (_items.contains(object))
        return;

    int rowToInsert = row < 0?0:row;

    if (rowToInsert > rowCount())
        rowToInsert = rowCount();

    emit beginInsertRows(QModelIndex(), rowToInsert, rowToInsert);

    _items.insert(rowToInsert, object);

    object->setParent(this);

    for (int i = 0; i < object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty objectProperty = object->metaObject()->property(i);
        if (!objectProperty.isValid())
            continue;

        QMetaMethod notifiSignal = objectProperty.notifySignal();
        if (!notifiSignal.isValid())
            continue;

        connect(object,
                notifiSignal,
                this,
                metaObject()->method(_indexOfOnObjectChangedMethod));
    }

    emit endInsertRows();
    emit countChanged();
}

template <typename T>
void IQORMObjectsModel<T>::emitDataChangedSignal(QObject *object, const QString &property)
{
    T* senderObject = qobject_cast<T*>(object);

    if (!_items.contains(senderObject))
        return;

    int itemRow = _items.indexOf(senderObject);
    int propertyColumn = _visibleProperties.indexOf(property);

    if (propertyColumn != -1)
    {
        QModelIndex topLeft = index(itemRow, propertyColumn);
        QModelIndex bottomRigth = index(itemRow, propertyColumn);
        emit dataChanged(topLeft, bottomRigth);
    }
    else
    {
        QModelIndex topLeft = index(itemRow, 0);
        QModelIndex bottomRigth = index(itemRow, 0);
        emit dataChanged(topLeft, bottomRigth);

        emit invisibleDataChanged(itemRow, property);
    }
}

template <typename T>
void IQORMObjectsModel<T>::showProperty(const QString &property, const QString &header)
{
    if (_visibleProperties.contains(property))
        return;

    emit beginInsertColumns(QModelIndex(), _visibleProperties.count(), _visibleProperties.count());
    _visibleProperties.append(property);
    _propertyHeaders[property] = header;
    emit endInsertColumns();
}

template <typename T>
void IQORMObjectsModel<T>::hideProperty(const QString &property)
{
    if (!_visibleProperties.contains(property))
        return;

    emit beginRemoveColumns(QModelIndex(), _visibleProperties.indexOf(property), _visibleProperties.indexOf(property));
    _visibleProperties.removeAll(property);
    emit endRemoveColumns();
}

template <typename T>
void IQORMObjectsModel<T>::setPropertyHeader(const QString &property, const QString &header)
{
    if (_propertyHeaders[property] == header)
        return;

    _propertyHeaders[property] = header;

    if (_visibleProperties.contains(property))
        emit headerDataChanged(Qt::Horizontal, _visibleProperties.indexOf(property), _visibleProperties.indexOf(property));
}

template <typename T>
QVariant IQORMObjectsModel<T>::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    if (orientation == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);

    if (section >= _visibleProperties.count() || section < 0)
        return QAbstractTableModel::headerData(section, orientation, role);

    QString property = _visibleProperties[section];

    if (role == Qt::DisplayRole && !_propertyHeaders[property].isEmpty())
    {
        return _propertyHeaders[property];
    }
    else
    {
        return property;
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

template <typename T>
T* IQORMObjectsModel<T>::atRow(const int row) const
{
    if (row < 0 || row >= _items.count())
        return NULL;

    return _items[row];
}

template <typename T>
QObject * IQORMObjectsModel<T>::get(const int row) const
{
    return qobject_cast<QObject*> (atRow(row));
}

template <typename T>
void IQORMObjectsModel<T>::remove(T *object)
{
    if (!_items.contains(object))
        return;

    qint64 oldCount = count();

    emit beginRemoveRows(QModelIndex(), _items.indexOf(object), _items.indexOf(object));
    if(_items.removeAll(object))
    {
        disconnect(object, 0, this, 0);
        object->setParent(NULL);
    }
    emit endRemoveRows();

    if (count() != oldCount)
        emit countChanged();
}

template <typename T>
void IQORMObjectsModel<T>::clear()
{
    if (rowCount() == 0)
        return;

    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

    _items.clear();

    emit endRemoveRows();
    emit countChanged();
}

template <typename T>
bool IQORMObjectsModel<T>::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if (row < 0 || row >= _items.count())
        return false;

    int column = index.column();
    if (column < 0 || column >= _visibleProperties.count())
        return false;

    if (role >= FIRST_QML_ROLE + _propertyNames.count())
        return false;

    T* object = _items[row];
    if (!object)
        return false;

    QString propertyName = "";
    if (role < FIRST_QML_ROLE)
    {
        propertyName = _visibleProperties[index.column()];
    }
    else
    {
        int propertyIndex = role - FIRST_QML_ROLE;
        propertyName = _propertyNames[propertyIndex];
    }

    if (propertyName.isEmpty())
        return false;

    bool result = object->setProperty(propertyName.toLocal8Bit().constData(), value);

    if (result)
        emit dataChanged(this->index(row,column), this->index(row, column));

    return result;
}

template <typename T>
void IQORMObjectsModel<T>::setPropertyEditable(const QString &property, const bool editable)
{
    if (editable)
    {
        _editableProperties.insert(property);
    }
    else
    {
        _editableProperties.remove(property);
    }
}

template <typename T>
Qt::ItemFlags IQORMObjectsModel<T>::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.column() < 0 || index.column() >= _visibleProperties.count())
        return defaultFlags;

    if (index.row() < 0 || index.row() >= _items.count())
        return defaultFlags;

    QString propertyName = _visibleProperties[index.column()];

    QMetaProperty property = T::staticMetaObject.property(_propertyNames.indexOf(propertyName));

    if (_editableProperties.contains(propertyName)
            && property.isWritable())
        defaultFlags = defaultFlags | Qt::ItemIsEditable;

    return defaultFlags;
}

template <typename T>
bool IQORMObjectsModel<T>::moveRows(const QModelIndex &sourceParent,
                                       int sourceRow,
                                       int count,
                                       const QModelIndex &destinationParent,
                                       int destinationChild)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(destinationParent);
    if (sourceRow < 0 || sourceRow >= _items.count())
        return false;

    int correctedCount = _items.count() - (sourceRow + count) < 0 ? _items.count() - sourceRow : count;

    int correctedDestinationChild = destinationChild;

    if (destinationChild < 0)
        correctedDestinationChild = 0;

    if (destinationChild + correctedCount > _items.count() + 1)
        correctedDestinationChild = _items.count() - correctedCount;

    if (sourceRow == correctedDestinationChild)
        return false;

    emit beginMoveRows(sourceParent, sourceRow, sourceRow + correctedCount - 1, destinationParent, correctedDestinationChild);

    QList<T *> temp;

    for (int i = sourceRow + correctedCount - 1; i >= sourceRow ; i--)
    {
        temp << _items[i];
        _items.removeAt(i);
    }

    for (int i = 0; i < correctedCount; i++)
    {
        if (destinationChild < sourceRow)
        {
            _items.insert(destinationChild + i, temp[i]);
        }
        else
        {
            _items.insert(destinationChild + i - 1, temp[i]);
        }
    }

    emit endMoveRows();

    return true;
}

template <typename T>
bool IQORMObjectsModel<T>::load(IQORMAbstractDataSource *dataSource)
{
    return load(-1, 0, dataSource);
}

template <typename T>
bool IQORMObjectsModel<T>::load(const qint64 limit,
                                const qint64 offset,
                                IQORMAbstractDataSource *dataSource)
{
    lastError()->clearErrors();

    if (dataSource)
    {
        if (_lastDataSource.data() == dataSource)
            return true;

        _lastDataSource = dataSource;
    }

    if (!_lastDataSource)
        _lastDataSource = IQORMCore::dataSource();

    if (!_lastDataSource)
    {
        lastError()->addError(QObject::tr("Not found any Data Source. Use IQORMCore::setDataSource() first."));
        return false;
    }

    if (!_lastDataSource->objectsModelDataSource())
    {
        lastError()->addError(QObject::tr("Not found any Objects Model Data Source. Maybe Data Source broken, check Data Source."));
        return false;
    }

    bool result = _lastDataSource->objectsModelDataSource()->loadModel(this, limit, offset);

    return result;
}

#endif // IQORMOBJECTSMODEL_H
