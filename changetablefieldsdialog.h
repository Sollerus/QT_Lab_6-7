#ifndef CHANGETABLEFIELDSDIALOG_H
#define CHANGETABLEFIELDSDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class ChangeTableFieldsDialog;
}

class ChangeTableFieldsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeTableFieldsDialog(QWidget *parent = nullptr);
    ChangeTableFieldsDialog(QList<QTableWidgetItem*>* listOfTableItems, QWidget *parent = nullptr);
    ~ChangeTableFieldsDialog();

private slots:
    void returnResultsToDB();

private:
    Ui::ChangeTableFieldsDialog *ui;
    const QStringList priorityList_{"high", "middle", "low"};
    const QStringList statusList_{"done", "in_the_process", "not_done"};
    const bool isCreatingTask_;
    const QList<QTableWidgetItem*>* listOfTableItems_;
};

#endif // CHANGETABLEFIELDSDIALOG_H
