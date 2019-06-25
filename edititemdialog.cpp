#include "edititemdialog.h"
#include "ui_edititemdialog.h"

editItemDialog::editItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editItemDialog)
{
    ui->setupUi(this);
    ui->editDueDate->setDateTime(QDateTime::currentDateTime());
}

editItemDialog::~editItemDialog()
{
    delete ui;
}

// Bestätigung oder Abbruch ermöglichen

void editItemDialog::on_editItemButtonBox_accepted()
{
    accept();
}

void editItemDialog::on_editItemButtonBox_rejected()
{
    reject();
}

// Übernahme von Werten (aus gewählter Zeile)

void editItemDialog::pasteFields(QString &project, QString &dueDateString, QString &priorityString, QString &task, QString &taskInfo)
{
    ui->editProject->setText(project);
    ui->editDueDate->setDateTime(QDateTime::fromString(dueDateString, "yyyy.MM.dd"));
    ui->editPriority->setValue(priorityString.toInt());
    ui->editTask->setText(task);
    ui->editTaskInfo->setText(taskInfo);
}

// Übergabe von Werten aus den Feldern

QString editItemDialog::project() const
{
    return ui->editProject->text();
}

QDateTime editItemDialog::dueDate() const
{
    return ui->editDueDate->dateTime();
}

QString editItemDialog::priority() const
{
    int priorityNumber = ui->editPriority->value();

    QString priorityString = QString::number(priorityNumber);

    return priorityString;
}

QString editItemDialog::task() const
{
    return ui->editTask->toPlainText();
}

QString editItemDialog::taskInfo() const
{
    return ui->editTaskInfo->text();
}

