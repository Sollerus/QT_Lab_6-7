#ifndef SINGUPDIALOG_H
#define SINGUPDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class SingUpDialog;
}

class SingUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingUpDialog(QWidget *parent = nullptr);
    ~SingUpDialog();

private slots:
    void createUserTry();
    void checkingLoginForCorrectness();
    void checkingPasswordForCorrectness();
    void checkingMathchingPasswords();
    void checkingFirstNameForCorrectness();
    void checkingLastNameForCorrectness();
    void checkingPatronymicForCorrectness();

signals:
    void addToDatabase(const QString& login, const QString& password,
                       const QString& role, const QString& firstName,
                       const QString& lastName, const QString& patronymic);
    void closeDialog();


private:
    Ui::SingUpDialog *ui;
    void removeSpacesAfterWord(QString& word) const;
    bool isCorrected(const QString& word);
};

#endif // SINGUPDIALOG_H
