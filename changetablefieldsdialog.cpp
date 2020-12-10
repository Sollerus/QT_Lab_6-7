#include "changetablefieldsdialog.h"
#include "ui_changetablefieldsdialog.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>



ChangeTableFieldsDialog::ChangeTableFieldsDialog(QWidget *parent, QString creatorName) :
    QDialog(parent),
    ui(new Ui::ChangeTableFieldsDialog),
    isCreatingTask_(true),
    listOfTableItems_(nullptr)
{
    ui->setupUi(this);
    ui->idSpinBox->setEnabled(true);
    ui->executorLineEdit->setEnabled(true);
    ui->contactLineEdit->setEnabled(true);
    ui->descriptionTextEdit->setEnabled(true);
    ui->contractLineEdit->setEnabled(true);
    ui->equipmentLineEdit->setEnabled(true);
    ui->startDateEdit->setEnabled(true);
    ui->endDateEdit->setEnabled(true);
    ui->monthsSpinBox->setEnabled(true);
    ui->daysSpinBox->setEnabled(true);
    ui->hoursTimeEdit->setEnabled(true);
    ui->priorityComboBox->setEnabled(true);
    ui->statusComboBox->setEnabled(true);
    ui->periodCheckBox->setChecked(true);
    ui->endDateCheckBox->setChecked(true);
    ui->priorityComboBox->addItems(priorityList_);
    ui->statusComboBox->addItems(statusList_);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ChangeTableFieldsDialog::checkUpdatesCorrections);
    ui->creatorLineEdit->setText(creatorName);
    if (dynamic_cast<MainWindow*>(parent)->getRoleGroup() == "manager"){
        ui->creatorLineEdit->setEnabled(false);
    }
    else
        ui->creatorLineEdit->setEnabled(true);
    setAllFieldsList();

}

ChangeTableFieldsDialog::ChangeTableFieldsDialog(QList<QTableWidgetItem*>* listOfTableItems, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ChangeTableFieldsDialog),
    isCreatingTask_(false),
    listOfTableItems_(listOfTableItems)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ChangeTableFieldsDialog::checkUpdatesCorrections);
    setAllFieldsList();
    auto role = dynamic_cast<MainWindow*>(parent)->getRoleGroup();
    if (role == "worker"){
        ui->endDateEdit->setEnabled(true);
        ui->statusComboBox->setEnabled(true);
    }
    else if (role == "manager"){
        ui->executorLineEdit->setEnabled(true);
        ui->endDateEdit->setEnabled(true);
        ui->statusComboBox->setEnabled(true);
    }
    else{
        ui->idSpinBox->setEnabled(true);
        ui->creatorLineEdit->setEnabled(true);
        ui->executorLineEdit->setEnabled(true);
        ui->contactLineEdit->setEnabled(true);
        ui->descriptionTextEdit->setEnabled(true);
        ui->contractLineEdit->setEnabled(true);
        ui->equipmentLineEdit->setEnabled(true);
        ui->startDateEdit->setEnabled(true);
        ui->endDateEdit->setEnabled(true);
        ui->monthsSpinBox->setEnabled(true);
        ui->daysSpinBox->setEnabled(true);
        ui->hoursTimeEdit->setEnabled(true);
        ui->priorityComboBox->setEnabled(true);
        ui->statusComboBox->setEnabled(true);
    }
    QStringList dateStr;
    for (int i = 0; i < listOfTableItems_->size(); i++){
        switch(i){
        case 0:
            ui->idSpinBox->setValue(listOfTableItems_->at(i)->text().toInt());
            break;
        case 1:
            ui->creatorLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 2:
            ui->executorLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 3:
            ui->contactLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 4:
            ui->descriptionTextEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 5:
            if(listOfTableItems_->at(i)->text() != "0")
                ui->contractLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 6:
            if(listOfTableItems_->at(i)->text() != "0")
                ui->equipmentLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 7:
            dateStr = listOfTableItems_->at(i)->text().split("-");
            ui->startDateEdit->setDate(QDate(dateStr[0].toInt(), dateStr[1].toInt(), dateStr[2].toInt()));
            break;
        case 8:
            if (!listOfTableItems_->at(i)->text().isEmpty()){
                ui->endDateCheckBox->setChecked(true);
                dateStr = listOfTableItems_->at(i)->text().split("-");
                ui->endDateEdit->setDate(QDate(dateStr[0].toInt(), dateStr[1].toInt(), dateStr[2].toInt()));
            }
            else{
                ui->endDateCheckBox->setChecked(false);
                ui->endDateEdit->setEnabled(false);
            }
            break;
        case 9:
            if (listOfTableItems_->at(i)->text().isEmpty()){
                ui->monthsSpinBox->setEnabled(false);
                ui->daysSpinBox->setEnabled(false);
                ui->hoursTimeEdit->setEnabled(false);
            }
            else{
                dateStr = listOfTableItems_->at(i)->text().split(" ");
                for (int j = 0; j < dateStr.size() - 1; j += 2){
                    if (dateStr[j + 1] == "mon")
                        ui->monthsSpinBox->setValue(dateStr[j].toInt());
                    else if (dateStr[j + 1] == "day")
                        ui->daysSpinBox->setValue(dateStr[j].toInt());
                }
                if (dateStr[dateStr.size() - 1].contains(':')){
                    auto time = dateStr[dateStr.size() - 1].split(":");
                    ui->hoursTimeEdit->setTime(QTime(time[0].toInt(), time[1].toInt(), time[2].toInt()));
                }
            }
            break;
        case 10:
            ui->priorityComboBox->addItems(priorityList_);
            ui->priorityComboBox->setCurrentText(listOfTableItems_->at(i)->text());
            break;
        case 11:
            ui->statusComboBox->addItems(statusList_);
            ui->statusComboBox->setCurrentText(listOfTableItems_->at(i)->text());
            break;
        }
    }

}

