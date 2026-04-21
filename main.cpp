#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include "authwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("repair_workshop");
    db.setUserName("postgre");
    db.setPassword("postgres");
    db.setPort(5432);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось подключиться к базе данных PostgreSQL:\n" +
                                  db.lastError().text() +
                                  "\n\nПроверьте настройки подключения и запустите скрипт SQL");
        return -1;
    }

    AuthWindow w;
    w.show();

    return a.exec();
}
