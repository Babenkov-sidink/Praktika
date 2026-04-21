#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

MainWindow::MainWindow(int userId, QString fullName, QString role, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUserId(userId)
    , currentFullName(fullName)
    , currentRole(role)
{
    ui->setupUi(this);
    setWindowTitle("RepairMaster - " + fullName + " (" + role + ")");
    setupModels();
    setPermissions();
    setupButtonStyles();
    refreshAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupButtonStyles()
{
    ui->addOrderButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1e7e46;"
        "}"
        );

    ui->editOrderButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f39c12;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e67e22;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #b85e0e;"
        "}"
        );

    ui->deleteOrderButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c0392b;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #992e22;"
        "}"
        );

    ui->refreshOrdersButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #27ae60;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #229954;"
        "}"
        );

    ui->searchOrdersButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f39c12;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 25px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e67e22;"
        "}"
        );

    ui->logoutButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #7f8c8d;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 25px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #6c7a7a;"
        "}"
        );

    ui->addWorkTypeButton->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #229954; }"
        );

    ui->editWorkTypeButton->setStyleSheet(
        "QPushButton { background-color: #f39c12; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #e67e22; }"
        );

    ui->deleteWorkTypeButton->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #c0392b; }"
        );

    ui->addPerformerButton->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #229954; }"
        );

    ui->editPerformerButton->setStyleSheet(
        "QPushButton { background-color: #f39c12; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #e67e22; }"
        );

    ui->deletePerformerButton->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #c0392b; }"
        );

    ui->addCustomerButton->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #229954; }"
        );

    ui->editCustomerButton->setStyleSheet(
        "QPushButton { background-color: #f39c12; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #e67e22; }"
        );

    ui->deleteCustomerButton->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #c0392b; }"
        );
}

