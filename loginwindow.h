#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LogInWindow;
}

class LogInWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogInWindow(QWidget *parent = nullptr);
    ~LogInWindow();

private slots:
    void log_in_func();

signals:
    bool connect_try(const QString & login, const QString & passwd);

private:
    Ui::LogInWindow *ui;
};

#endif // LOGINWINDOW_H
