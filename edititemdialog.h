#ifndef EDITITEMDIALOG_H
#define EDITITEMDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class editItemDialog;
}

class editItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editItemDialog(QWidget *parent = 0);
    ~editItemDialog();

    void pasteFields(QString &project, QString &dueDateString, QString &priorityString, QString &task, QString &taskInfo);
    QString project() const;
    QDateTime dueDate() const;
    QString priority() const;
    QString task() const;
    QString taskInfo() const;

private slots:
    void on_editItemButtonBox_rejected();

    void on_editItemButtonBox_accepted();

private:
    Ui::editItemDialog *ui;
};

#endif // EDITITEMDIALOG_H
