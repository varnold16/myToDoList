#include "newitemdialog.h"
#include "ui_newitemdialog.h"

newItemDialog::newItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newItemDialog)
{
    ui->setupUi(this);
    ui->newDueDate->setDateTime(QDateTime::currentDateTime());
}

newItemDialog::~newItemDialog()
{
    delete ui;
}

// Bestätigung oder Abbruch ermöglichen

void newItemDialog::on_newItemButtonBox_accepted()
{
    accept();
}

void newItemDialog::on_newItemButtonBox_rejected()
{
    reject();
}

// Übergabe von Werten aus den Feldern

QString newItemDialog::project() const
{
    return ui->newProject->text();
}

QDateTime newItemDialog::dueDate() const
{
    return ui->newDueDate->dateTime();
}

QString newItemDialog::priority() const
{
    int priorityNumber = ui->newPriority->value();

    QString priorityString = QString::number(priorityNumber);

    return priorityString;
}

QString newItemDialog::task() const
{
    return ui->newTask->toPlainText();
}

QString newItemDialog::taskInfo() const
{
    return ui->newTaskInfo->text();
}
