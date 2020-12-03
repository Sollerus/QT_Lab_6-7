#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singupdialog.h"
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db.setHostName(HOST_NAME);
    db.setDatabaseName(DATABASE_NAME);
//    loginWin = new LogInWindow();
//    connect(loginWin, &LogInWindow::connect_try, this, &MainWindow::setDBLoginAndPassword);
//    loginWin->show();

    connect(ui->actionLogOut, &QAction::triggered, this, &MainWindow::changeUserAccount);
    connect(ui->actionAddNewUser, &QAction::triggered, this, &MainWindow::createNewUserDialog);

    db.setUserName("manager_1");
    db.setPassword("123");
    db.open();
    this->show();
    showTasksTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::setDBLoginAndPassword(const QString & login, const QString & password){
    db.setUserName(login);
    db.setPassword(password);
    if (db.open()){
        this->show();
        QSqlQuery query("SELECT rolname"
                        " FROM pg_user JOIN pg_auth_members ON (pg_user.usesysid=pg_auth_members.member)"
                        " JOIN pg_roles ON (pg_roles.oid=pg_auth_members.roleid) WHERE pg_user.usename=current_user;");
        query.next();
        qDebug() << query.value(0);
        if(query.value(0) == "admin"){
            qDebug() << "Work";
            ui->actionAddNewUser->setVisible(true);
        }
        else {
            ui->actionAddNewUser->setVisible(false);
        }
//        query.prepare("SELECT count(*) FROM information_schema.columns WHERE table_schema = 'public' AND table_name = 'tasks';");
//        //query.bindValue(":tasks", TASKS_TABLE_NAME);
//        query.exec();
//        query.next();
//       // qDebug() << "hello \"world\"";
//        qDebug() << query.value(0);
        showTasksTable();
        return true;
    }
    else return false;
}

void MainWindow::changeUserAccount(){
    db.close();
    this->hide();
    loginWin->show();
}

void MainWindow::createNewUserDialog(){
    SingUpDialog* singUpDialog = new SingUpDialog(this);
    connect(singUpDialog, &SingUpDialog::addToDatabase, this, &MainWindow::createNewUserInDatabase);
    singUpDialog->exec();
    delete singUpDialog;
}

void MainWindow::createNewUserInDatabase(const QString& login, const QString& password,
                                         const QString& role, const QString& firstName,
                                         const QString& lastName, const QString& patronymic){
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("INSERT INTO employees"
                  "VALUES (DEFAULT, :first_name, :last_name, :patronymic, :position)");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":patronymic", patronymic);
    query.bindValue(":position", login);
    query.exec();
    query.prepare("CREATE USER :login IN ROLE :role ENCRYPTED PASSWORD :password");
    query.bindValue(":login", login);
    query.bindValue(":role", role);
    query.bindValue(":password", password);
    query.exec();
}

void MainWindow::showTasksTable(){
    auto table = ui->tasksTableWidget;
    QSqlQuery sqlQuery;
    sqlQuery.setForwardOnly(true);
    sqlQuery.prepare("SELECT count(*) FROM information_schema.columns WHERE table_schema = 'public' AND table_name = '" +
                     TASKS_TABLE_NAME + "';");
   // sqlQuery.bindValue(":tasks", TASKS_TABLE_NAME);
   // sqlQuery.addBindValue(TASKS_TABLE_NAME);
    sqlQuery.exec();
    //qDebug() << sqlQuery.isSelect();
    sqlQuery.next();
    const auto columnCountWithoutId = sqlQuery.value(0).toInt() - 1;
    table->setRowCount(10);

    table->setColumnCount(columnCountWithoutId);
    //table->setVerticalHeaderLabels(QStringList({"name", "last"}));
    table->setHorizontalHeaderLabels(QStringList({"Task creator", "Task executor", "Contact name", "Description",
                                                  "Contract id", "Equipment id", "Start date", "End date",
                                                  "Period of execution", "Priority", "Status"}));

    sqlQuery.prepare("SELECT e.first_name || ' ' || e.last_name as Creator, e2.first_name || ' ' || e2.last_name as Executor, "
                     "c.first_name || ' ' || c.last_name as Contact, t.description, t.contract, t.equipment_id, "
                     "t.start_date, t.end_date, t.period_of_exec, t.priority, t.status "
                     "FROM " + TASKS_TABLE_NAME + " t "
                                                 "JOIN " + EMPLOYEES_TABLE_NAME + " e on e.id = t.creator "
                                                 "JOIN " + EMPLOYEES_TABLE_NAME + " e2 on e2.id = t.executor "
                                                 "JOIN " + CONTACTS_TABLE_NAME + " c on c.id = t.contact;");

//    sqlQuery.bindValue(":employees", EMPLOYEES_TABLE_NAME);
//    sqlQuery.bindValue(":tasks", TASKS_TABLE_NAME);
//    sqlQuery.bindValue(":contacts", CONTACTS_TABLE_NAME);
    sqlQuery.exec();
    unsigned short rowCount{};
//    qDebug() << sqlQuery.isSelect();

//    sqlQuery.next();
//    qDebug() << sqlQuery.isValid();
//    auto x = sqlQuery.result();
//    qDebug() << sqlQuery.value(0);
//    qDebug() << sqlQuery.lastQuery();
    while (sqlQuery.next()){
        //table->setRowCount(1 + rowCount);
        for (int i = 0; i < columnCountWithoutId; i++){
            qDebug() << sqlQuery.value(i);
            table->setItem(rowCount, i, new QTableWidgetItem(sqlQuery.value(i).toString()));
        }
        ++rowCount;
    }


}








