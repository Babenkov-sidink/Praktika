#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "authwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_registerButton_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    QString confirmPassword = ui->confirmPasswordEdit->text();
    QString fullName = ui->fullNameEdit->text();
    QString email = ui->emailEdit->text();
    QString phone = ui->phoneEdit->text();
    QString address = ui->addressEdit->toPlainText();

    if (username.isEmpty() || password.isEmpty() || fullName.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все обязательные поля");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash, full_name, email, role) VALUES (:username, :password, :full_name, :email, 'client') RETURNING id");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":full_name", fullName);
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();

        if (phone.isEmpty()) {
            phone = "не указан";
        }
        if (address.isEmpty()) {
            address = "не указан";
        }

        QSqlQuery customerQuery;
        customerQuery.prepare("INSERT INTO customers (user_id, full_name, phone, email, address) VALUES (:user_id, :full_name, :phone, :email, :address)");
        customerQuery.bindValue(":user_id", userId);
        customerQuery.bindValue(":full_name", fullName);
        customerQuery.bindValue(":phone", phone);
        customerQuery.bindValue(":email", email);
        customerQuery.bindValue(":address", address);

        if (customerQuery.exec()) {
            QMessageBox::information(this, "Успех", "Регистрация успешно завершена!");
            AuthWindow *authWin = new AuthWindow();
            authWin->show();
            this->close();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось создать профиль заказчика: " + customerQuery.lastError().text());
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Пользователь с таким логином или email уже существует");
    }
}

void RegisterWindow::on_backButton_clicked()
{
    AuthWindow *authWin = new AuthWindow();
    authWin->show();
    this->close();
}
