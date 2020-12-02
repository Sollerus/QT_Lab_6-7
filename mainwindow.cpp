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

    db.setHostName("localhost");
    db.setDatabaseName("lab_db");
    loginWin = new LogInWindow();
    connect(loginWin, &LogInWindow::connect_try, this, &MainWindow::setDBLoginAndPassword);
    loginWin->show();

    connect(ui->actionLogOut, &QAction::triggered, this, &MainWindow::changeUserAccount);
    connect(ui->actionAddNewUser, &QAction::triggered, this, &MainWindow::createNewUserDialog);

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

}









