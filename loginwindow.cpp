#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LogInWindow::LogInWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogInWindow)
{
    ui->setupUi(this);
    connect(ui->logInButton, &QPushButton::clicked, this, &LogInWindow::log_in_func);
}

LogInWindow::~LogInWindow()
{
    delete ui;
}

void LogInWindow::log_in_func(){
    bool isOpen = emit connect_try(ui->loginEdit->text(), ui->passwordEdit->text());
    if (isOpen){
        this->close();
    }
    else{
        QMessageBox mess(this);
        mess.setText("ERROR: Wrong login or password");
        mess.setPalette(this->palette());
        ui->loginEdit->clear();
        ui->passwordEdit->clear();
        mess.exec();
    }
}
