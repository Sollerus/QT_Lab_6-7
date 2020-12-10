#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singupdialog.h"
#include "changetablefieldsdialog.h"
#include <QSqlQuery>
#include <QDebug>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QSpinBox>
#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db.setHostName(HOST_NAME);
    db.setDatabaseName(DATABASE_NAME);
    loginWin = new LogInWindow();
    connect(loginWin, &LogInWindow::connect_try, this, &MainWindow::setDBLoginAndPassword);
    loginWin->show();

    connect(ui->actionLogOut, &QAction::triggered, this, &MainWindow::changeUserAccount);
    connect(ui->actionAddNewUser, &QAction::triggered, this, &MainWindow::createNewUserDialog);
    //connect(ui->tasksTableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::changeFieldValue);
    connect(ui->tasksTableWidget, &QTableWidget::itemDoubleClicked, this, &MainWindow::changeFieldValue);
    connect(ui->newTaskPushButton, &QPushButton::clicked, this, &MainWindow::createNewTaskButtonPressed);

    ui->tasksTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->employeesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->clientsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->contactsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->contractsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
        roleGroup_ = query.value(0).toString();
        ui->currentUserLabel->setText(login);
        ui->currentRoleLabel->setText(getRoleGroup());
        if(getRoleGroup() == "admin"){
            ui->actionAddNewUser->setVisible(true);
        }
        else {
            ui->actionAddNewUser->setVisible(false);
        }
        if(getRoleGroup() == "worker"){

        }
//        query.prepare("SELECT count(*) FROM information_schema.columns WHERE table_schema = 'public' AND table_name = 'tasks';");
//        //query.bindValue(":tasks", TASKS_TABLE_NAME);
//        query.exec();
//        query.next();
//       // qDebug() << "hello \"world\"";
//        qDebug() << query.value(0);
        showTable(ui->tasksTableWidget, TASKS_TABLE_NAME);
        showTable(ui->employeesTableWidget, EMPLOYEES_TABLE_NAME);
        showTable(ui->contactsTableWidget, CONTACTS_TABLE_NAME);
        showTable(ui->clientsTableWidget, CLIENTS_TABLE_NAME);
        showTable(ui->contractsTableWidget, CONTRACTS_TABLE_NAME);
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

bool MainWindow::createNewUserInDatabase(const QString& login, const QString& password,
                                         const QString& role, const QString& firstName,
                                         const QString& lastName, QString patronymic){
    QSqlQuery query;
    query.setForwardOnly(true);
    if (patronymic.isEmpty()){
        patronymic = "NULL";
    }
    else{
        patronymic = "'" + patronymic + "'";
    }
    query.exec("SELECT current_user");
    query.next();
    auto currentUser = query.value(0).toString();
    query.exec("BEGIN");
    auto result = query.exec("INSERT INTO " + EMPLOYEES_TABLE_NAME +
                  " VALUES (DEFAULT, '" + firstName +"', '" + lastName + "', " + patronymic + ", '" + login + "')");
    if (!result){
        query.exec("ROLLBACK");
        return false;
    }
    query.exec("SET ROLE admin");
    result = query.exec("CREATE USER " + login + " IN ROLE " + role + " ENCRYPTED PASSWORD '" + password + "'");
    if (!result){
        query.exec("ROLLBACK");
        return false;
    }
    result = query.exec("SET ROLE " + currentUser);
    if (result)
        return true;
    else{
        query.exec("ROLLBACK");
        return false;
    }
}