ChangeTableFieldsDialog::~ChangeTableFieldsDialog()
{
    delete ui;
}

void ChangeTableFieldsDialog::checkUpdatesCorrections(){
    if (ui->creatorLineEdit->text().isEmpty() || ui->executorLineEdit->text().isEmpty() ||
            ui->contactLineEdit->text().isEmpty() || ui->statusComboBox->currentText().isEmpty() ||
            ui->priorityComboBox->currentText().isEmpty() ||
            (ui->statusComboBox->currentText() == "done" && !ui->endDateCheckBox->isChecked())){
        QMessageBox message(this);
        message.setWindowTitle("Error");
        message.setText("Error: Not all required fields are filled");
        message.exec();
    }
    else if (emit pushUpdatedFieldsToDatabase(allFieldsList, isCreatingTask_)){
        emit accept();
    }
    else{
        QMessageBox message(this);
        message.setWindowTitle("Error");
        message.setText("Error: Some data incorrect");
        message.exec();
    }
}

void ChangeTableFieldsDialog::setAllFieldsList(){
    allFieldsList.push_back(ui->idSpinBox);
    allFieldsList.push_back(ui->creatorLineEdit);
    allFieldsList.push_back(ui->executorLineEdit);
    allFieldsList.push_back(ui->contactLineEdit);
    allFieldsList.push_back(ui->descriptionTextEdit);
    allFieldsList.push_back(ui->contractLineEdit);
    allFieldsList.push_back(ui->equipmentLineEdit);
    allFieldsList.push_back(ui->startDateEdit);
    allFieldsList.push_back(ui->endDateEdit);
    allFieldsList.push_back(ui->monthsSpinBox);
    allFieldsList.push_back(ui->daysSpinBox);
    allFieldsList.push_back(ui->hoursTimeEdit);
    allFieldsList.push_back(ui->priorityComboBox);
    allFieldsList.push_back(ui->statusComboBox);
    allFieldsList.push_back(ui->endDateCheckBox);
    allFieldsList.push_back(ui->periodCheckBox);
}


void ChangeTableFieldsDialog::on_endDateCheckBox_stateChanged(int arg1)
{
    auto checked = (Qt::Checked == arg1);
    ui->endDateEdit->setEnabled(checked);
}



void ChangeTableFieldsDialog::on_periodCheckBox_stateChanged(int arg1)
{
    auto checked = (Qt::Checked == arg1);
    ui->monthsSpinBox->setEnabled(checked);
    ui->daysSpinBox->setEnabled(checked);
    ui->hoursTimeEdit->setEnabled(checked);
}
