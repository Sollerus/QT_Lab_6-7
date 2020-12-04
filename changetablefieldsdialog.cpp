#include "changetablefieldsdialog.h"
#include "ui_changetablefieldsdialog.h"

ChangeTableFieldsDialog::ChangeTableFieldsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeTableFieldsDialog)
{
    ui->setupUi(this);
}

ChangeTableFieldsDialog::~ChangeTableFieldsDialog()
{
    delete ui;
}
