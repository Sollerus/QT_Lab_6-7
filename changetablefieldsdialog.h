#ifndef CHANGETABLEFIELDSDIALOG_H
#define CHANGETABLEFIELDSDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QList>

namespace Ui {
class ChangeTableFieldsDialog;
}

class ChangeTableFieldsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeTableFieldsDialog(QWidget *parent, QString creatorName = "");
    ChangeTableFieldsDialog(QList<QTableWidgetItem*>* listOfTableItems, QWidget *parent = nullptr);
    ~ChangeTableFieldsDialog();
    QStringList getFieldsResult();


private slots:
    void checkUpdatesCorrections();

    void on_endDateCheckBox_stateChanged(int arg1);

    void on_periodCheckBox_stateChanged(int arg1);

signals:
    bool pushUpdatedFieldsToDatabase(const QList <QWidget*>& fieldsList, const bool isCreating);

private:
    Ui::ChangeTableFieldsDialog *ui;
    const QStringList priorityList_{"high", "middle", "low"};
    const QStringList statusList_{"done", "in_the_process", "not_done"};
    //QList <QWidget*> enabledFields;
    QStringList fullNameStrList_;
    const bool isCreatingTask_;
    const QList<QTableWidgetItem*>* listOfTableItems_;
    QList<QWidget*> allFieldsList;
    void setAllFieldsList();
};

#endif // CHANGETABLEFIELDSDIALOG_H