void MainWindow::setupModels()
{
    ordersModel = new QSqlTableModel(this);
    ordersModel->setTable("repair_orders");
    ordersModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ordersModel->setHeaderData(1, Qt::Horizontal, "Номер заказа");
    ordersModel->setHeaderData(2, Qt::Horizontal, "Заказчик");
    ordersModel->setHeaderData(3, Qt::Horizontal, "Вид работы");
    ordersModel->setHeaderData(4, Qt::Horizontal, "Исполнитель");
    ordersModel->setHeaderData(5, Qt::Horizontal, "Дата заказа");
    ordersModel->setHeaderData(6, Qt::Horizontal, "Срок");
    ordersModel->setHeaderData(7, Qt::Horizontal, "Статус");
    ordersModel->setHeaderData(8, Qt::Horizontal, "Часы");
    ordersModel->setHeaderData(9, Qt::Horizontal, "Стоимость");
    ordersModel->setHeaderData(10, Qt::Horizontal, "Описание");
    ui->ordersTableView->setModel(ordersModel);
    ui->ordersTableView->hideColumn(0);
    ui->ordersTableView->resizeColumnsToContents();

    workTypesModel = new QSqlTableModel(this);
    workTypesModel->setTable("work_types");
    workTypesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    workTypesModel->setHeaderData(1, Qt::Horizontal, "Название");
    workTypesModel->setHeaderData(2, Qt::Horizontal, "Базовая цена");
    ui->workTypesTableView->setModel(workTypesModel);
    ui->workTypesTableView->hideColumn(0);
    ui->workTypesTableView->resizeColumnsToContents();

    performersModel = new QSqlTableModel(this);
    performersModel->setTable("performers");
    performersModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    performersModel->setHeaderData(1, Qt::Horizontal, "ФИО");
    performersModel->setHeaderData(2, Qt::Horizontal, "Специальность");
    performersModel->setHeaderData(3, Qt::Horizontal, "Телефон");
    performersModel->setHeaderData(4, Qt::Horizontal, "Ставка/час");
    ui->performersTableView->setModel(performersModel);
    ui->performersTableView->hideColumn(0);
    ui->performersTableView->resizeColumnsToContents();

    customersModel = new QSqlTableModel(this);
    customersModel->setTable("customers");
    customersModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    customersModel->setHeaderData(1, Qt::Horizontal, "ФИО");
    customersModel->setHeaderData(2, Qt::Horizontal, "Телефон");
    customersModel->setHeaderData(3, Qt::Horizontal, "Email");
    customersModel->setHeaderData(4, Qt::Horizontal, "Адрес");
    ui->customersTableView->setModel(customersModel);
    ui->customersTableView->hideColumn(0);
    ui->customersTableView->resizeColumnsToContents();

    ui->ordersTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->workTypesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->performersTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->customersTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::loadOrders()
{
    if (currentRole != "admin") {
        QSqlQuery query;
        query.prepare("SELECT id FROM customers WHERE user_id = :user_id");
        query.bindValue(":user_id", currentUserId);
        query.exec();
        if (query.next()) {
            int customerId = query.value(0).toInt();
            ordersModel->setFilter(QString("customer_id = %1").arg(customerId));
        } else {
            ordersModel->setFilter("customer_id = -1");
        }
    } else {
        ordersModel->setFilter("");
    }
    ordersModel->select();
    ui->ordersTableView->resizeColumnsToContents();
}

void MainWindow::loadWorkTypes()
{
    workTypesModel->select();
    ui->workTypesTableView->resizeColumnsToContents();
}

void MainWindow::loadPerformers()
{
    performersModel->select();
    ui->performersTableView->resizeColumnsToContents();
}

void MainWindow::loadCustomers()
{
    if (currentRole != "admin") {
        QSqlQuery query;
        query.prepare("SELECT id FROM customers WHERE user_id = :user_id");
        query.bindValue(":user_id", currentUserId);
        query.exec();
        if (query.next()) {
            int customerId = query.value(0).toInt();
            customersModel->setFilter(QString("id = %1").arg(customerId));
        } else {
            customersModel->setFilter("id = -1");
        }
    } else {
        customersModel->setFilter("");
    }
    customersModel->select();
    ui->customersTableView->resizeColumnsToContents();
}

void MainWindow::setPermissions()
{
    if (currentRole != "admin") {
        ui->tabWidget->setTabVisible(1, false);
        ui->tabWidget->setTabVisible(2, false);
        ui->tabWidget->setTabVisible(3, false);

        ui->editOrderButton->setVisible(false);
        ui->deleteOrderButton->setVisible(false);

        ui->statusBar->showMessage("Вы вошли как клиент. Доступны только ваши заказы.", 5000);
    } else {
        ui->addOrderButton->setVisible(true);
        ui->editOrderButton->setVisible(true);
        ui->deleteOrderButton->setVisible(true);
        ui->refreshOrdersButton->setVisible(true);
        ui->statusBar->showMessage("Вы вошли как администратор. Полный доступ.", 5000);
    }
}

void MainWindow::refreshAll()
{
    loadOrders();
    if (currentRole == "admin") {
        loadWorkTypes();
        loadPerformers();
        loadCustomers();
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0) loadOrders();
    else if (index == 1 && currentRole == "admin") loadWorkTypes();
    else if (index == 2 && currentRole == "admin") loadPerformers();
    else if (index == 3 && currentRole == "admin") loadCustomers();
}

int MainWindow::getCurrentCustomerId()
{
    QSqlQuery query;
    query.prepare("SELECT id FROM customers WHERE user_id = :user_id");
    query.bindValue(":user_id", currentUserId);
    query.exec();
    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

void MainWindow::showOrderDialog(int row)
{
    QDialog dialog(this);
    dialog.setWindowTitle(row < 0 ? "Добавить заказ" : "Редактировать заказ");
    dialog.setMinimumWidth(500);
    dialog.setStyleSheet("QDialog { background-color: #f0f0f0; } QLabel { color: #000000; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();

    QComboBox *customerCombo = new QComboBox();
    int currentCustomerId = -1;

    if (currentRole == "admin") {
        QSqlQuery customerQuery("SELECT id, full_name FROM customers");
        while (customerQuery.next()) {
            customerCombo->addItem(customerQuery.value(1).toString(), customerQuery.value(0));
        }
    } else {
        QSqlQuery customerQuery;
        customerQuery.prepare("SELECT id, full_name FROM customers WHERE user_id = :user_id");
        customerQuery.bindValue(":user_id", currentUserId);
        customerQuery.exec();
        if (customerQuery.next()) {
            currentCustomerId = customerQuery.value(0).toInt();
            customerCombo->addItem(customerQuery.value(1).toString(), currentCustomerId);
        }
        customerCombo->setEnabled(false);
    }

    QComboBox *workTypeCombo = new QComboBox();
    QSqlQuery workQuery("SELECT id, name FROM work_types");
    while (workQuery.next()) {
        workTypeCombo->addItem(workQuery.value(1).toString(), workQuery.value(0));
    }

    QComboBox *performerCombo = new QComboBox();
    QSqlQuery perfQuery("SELECT id, full_name FROM performers");
    while (perfQuery.next()) {
        performerCombo->addItem(perfQuery.value(1).toString(), perfQuery.value(0));
    }

    QLineEdit *orderNumberEdit = new QLineEdit();
    QDateEdit *orderDateEdit = new QDateEdit(QDate::currentDate());
    QDateEdit *deadlineEdit = new QDateEdit(QDate::currentDate().addDays(7));
    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"в работе", "завершен", "отменен"});
    QDoubleSpinBox *hoursSpin = new QDoubleSpinBox();
    hoursSpin->setRange(0, 1000);
    hoursSpin->setDecimals(2);
    QTextEdit *descEdit = new QTextEdit();

    QPushButton *saveBtn = new QPushButton("Сохранить");
    QPushButton *cancelBtn = new QPushButton("Отмена");

    saveBtn->setStyleSheet("QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #229954; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #7f8c8d; }");

    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(new QLabel("Номер заказа:"));
    layout->addWidget(orderNumberEdit);
    layout->addWidget(new QLabel("Заказчик:"));
    layout->addWidget(customerCombo);
    layout->addWidget(new QLabel("Вид работы:"));
    layout->addWidget(workTypeCombo);
    layout->addWidget(new QLabel("Исполнитель:"));
    layout->addWidget(performerCombo);
    layout->addWidget(new QLabel("Дата заказа:"));
    layout->addWidget(orderDateEdit);
    layout->addWidget(new QLabel("Срок выполнения:"));
    layout->addWidget(deadlineEdit);
    layout->addWidget(new QLabel("Статус:"));
    layout->addWidget(statusCombo);
    layout->addWidget(new QLabel("Затрачено часов:"));
    layout->addWidget(hoursSpin);
    layout->addWidget(new QLabel("Описание:"));
    layout->addWidget(descEdit);
    layout->addLayout(btnLayout);

    if (row >= 0) {
        QSqlRecord rec = ordersModel->record(row);
        orderNumberEdit->setText(rec.value("order_number").toString());
        int custId = rec.value("customer_id").toInt();
        for (int i = 0; i < customerCombo->count(); i++) {
            if (customerCombo->itemData(i).toInt() == custId) customerCombo->setCurrentIndex(i);
        }
        int workId = rec.value("work_type_id").toInt();
        for (int i = 0; i < workTypeCombo->count(); i++) {
            if (workTypeCombo->itemData(i).toInt() == workId) workTypeCombo->setCurrentIndex(i);
        }
        int perfId = rec.value("performer_id").toInt();
        for (int i = 0; i < performerCombo->count(); i++) {
            if (performerCombo->itemData(i).toInt() == perfId) performerCombo->setCurrentIndex(i);
        }
        orderDateEdit->setDate(rec.value("order_date").toDate());
        deadlineEdit->setDate(rec.value("deadline_date").toDate());
        statusCombo->setCurrentText(rec.value("status").toString());
        hoursSpin->setValue(rec.value("hours_spent").toDouble());
        descEdit->setText(rec.value("description").toString());
    }

    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (orderNumberEdit->text().isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Введите номер заказа");
            return;
        }

        int customerId;
        if (currentRole == "admin") {
            customerId = customerCombo->currentData().toInt();
        } else {
            customerId = currentCustomerId;
            if (customerId == -1) {
                QMessageBox::warning(&dialog, "Ошибка", "Не найден профиль заказчика. Обратитесь к администратору.");
                return;
            }
        }

        QSqlQuery query;
        if (row < 0) {
            query.prepare("INSERT INTO repair_orders (order_number, customer_id, work_type_id, performer_id, order_date, deadline_date, status, hours_spent, description) VALUES (:num, :cust, :work, :perf, :odate, :ddate, :status, :hours, :desc)");
        } else {
            query.prepare("UPDATE repair_orders SET order_number=:num, customer_id=:cust, work_type_id=:work, performer_id=:perf, order_date=:odate, deadline_date=:ddate, status=:status, hours_spent=:hours, description=:desc WHERE id=:id");
            query.bindValue(":id", ordersModel->record(row).value("id"));
        }
        query.bindValue(":num", orderNumberEdit->text());
        query.bindValue(":cust", customerId);
        query.bindValue(":work", workTypeCombo->currentData());
        query.bindValue(":perf", performerCombo->currentData());
        query.bindValue(":odate", orderDateEdit->date());
        query.bindValue(":ddate", deadlineEdit->date());
        query.bindValue(":status", statusCombo->currentText());
        query.bindValue(":hours", hoursSpin->value());
        query.bindValue(":desc", descEdit->toPlainText());

        if (query.exec()) {
            dialog.accept();
            loadOrders();
            QMessageBox::information(&dialog, "Успех", "Заказ успешно сохранен");
        } else {
            QMessageBox::critical(&dialog, "Ошибка", query.lastError().text());
        }
    });
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::on_addOrderButton_clicked()
{
    if (currentRole != "admin") {
        int customerId = getCurrentCustomerId();
        if (customerId == -1) {
            QMessageBox::warning(this, "Ошибка", "Ваш профиль заказчика не найден. Обратитесь к администратору.");
            return;
        }
    }
    showOrderDialog(-1);
}

