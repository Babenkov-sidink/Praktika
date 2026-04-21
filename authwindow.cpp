#include "authwindow.h"
#include "ui_authwindow.h"
#include "registerwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

AuthWindow::AuthWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    connectToDatabase();
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

bool AuthWindow::connectToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("repair_workshop");
    db.setUserName("postgre");
    db.setPassword("postgres");
    db.setPort(5432);

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к БД: " + db.lastError().text());
        return false;
    }
    return true;
}

void AuthWindow::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id, full_name, role FROM users WHERE username = :username AND password_hash = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        QString fullName = query.value(1).toString();
        QString role = query.value(2).toString();

        MainWindow *mainWin = new MainWindow(userId, fullName, role);
        mainWin->show();
        this->close();
    } else {
        QMessageBox::critical(this, "Ошибка", "Неверный логин или пароль");
    }
}

void AuthWindow::on_registerButton_clicked()
{
    RegisterWindow *regWin = new RegisterWindow();
    regWin->show();
    this->close();
}
