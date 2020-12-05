#include "changetablefieldsdialog.h"
#include "ui_changetablefieldsdialog.h"
#include "mainwindow.h"

ChangeTableFieldsDialog::ChangeTableFieldsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeTableFieldsDialog),
    isCreatingTask_(true),
    listOfTableItems_(nullptr)
{
    ui->setupUi(this);
}

ChangeTableFieldsDialog::ChangeTableFieldsDialog(QList<QTableWidgetItem*>* listOfTableItems, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ChangeTableFieldsDialog),
    isCreatingTask_(false),
    listOfTableItems_(listOfTableItems)
{
    QStringList dateStr;
    for (int i = 0; i < listOfTableItems_->size(); i++){
        switch(i){
        case 0:
            ui->creatorLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 1:
            ui->executorLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 2:
            ui->contactLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 3:
            ui->descriptionTextEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 4:
            ui->contractLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 5:
            ui->equipmentLineEdit->setText(listOfTableItems_->at(i)->text());
            break;
        case 6:
            dateStr = listOfTableItems_->at(i)->text().split("-");
            ui->startDateEdit->setDate(QDate(dateStr[0].toInt(), dateStr[1].toInt(), dateStr[2].toInt()));
            break;
        case 7:
            dateStr = listOfTableItems_->at(i)->text().split("-");
            ui->endDateEdit->setDate(QDate(dateStr[0].toInt(), dateStr[1].toInt(), dateStr[2].toInt()));
            break;
        case 8:
            dateStr = listOfTableItems_->at(i)->text().split(" ");
            for (int j = 0; j < dateStr.size() - 1; j += 2){
                if (dateStr[j + 1] == "mon")
                    ui->mounthsSpinBox->setValue(dateStr[j].toInt());
                else if (dateStr[j + 1] == "day")
                    ui->daysSpinBox->setValue(dateStr[j].toInt());
            }
            if (dateStr[dateStr.size() - 1].contains(':')){
                auto time = dateStr[dateStr.size() - 1].split(":");
                ui->hoursTimeEdit->setTime(QTime(time[0].toInt(), time[1].toInt(), time[2].toInt()));
            }
            break;
        case 9:
            //ui->priorityComboBox->addItem(listOfTableItems_->at(i)->text());
//            for (auto& priorityStr : priorityList_){
//                ui->priorityComboBox->addItems()
//            }
            ui->priorityComboBox->addItems(priorityList_);
            ui->priorityComboBox->setCurrentText(listOfTableItems_->at(i)->text());
            break;
        case 10:
            ui->statusComboBox->addItems(statusList_);
            ui->statusComboBox->setCurrentText(listOfTableItems_->at(i)->text());
            break;
        }   
    }
    auto role = dynamic_cast<MainWindow*>(parent)->getRoleGroup();
    if (role == "worker"){
        ui->creatorLineEdit->setEnabled(false);
        ui->executorLineEdit->setEnabled(false);
        ui->contactLineEdit->setEnabled(false);
        ui->descriptionTextEdit->setEnabled(false);
        ui->contractLineEdit->setEnabled(false);
        ui->equipmentLineEdit->setEnabled(false);
        ui->creatorLineEdit->setEnabled(false);
        ui->startDateEdit->setEnabled(false);
        ui->daysSpinBox->setEnabled(false);
        ui->mounthsSpinBox->setEnabled(false);
        ui->hoursTimeEdit->setEnabled(false);
        ui->priorityComboBox->setEditable(false);
    }
    else if (role == "manager"){
        ui->creatorLineEdit->setEnabled(false);
    }
}

ChangeTableFieldsDialog::~ChangeTableFieldsDialog()
{
    delete ui;
}
