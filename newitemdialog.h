#ifndef NEWITEMDIALOG_H
#define NEWITEMDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class newItemDialog;
}

class newItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newItemDialog(QWidget *parent = 0);
    ~newItemDialog();

    QString project() const;
    QDateTime dueDate() const;
    QString priority() const;
    QString task() const;
    QString taskInfo() const;

private slots:
    void on_newItemButtonBox_accepted();

    void on_newItemButtonBox_rejected();

private:
    Ui::newItemDialog *ui;
};

#endif // NEWITEMDIALOG_H
