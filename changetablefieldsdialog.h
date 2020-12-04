#ifndef CHANGETABLEFIELDSDIALOG_H
#define CHANGETABLEFIELDSDIALOG_H

#include <QDialog>

namespace Ui {
class ChangeTableFieldsDialog;
}

class ChangeTableFieldsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeTableFieldsDialog(QWidget *parent = nullptr);
    ~ChangeTableFieldsDialog();

private:
    Ui::ChangeTableFieldsDialog *ui;
};

#endif // CHANGETABLEFIELDSDIALOG_H
