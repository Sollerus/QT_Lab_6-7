#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include "loginwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

static const QString TASKS_TABLE_NAME = "tasks";
static const QString EMPLOYEES_TABLE_NAME = "employees";
static const QString CONTACTS_TABLE_NAME = "contacts";
static const QString CLIENTS_TABLE_NAME = "clients";
static const QString CONTRACTS_TABLE_NAME = "contracts";
static const QString HOST_NAME = "localhost";
static const QString DATABASE_NAME = "lab_db";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getRoleGroup() const noexcept {return roleGroup_;}

private slots:
    bool setDBLoginAndPassword(const QString& login, const QString& password);
    void changeUserAccount();
    void createNewUserDialog();
    void createNewUserInDatabase(const QString& login, const QString& password,
                                 const QString& role, const QString& firstName,
                                 const QString& lastName, const QString& patronymic);

    void changeFieldValue(QTableWidgetItem* cellTableWidget);
    void updateQuery();

private:
    Ui::MainWindow *ui;
    enum ColumnsIndexes{creatorName, executorName, contactName, description, contractId, equipmentId, startDate, endDate, periodOfExecution, priority, status};

    LogInWindow *loginWin;
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    QString roleGroup_;

    void showTasksTable();
};
#endif // MAINWINDOW_H
