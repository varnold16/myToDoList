#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void deleteList();

    void loadList(const QString &fileName);

    void disableSort();

    void on_actionNewList_triggered();

    void on_actionOpenList_triggered();

    void on_actionReloadList_triggered();

    void on_actionDeleteAllTasks_triggered();

    void on_buttonEditItem_clicked();

    void on_buttonDeleteItem_clicked();

    void on_buttonEnableSort_stateChanged(int arg1);

    void on_buttonInsertItem_clicked();

    void on_buttonFind_clicked();

    void on_buttonSaveList_clicked();

private:
    Ui::MainWindow *ui;

    enum column
    {
        PROJECT, DUEDATE, PRIORITY, TASK, TASKINFO
    };
};

#endif // MAINWINDOW_H
