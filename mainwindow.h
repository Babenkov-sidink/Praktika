#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int userId, QString fullName, QString role, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_addOrderButton_clicked();
    void on_editOrderButton_clicked();
    void on_deleteOrderButton_clicked();
    void on_refreshOrdersButton_clicked();
    void on_addWorkTypeButton_clicked();
    void on_editWorkTypeButton_clicked();
    void on_deleteWorkTypeButton_clicked();
    void on_addPerformerButton_clicked();
    void on_editPerformerButton_clicked();
    void on_deletePerformerButton_clicked();
    void on_addCustomerButton_clicked();
    void on_editCustomerButton_clicked();
    void on_deleteCustomerButton_clicked();
    void on_logoutButton_clicked();
    void on_searchOrdersButton_clicked();
    void on_searchEdit_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    int currentUserId;
    QString currentFullName;
    QString currentRole;
    QSqlTableModel *ordersModel;
    QSqlTableModel *workTypesModel;
    QSqlTableModel *performersModel;
    QSqlTableModel *customersModel;

    void setupModels();
    void loadOrders();
    void setupButtonStyles();
    void loadWorkTypes();
    void loadPerformers();
    void loadCustomers();
    void setPermissions();
    void refreshAll();
    void showOrderDialog(int row = -1);
    void showWorkTypeDialog(int row = -1);
    void showPerformerDialog(int row = -1);
    void showCustomerDialog(int row = -1);
    int getCurrentCustomerId();
};

#endif
