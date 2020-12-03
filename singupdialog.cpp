#include "singupdialog.h"
#include "ui_singupdialog.h"

SingUpDialog::SingUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingUpDialog)
{
    ui->setupUi(this);
    ui->notCorrectLoginLabel->hide();
    ui->notCorrectPasswordLabel->hide();
    ui->notCorrectPasswordAgainLabel->hide();
    ui->notSelectedRoleLabel->hide();
    ui->firstNameErrorLabel->hide();
    ui->lastNameErrorLabel->hide();

    ui->RolesComboBox->addItem("admin");
    ui->RolesComboBox->addItem("worker");
    ui->RolesComboBox->addItem("manager");

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SingUpDialog::createUserTry);
    connect(ui->NewLoginLineEdit, &QLineEdit::editingFinished, this, &SingUpDialog::checkingLoginForCorrectness);
    connect(ui->NewPasswordLineEdit, &QLineEdit::editingFinished, this, &SingUpDialog::checkingPasswordForCorrectness);
    connect(ui->NewPasswordAgainLineEdit, &QLineEdit::editingFinished, this, &SingUpDialog::checkingMathchingPasswords);
    connect(this, &SingUpDialog::closeDialog, this, &SingUpDialog::accept);
    connect(ui->firstNameLineEdit, &QLineEdit::editingFinished, this, &SingUpDialog::checkingFirstNameForCorrectness);
    connect(ui->lastNameLineEdit, &QLineEdit::editingFinished, this, &SingUpDialog::checkingLastNameForCorrectness);
    connect(ui->patronymicLineEdit, &QLineEdit::editingFinished, this, &SingUpDialog::checkingPatronymicForCorrectness);
}

SingUpDialog::~SingUpDialog()
{
    delete ui;
}

void SingUpDialog::removeSpacesAfterWord(QString& word) const{
    short i = word.length() - 1;
    short count = 1;
    while(word[i] == ' '){
        --i;
        ++count;
    }
    word.remove(i + 1, count);
}

bool SingUpDialog::isCorrected(const QString& word){
    for(auto& x : word){
        if (!x.isLetter() || ' '){
            return false;
        }
    }
    return true;
}

void SingUpDialog::createUserTry(){
    bool allCorrect = true;
    if (ui->firstNameLineEdit->text().isEmpty()){
        ui->firstNameErrorLabel->show();
        allCorrect = false;
    }
    if (ui->lastNameLineEdit->text().isEmpty()){
        ui->lastNameErrorLabel->show();
        allCorrect = false;
    }
    if (ui->NewLoginLineEdit->text().isEmpty()){
        ui->notCorrectLoginLabel->show();
        allCorrect = false;
    }
    if (ui->NewPasswordLabel->text().isEmpty()){
        ui->notCorrectPasswordLabel->show();
        allCorrect = false;
    }
    if (ui->NewPasswordAgainLineEdit->text().isEmpty()){
        ui->notCorrectPasswordAgainLabel->show();
        allCorrect = false;
    }
    if (ui->RolesComboBox->currentText().isEmpty()){
        ui->notSelectedRoleLabel->show();
        allCorrect = false;
    }
    if (allCorrect){
        ui->notSelectedRoleLabel->hide();
        emit addToDatabase(ui->NewLoginLineEdit->text(), ui->NewPasswordLineEdit->text(),
                           ui->RolesComboBox->currentText().toLower(), ui->firstNameLineEdit->text(),
                           ui->lastNameLineEdit->text(), ui->patronymicLineEdit->text());
        emit close();
    }
}

void SingUpDialog::checkingLoginForCorrectness(){
    auto login = ui->NewLoginLineEdit->text();
    if(!login.isEmpty()){
        removeSpacesAfterWord(login);
        ui->NewLoginLineEdit->setText(login);
    }
    if(login.contains(' ', Qt::CaseInsensitive) || login.isEmpty() || login.length() < 5){
        ui->notCorrectLoginLabel->show();
        ui->buttonBox->setDisabled(true);
    }
    else{
        ui->buttonBox->setEnabled(true);
        ui->notCorrectLoginLabel->hide();
    }
}

void SingUpDialog::checkingPasswordForCorrectness(){
    auto password = ui->NewLoginLineEdit->text();
    if(password.contains(' ', Qt::CaseInsensitive) || password.isEmpty() || password.length() < 8){
        ui->notCorrectPasswordLabel->show();
        ui->buttonBox->setDisabled(true);
    }
    else{
        ui->buttonBox->setEnabled(true);
        ui->notCorrectPasswordLabel->hide();
    }
}

void SingUpDialog::checkingMathchingPasswords(){
    if (ui->NewPasswordLineEdit->text() != ui->NewPasswordAgainLineEdit->text()){
        ui->notCorrectPasswordAgainLabel->show();
        ui->buttonBox->setDisabled(true);
    }
    else{
        ui->notCorrectPasswordAgainLabel->hide();
        ui->buttonBox->setEnabled(true);
    }
}

void SingUpDialog::checkingFirstNameForCorrectness(){
    auto name = ui->firstNameLineEdit->text();
    if(!name.isEmpty()){
        removeSpacesAfterWord(name);
        ui->firstNameLineEdit->setText(name);
        if(isCorrected(name)){
            ui->firstNameErrorLabel->show();
            ui->buttonBox->setDisabled(true);
            return;
        }

    }
    ui->firstNameErrorLabel->hide();
    ui->buttonBox->setEnabled(true);


}

void SingUpDialog::checkingLastNameForCorrectness(){
    auto name = ui->lastNameLineEdit->text();
    if(!name.isEmpty()){
        removeSpacesAfterWord(name);
        ui->lastNameLineEdit->setText(name);
        if(isCorrected(name)){
            ui->lastNameErrorLabel->show();
            ui->buttonBox->setDisabled(true);
            return;
        }
    }
    ui->lastNameErrorLabel->hide();
    ui->buttonBox->setEnabled(true);
}

void SingUpDialog::checkingPatronymicForCorrectness(){
    auto name = ui->patronymicLineEdit->text();
    if (!name.isEmpty()){
        removeSpacesAfterWord(name);
        ui->patronymicLineEdit->setText(name);
        if(isCorrected(name)){
            ui->patronymicErrorLabel->show();
            ui->buttonBox->setDisabled(true);
            return;
        }
    }
    ui->patronymicErrorLabel->hide();
    ui->buttonBox->setEnabled(true);

}

//void Sing








