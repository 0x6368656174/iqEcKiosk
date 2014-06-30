#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "cardinfo/iqcardinfo.h"
#include <QQmlContext>
#include <QSqlDatabase>
#include "iqormcore.h"
#include "iqormsqldatasource.h"
#include <QSettings>
#include <QCoreApplication>
#include "iqcardcontroller.h"
#include "iqsortfilterproxymodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("itQuasar");
    QCoreApplication::setApplicationName("ecKiosk");

    QSettings setting;
    if (!setting.contains("odbcName"))
        setting.setValue("odbcName", "ecKiosk");
    if (!setting.contains("dbUser"))
        setting.setValue("dbUser", "sa");
    if (!setting.contains("dbPassword"))
        setting.setValue("dbPassword", "");

    //Настроим подключение к БД
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(setting.value("odbcName").toString());
    db.setUserName(setting.value("dbUser").toString());
    db.setPassword(setting.value("dbPassword").toString());

    //Создадим источник данных, работающий с БД
    IQORMSQLDataSource *sqlDataSource = new IQORMSQLDataSource();

    //Установим для него подключение к БД
    sqlDataSource->setDatabase(db);
    IQORMCore::setDataSource(sqlDataSource);

    IQCardInfo::registerQmlTypes();
    qmlRegisterType<IQCardController>("ru.itquasar.Qt.ECKiosk.MitecController", 1, 0, "CardController");

    qmlRegisterUncreatableType<QAbstractItemModel>("ru.itquasar.Qt.ECKiosk.QML", 1, 0, "AbstractItemModel", "AbstractItemModel is virtual type. Create is not suporte.");
    qmlRegisterType<IQSortFilterProxyModel>("ru.itquasar.Qt.ECKiosk.QML", 1, 0, "SortModel");


    QtQuick2ApplicationViewer viewer;
    viewer.setTitle("ECKiosk - Киоск Развлекательного Центра");
    viewer.setIcon(QIcon("://icons/dataOfComputer.png"));
    viewer.setMainQmlFile(QStringLiteral("qml/eckiosk/main.qml"));
    viewer.showFullScreen();

    return app.exec();
}