void MainWindow::on_editOrderButton_clicked() {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может редактировать заказы");
        return;
    }
    int row = ui->ordersTableView->currentIndex().row();
    if (row >= 0) showOrderDialog(row);
    else QMessageBox::warning(this, "Ошибка", "Выберите заказ");
}

void MainWindow::on_deleteOrderButton_clicked() {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может удалять заказы");
        return;
    }
    int row = ui->ordersTableView->currentIndex().row();
    if (row < 0) { QMessageBox::warning(this, "Ошибка", "Выберите заказ"); return; }
    if (QMessageBox::question(this, "Удаление", "Удалить заказ?") == QMessageBox::Yes) {
        ordersModel->removeRow(row);
        ordersModel->submitAll();
        loadOrders();
    }
}

void MainWindow::on_refreshOrdersButton_clicked() { loadOrders(); }

void MainWindow::on_logoutButton_clicked() {
    AuthWindow *authWin = new AuthWindow();
    authWin->show();
    this->close();
}

void MainWindow::on_searchOrdersButton_clicked() { loadOrders(); }

void MainWindow::on_searchEdit_textChanged(const QString &text) {
    if (text.isEmpty()) {
        if (currentRole != "admin") {
            QSqlQuery query;
            query.prepare("SELECT id FROM customers WHERE user_id = :user_id");
            query.bindValue(":user_id", currentUserId);
            query.exec();
            if (query.next()) {
                int customerId = query.value(0).toInt();
                ordersModel->setFilter(QString("customer_id = %1").arg(customerId));
            } else {
                ordersModel->setFilter("customer_id = -1");
            }
        } else {
            ordersModel->setFilter("");
        }
    } else {
        if (currentRole != "admin") {
            QSqlQuery query;
            query.prepare("SELECT id FROM customers WHERE user_id = :user_id");
            query.bindValue(":user_id", currentUserId);
            query.exec();
            if (query.next()) {
                int customerId = query.value(0).toInt();
                ordersModel->setFilter(QString("customer_id = %1 AND (order_number LIKE '%%2%' OR description LIKE '%%2%')").arg(customerId).arg(text));
            } else {
                ordersModel->setFilter(QString("customer_id = -1 AND (order_number LIKE '%%1%' OR description LIKE '%%1%')").arg(text));
            }
        } else {
            ordersModel->setFilter(QString("order_number LIKE '%%1%' OR description LIKE '%%1%'").arg(text));
        }
    }
    loadOrders();
}