void MainWindow::showTable(QTableWidget* table, const QString sqlTableName){
    QSqlQuery sqlQuery;
    sqlQuery.setForwardOnly(true);
    sqlQuery.exec("SELECT count(*) FROM information_schema.columns WHERE table_schema = 'public' AND table_name = '" +
                     sqlTableName + "';");
    sqlQuery.next();
    const auto columnCount = sqlQuery.value(0).toInt();
    table->setColumnCount(columnCount);
    sqlQuery.exec("SELECT count(*) FROM " + sqlTableName + ";");
    sqlQuery.next();
    table->setRowCount(sqlQuery.value(0).toInt());
    if (sqlTableName == TASKS_TABLE_NAME)
        sqlQuery.exec("SELECT t.id, e.first_name || ' ' || e.last_name as Creator, e2.first_name || ' ' || e2.last_name as Executor, "
                         "c.first_name || ' ' || c.last_name as Contact, t.description, t.contract, t.equipment_id, "
                         "t.start_date, t.end_date, t.period_of_exec, t.priority, t.status "
                         "FROM " + TASKS_TABLE_NAME + " t "
                                                     "JOIN " + EMPLOYEES_TABLE_NAME + " e on e.id = t.creator "
                                                     "JOIN " + EMPLOYEES_TABLE_NAME + " e2 on e2.id = t.executor "
                                                     "JOIN " + CONTACTS_TABLE_NAME + " c on c.id = t.contact;");
    else if (sqlTableName == EMPLOYEES_TABLE_NAME)
        sqlQuery.exec("SELECT * FROM " + EMPLOYEES_TABLE_NAME);
    else if (sqlTableName == CLIENTS_TABLE_NAME)
        sqlQuery.exec("SELECT * FROM " + CLIENTS_TABLE_NAME);
    else if (sqlTableName == CONTACTS_TABLE_NAME)
        sqlQuery.exec("SELECT c.id, c.first_name, c.last_name, c.patronymic, "
                      "cl.company_name, c.phone_number, c.email, c.fax, c.post_address FROM " +
                      CONTACTS_TABLE_NAME + " AS c JOIN " + CLIENTS_TABLE_NAME + " cl ON c.company_id = cl.id");
    else if (sqlTableName == CONTRACTS_TABLE_NAME)
        sqlQuery.exec("SELECT cr.id, cl.company_name, cr.description FROM " +
                      CONTRACTS_TABLE_NAME + " AS cr JOIN " + CLIENTS_TABLE_NAME + "cl ON cr.company_id = cl.id");
    unsigned short rowCount{};

    while (sqlQuery.next()){
        for (int i = 0; i < columnCount; i++){
            table->setItem(rowCount, i, new QTableWidgetItem(sqlQuery.value(i).toString()));
        }
        ++rowCount;
    }
}


void MainWindow::changeFieldValue(QTableWidgetItem* cellTableWidgetItem){
    auto qListOfTableWigetItems = ui->tasksTableWidget->selectedItems();
    ChangeTableFieldsDialog* dialog = new ChangeTableFieldsDialog(&qListOfTableWigetItems, this);
    connect(dialog, &ChangeTableFieldsDialog::pushUpdatedFieldsToDatabase, this, &MainWindow::createOrUpdateQuery);
    if(dialog->exec() == QDialog::Accepted)
        showTable(ui->tasksTableWidget, TASKS_TABLE_NAME);
    delete dialog;

}

