#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "loginwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    bool setDBLoginAndPassword(const QString& login, const QString& password);
    void changeUserAccount();
    void createNewUserDialog();
    void createNewUserInDatabase(const QString& login, const QString& password,
                                 const QString& role, const QString& firstName,
                                 const QString& lastName, const QString& patronymic);

private:
    Ui::MainWindow *ui;
    LogInWindow *loginWin;
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
};
#endif // MAINWINDOW_H