void MainWindow::showWorkTypeDialog(int row) {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может управлять видами работ");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(row < 0 ? "Добавить вид работы" : "Редактировать вид работы");
    dialog.setStyleSheet("QDialog { background-color: #f0f0f0; } QLabel { color: #000000; }");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLineEdit *nameEdit = new QLineEdit();
    QDoubleSpinBox *priceSpin = new QDoubleSpinBox();
    priceSpin->setRange(0, 100000);
    priceSpin->setPrefix("₽ ");
    if (row >= 0) {
        QSqlRecord rec = workTypesModel->record(row);
        nameEdit->setText(rec.value("name").toString());
        priceSpin->setValue(rec.value("base_price").toDouble());
    }
    layout->addWidget(new QLabel("Название:"));
    layout->addWidget(nameEdit);
    layout->addWidget(new QLabel("Базовая цена:"));
    layout->addWidget(priceSpin);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить");
    QPushButton *cancelBtn = new QPushButton("Отмена");

    saveBtn->setStyleSheet("QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #229954; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #7f8c8d; }");

    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameEdit->text().isEmpty()) { QMessageBox::warning(&dialog, "Ошибка", "Введите название"); return; }
        if (row < 0) workTypesModel->insertRow(workTypesModel->rowCount());
        int r = (row < 0) ? workTypesModel->rowCount() - 1 : row;
        workTypesModel->setData(workTypesModel->index(r, 1), nameEdit->text());
        workTypesModel->setData(workTypesModel->index(r, 2), priceSpin->value());
        if (workTypesModel->submitAll()) dialog.accept();
        else QMessageBox::critical(&dialog, "Ошибка", workTypesModel->lastError().text());
        loadWorkTypes();
    });
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::on_addWorkTypeButton_clicked() { showWorkTypeDialog(-1); }
void MainWindow::on_editWorkTypeButton_clicked() {
    int row = ui->workTypesTableView->currentIndex().row();
    if (row >= 0) showWorkTypeDialog(row);
    else QMessageBox::warning(this, "Ошибка", "Выберите вид работы");
}
void MainWindow::on_deleteWorkTypeButton_clicked() {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может удалять виды работ");
        return;
    }
    int row = ui->workTypesTableView->currentIndex().row();
    if (row < 0) return;
    if (QMessageBox::question(this, "Удаление", "Удалить?") == QMessageBox::Yes) {
        workTypesModel->removeRow(row);
        workTypesModel->submitAll();
        loadWorkTypes();
    }
}