bool MainWindow::createOrUpdateQuery(const QList <QWidget*>& fieldsList, const bool isCreating){

    QSqlQuery sqlQuery;
    QString idValue, creatorId, executorId, contactId, endDateValue, periodValue,
            descriptionValue, contractValue, equipmentValue, startDateValue, statusValue, priorityValue;
    if (dynamic_cast<QCheckBox*>(fieldsList[ColumnsIndexes::endDateCheckBox])->isChecked())
        endDateValue = "'" + dynamic_cast<QDateEdit*>(fieldsList[ColumnsIndexes::endDate])->text() + "'";
    else
        endDateValue = "NULL";
    
    if (dynamic_cast<QCheckBox*>(fieldsList[ColumnsIndexes::periodCheckBox])->isChecked()){
        periodValue = "'" + dynamic_cast<QAbstractSpinBox*>(fieldsList[ColumnsIndexes::monthsPeriodOfExecution])->text() + " months " +
                dynamic_cast<QAbstractSpinBox*>(fieldsList[ColumnsIndexes::daysPeriodOfExecution])->text() + " days " +
                dynamic_cast<QAbstractSpinBox*>(fieldsList[ColumnsIndexes::hoursPeriodOfExecution])->text().split(":")[0] + " hours'";
    }
    else{
        periodValue = "NULL";
    }
    if (!dynamic_cast<QTextEdit*>(fieldsList[ColumnsIndexes::description])->toPlainText().isEmpty()){
        descriptionValue = "'" + dynamic_cast<QTextEdit*>(fieldsList[ColumnsIndexes::description])->toPlainText() + "'";
    }
    else{
        descriptionValue = "NULL";
    }
    if (!dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::contractId])->text().isEmpty()){
        contractValue = dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::contractId])->text();
    }
    else{
        contractValue = "NULL";
    }
    if (!dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::equipmentId])->text().isEmpty()){
        equipmentValue = dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::equipmentId])->text();
    }
    else{
        equipmentValue = "NULL";
    }
    auto result = sqlQuery.exec("SELECT id FROM " + EMPLOYEES_TABLE_NAME + " WHERE first_name = '"
                  + dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::creatorName])->text().split(" ")[0] +
            "' AND last_name = '" + dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::creatorName])->text().split(" ")[1] + "'");
    if (!result){
        return false;
    }
    sqlQuery.next();
    creatorId = sqlQuery.value(0).toString();
    result = sqlQuery.exec("SELECT id FROM " + EMPLOYEES_TABLE_NAME + " WHERE first_name = '" +
                  dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::executorName])->text().split(" ")[0] +
            "' AND last_name = '" + dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::executorName])->text().split(" ")[1] + "'");
    if (!result){
        return false;
    }
    sqlQuery.next();
    executorId = sqlQuery.value(0).toString();
    result = sqlQuery.exec("SELECT id FROM " + CONTACTS_TABLE_NAME + " WHERE first_name = '" +
                  dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::contactName])->text().split(" ")[0] +
            "' AND last_name = '" + dynamic_cast<QLineEdit*>(fieldsList[ColumnsIndexes::contactName])->text().split(" ")[1] + "'");
    if (!result){
        return false;
    }
    sqlQuery.next();
    contactId = sqlQuery.value(0).toString();
    idValue = dynamic_cast<QAbstractSpinBox*>(fieldsList[ColumnsIndexes::id])->text();
    startDateValue = "'" + dynamic_cast<QDateEdit*>(fieldsList[ColumnsIndexes::startDate])->text() + "'";
    priorityValue = "'" + dynamic_cast<QComboBox*>(fieldsList[ColumnsIndexes::priority])->currentText() + "'";
    statusValue = "'" +  dynamic_cast<QComboBox*>(fieldsList[ColumnsIndexes::status])->currentText() + "'";

    if (isCreating){
        result = sqlQuery.exec("INSERT INTO " + TASKS_TABLE_NAME + " VALUES (" +
                      idValue + ", " + creatorId + ", " + executorId + ", " + contactId + ", " +
                      descriptionValue + ", " + contractValue + ", " + equipmentValue + ", "+
                      startDateValue + ", " + endDateValue + ", " + periodValue + ", " +
                      priorityValue + ", " + statusValue + ")");
    }
    else{
        QString sets;
        if (getRoleGroup() == "worker"){
            sets = "SET end_date = " + endDateValue +
                    ", status = " + statusValue;
        }
        else if (getRoleGroup() == "manager"){
            sets = "SET executor = " + executorId +
                    ", end_date = " + endDateValue +
                    ", status = " + statusValue;
        }
        else{
            sets = "SET id = " + idValue +
                    ", creator = " + creatorId +
                    ", executor = " + executorId +
                    ", contact = " + contactId +
                    ", description = " + descriptionValue +
                    ", contract = " + contractValue +
                    ", equipment_id = " + equipmentValue +
                    ", start_date = " + startDateValue +
                    ", end_date = " + endDateValue +
                    ", period_of_exec = " + periodValue +
                    ", priority = " + priorityValue +
                    ", status = " + statusValue;
        }
        result = sqlQuery.exec("UPDATE " + TASKS_TABLE_NAME + " " +
                      sets + " WHERE id = " +  /*dynamic_cast<QAbstractSpinBox*>(fieldsList[ColumnsIndexes::id])->text()*/
                               ui->tasksTableWidget->selectedItems().at(0)->text());
    }
    if (result)
        return true;
    else{
        qDebug() << "EEEEEEERRRRROOOOOR";
        return false;
    }
}

void MainWindow::createNewTaskButtonPressed(){
    QSqlQuery sqlQuery("SELECT first_name || ' ' || last_name FROM " + EMPLOYEES_TABLE_NAME + " WHERE position = current_role");
    sqlQuery.next();
    ChangeTableFieldsDialog* dialog = new ChangeTableFieldsDialog(this, sqlQuery.value(0).toString());
    connect(dialog, &ChangeTableFieldsDialog::pushUpdatedFieldsToDatabase, this, &MainWindow::createOrUpdateQuery);
    if(dialog->exec() == QDialog::Accepted)
        showTable(ui->tasksTableWidget, TASKS_TABLE_NAME);
    delete dialog;
}