void MainWindow::showPerformerDialog(int row) {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может управлять исполнителями");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(row < 0 ? "Добавить исполнителя" : "Редактировать исполнителя");
    dialog.setStyleSheet("QDialog { background-color: #f0f0f0; } QLabel { color: #000000; }");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *specEdit = new QLineEdit();
    QLineEdit *phoneEdit = new QLineEdit();
    QDoubleSpinBox *rateSpin = new QDoubleSpinBox();
    rateSpin->setRange(0, 10000);
    rateSpin->setPrefix("₽ ");
    if (row >= 0) {
        QSqlRecord rec = performersModel->record(row);
        nameEdit->setText(rec.value("full_name").toString());
        specEdit->setText(rec.value("specialty").toString());
        phoneEdit->setText(rec.value("phone").toString());
        rateSpin->setValue(rec.value("hourly_rate").toDouble());
    }
    layout->addWidget(new QLabel("ФИО:"));
    layout->addWidget(nameEdit);
    layout->addWidget(new QLabel("Специальность:"));
    layout->addWidget(specEdit);
    layout->addWidget(new QLabel("Телефон:"));
    layout->addWidget(phoneEdit);
    layout->addWidget(new QLabel("Ставка в час:"));
    layout->addWidget(rateSpin);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить");
    QPushButton *cancelBtn = new QPushButton("Отмена");

    saveBtn->setStyleSheet("QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #229954; }");
    cancelBtn->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 5px; padding: 8px 15px; font-weight: bold; } QPushButton:hover { background-color: #7f8c8d; }");

    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameEdit->text().isEmpty()) { QMessageBox::warning(&dialog, "Ошибка", "Введите ФИО"); return; }
        if (row < 0) performersModel->insertRow(performersModel->rowCount());
        int r = (row < 0) ? performersModel->rowCount() - 1 : row;
        performersModel->setData(performersModel->index(r, 1), nameEdit->text());
        performersModel->setData(performersModel->index(r, 2), specEdit->text());
        performersModel->setData(performersModel->index(r, 3), phoneEdit->text());
        performersModel->setData(performersModel->index(r, 4), rateSpin->value());
        if (performersModel->submitAll()) dialog.accept();
        else QMessageBox::critical(&dialog, "Ошибка", performersModel->lastError().text());
        loadPerformers();
    });
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::on_addPerformerButton_clicked() { showPerformerDialog(-1); }
void MainWindow::on_editPerformerButton_clicked() {
    int row = ui->performersTableView->currentIndex().row();
    if (row >= 0) showPerformerDialog(row);
    else QMessageBox::warning(this, "Ошибка", "Выберите исполнителя");
}
void MainWindow::on_deletePerformerButton_clicked() {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может удалять исполнителей");
        return;
    }
    int row = ui->performersTableView->currentIndex().row();
    if (row < 0) return;
    if (QMessageBox::question(this, "Удаление", "Удалить?") == QMessageBox::Yes) {
        performersModel->removeRow(row);
        performersModel->submitAll();
        loadPerformers();
    }
}

void MainWindow::showCustomerDialog(int row) {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может управлять заказчиками");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(row < 0 ? "Добавить заказчика" : "Редактировать заказчика");
    dialog.setStyleSheet("QDialog { background-color: #f0f0f0; } QLabel { color: #000000; }");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLineEdit *nameEdit = new QLineEdit();
    QLineEdit *phoneEdit = new QLineEdit();
    QLineEdit *emailEdit = new QLineEdit();
    QTextEdit *addressEdit = new QTextEdit();
    if (row >= 0) {
        QSqlRecord rec = customersModel->record(row);
        nameEdit->setText(rec.value("full_name").toString());
        phoneEdit->setText(rec.value("phone").toString());
        emailEdit->setText(rec.value("email").toString());
        addressEdit->setText(rec.value("address").toString());
    }
    layout->addWidget(new QLabel("ФИО:"));
    layout->addWidget(nameEdit);
    layout->addWidget(new QLabel("Телефон:"));
    layout->addWidget(phoneEdit);
    layout->addWidget(new QLabel("Email:"));
    layout->addWidget(emailEdit);
    layout->addWidget(new QLabel("Адрес:"));
    layout->addWidget(addressEdit);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Сохранить");
    QPushButton *cancelBtn = new QPushButton("Отмена");
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameEdit->text().isEmpty()) { QMessageBox::warning(&dialog, "Ошибка", "Введите ФИО"); return; }
        if (row < 0) customersModel->insertRow(customersModel->rowCount());
        int r = (row < 0) ? customersModel->rowCount() - 1 : row;
        customersModel->setData(customersModel->index(r, 1), nameEdit->text());
        customersModel->setData(customersModel->index(r, 2), phoneEdit->text());
        customersModel->setData(customersModel->index(r, 3), emailEdit->text());
        customersModel->setData(customersModel->index(r, 4), addressEdit->toPlainText());
        if (customersModel->submitAll()) dialog.accept();
        else QMessageBox::critical(&dialog, "Ошибка", customersModel->lastError().text());
        loadCustomers();
    });
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void MainWindow::on_addCustomerButton_clicked() { showCustomerDialog(-1); }
void MainWindow::on_editCustomerButton_clicked() {
    int row = ui->customersTableView->currentIndex().row();
    if (row >= 0) showCustomerDialog(row);
    else QMessageBox::warning(this, "Ошибка", "Выберите заказчика");
}
void MainWindow::on_deleteCustomerButton_clicked() {
    if (currentRole != "admin") {
        QMessageBox::warning(this, "Доступ запрещен", "Только администратор может удалять заказчиков");
        return;
    }
    int row = ui->customersTableView->currentIndex().row();
    if (row < 0) return;
    if (QMessageBox::question(this, "Удаление", "Удалить?") == QMessageBox::Yes) {
        customersModel->removeRow(row);
        customersModel->submitAll();
        loadCustomers();
    }
}
