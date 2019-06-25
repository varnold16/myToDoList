#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newitemdialog.h"
#include "edititemdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Meine Aufgaben-Liste");

    // einrichten der Tabelle - tableTask beinhaltet in Folge alle Aufgaben
    ui->tableTasks->setColumnCount(5);
    QStringList columnNames;
    columnNames << "Projekt" << "Fälligkeit" << "Prioriät" << "Aufgabe" << "Siehe";
    ui->tableTasks->setHorizontalHeaderLabels(columnNames);
    ui->tableTasks->horizontalHeader()->resizeSection(0, 100);
    ui->tableTasks->horizontalHeader()->resizeSection(1, 100);
    ui->tableTasks->horizontalHeader()->resizeSection(2, 50);
    ui->tableTasks->horizontalHeader()->resizeSection(3, 450);
    ui->tableTasks->horizontalHeader()->resizeSection(4, 250);

    // "Default"-Speicherpfad ins Feld eintragen
    ui->pathToFile->setText(QDir::homePath()+"/neueToDoListe.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disableSort()
{
    // für manche Aktionen (insbesondere beim befüllen der Tabelle),
    // sollte Sortieren inaktiviert sein, da es sonst zu falschen Wertzuweisungen kommen kann.

    ui->buttonEnableSort->setChecked(false);
}

void MainWindow::deleteList()
{
    // löscht alle Zeilen

    disableSort();
    ui->tableTasks->clearContents();
    ui->tableTasks->setRowCount(0);
}

void MainWindow::loadList(const QString &fileName)
{
    // Funktion wird Dateipfad übergeben
    // Datei wird gelesen und Tabelle befüllt

    disableSort();

    // leerer Dateipfad/keine Text-Datei -> Abbruch
    if (fileName.isEmpty())
    {
        return;
    } else if (!fileName.endsWith(".txt")) {
        QMessageBox::critical(this, fileName,
                              "Datei konnte nicht geöffnet werden!\nEs können nur *.txt-Dateien geöffnet werden.");
        return;
    }

    // Datei öffnen und Dateipfad für spätere Zugriffe ins Feld übernehmen
    QFile inputFile;
    inputFile.setFileName(fileName);
    inputFile.open(QIODevice::ReadOnly | QIODevice::Text);
    ui->pathToFile->setText(fileName);

    int lineNumber = 0;
    QString lineFromFile;
    QStringList itemList;

    int row = 0;
    int columnTotalNumber = 5;

    while(!inputFile.atEnd())
    {
        lineFromFile = inputFile.readLine();

        // Überprüfung ob Header in erster Zeile vorhanden
        if (lineNumber == 0)
        {
            if (lineFromFile.startsWith("# Datei erstellt")) // "für myToDoList.pro" geht nicht wegen Ü
            {
                lineNumber++;
                deleteList();
                continue;
            } else {
                QApplication::beep();
                QMessageBox::critical(this, fileName, "Datei konnte nicht geöffnet werden!");
                return;
            }

        // Zeilen die mit '#' beginnen oder leer sind können ignoriert werden
        } else if ((lineFromFile[0] == "#") || (lineFromFile == "\n")) {

            lineNumber++;
            continue;

        // ansonsten Zeile aufteilen und Teile als Zellen in einer Reihe der Tabelle einfügen
        } else {
            itemList = lineFromFile.split("\t");
            ui->tableTasks->insertRow(row);

            for (int column = 0; column < columnTotalNumber; ++column)
            {
                QTableWidgetItem *itemFromList = new QTableWidgetItem(itemList.at(column));
                ui->tableTasks->setItem(row, column, itemFromList);
            }

            row++;
            lineNumber++;
        }
    }

    QMessageBox::information(this, fileName, "Liste erfolgreich geladen!");
    inputFile.close();
}

void MainWindow::on_actionNewList_triggered()
{
    // gesamte Tabelle wird gelöscht und der Datei-Pfad auf Default-Wert gesetzt

    disableSort();
    deleteList();
    ui->pathToFile->setText(QDir::homePath()+"/neueToDoListe.txt");
}

void MainWindow::on_actionOpenList_triggered()
{
    // Dateipfad kann durch Navigation gewählt werden. Es werden nur Ordner und *.txt-Dateien angezeigt.
    // Datei wird verarbeitet

    disableSort();
    QString fileName = QFileDialog::getOpenFileName(this, "Öffne Liste", QDir::homePath(), "Text files (*.txt)");
    MainWindow::loadList(fileName);
}

void MainWindow::on_actionReloadList_triggered()
{
    // Dateipfad wird aus Feld übernommen und verarbeitet

    disableSort();
    QString fileName = ui->pathToFile->text();
    MainWindow::loadList(fileName);
}

void MainWindow::on_actionDeleteAllTasks_triggered()
{
    // gesamte Tabelle wird gelöscht, der Datei-Pfad bleibt im Feld

    disableSort();
    MainWindow::deleteList();
}

void MainWindow::on_buttonEditItem_clicked()
{
    // Bearbeiten von einzelnen Reihen

    disableSort();

    int selectedRow;
    QList<QTableWidgetItem*> selectedItemsList;
    selectedItemsList = ui->tableTasks->selectedItems();

    //Tabelle ist leer
    if (ui->tableTasks->rowCount() == 0)
    {
        QApplication::beep();
        QMessageBox::warning(this, "Bearbeiten von Aufgaben",
                             "Es wurde keine Aufgabe ausgewählt, da die Liste leer ist.\nBitte öffnen Sie eine Liste.");
        return;

    // kein Eintrag ist ausgewählt
    } else if (selectedItemsList.isEmpty()) {
        QApplication::beep();
        QMessageBox::warning(this, "Bearbeiten von Aufgaben",
                             "Es wurde keine Aufgabe ausgewählt.\nBitte wählen Sie eine Aufgabe zur Bearbeitung aus.");
        return;

    // definitiv mehr als eine Zeile mit 5 Zellen gewählt
    } else if (selectedItemsList.size() >= 6) {
        QApplication::beep();
        QMessageBox::warning(this, "Bearbeiten von Aufgaben",
                             "Es wurde mehr als eine Aufgabe ausgewählt.\nBitte wählen Sie zur Bearbeitung nur eine Aufgabe.");
        return;

    // bei Auswahl von <= 5 Zellen erfolgt Überprüfung, ob nur eine Zeile gewählt
    } else if (selectedItemsList.size() >= 2) {

        for (int i = 0; i < selectedItemsList.size() - 1; ++i)
        {
            int rowOfCurrentItem = ui->tableTasks->row(selectedItemsList.at(i));
            int rowOfNextItem = ui->tableTasks->row(selectedItemsList.at(i + 1));

            if (rowOfCurrentItem != rowOfNextItem)
            {
                QApplication::beep();
                QMessageBox::warning(this, "Bearbeiten von Aufgaben",
                                     "Es wurde mehr als eine Aufgabe ausgewählt.\nBitte wählen Sie zur Bearbeitung nur eine Aufgabe.");
                return;
            }
        }
    }

    // Überprüfungen erfolgreich

    selectedRow = ui->tableTasks->row(selectedItemsList.at(0));

    // Übernahme von Werten aus gewählter Zeile in Formular
    QString project = ui->tableTasks->item(selectedRow, PROJECT)->text();
    QDateTime dueDate;
    QString dueDateString = ui->tableTasks->item(selectedRow, DUEDATE)->text();
    QString priority = ui->tableTasks->item(selectedRow, PRIORITY)->text();
    QString task = ui->tableTasks->item(selectedRow, TASK)->text();
    QString taskInfo = ui->tableTasks->item(selectedRow, TASKINFO)->text();

    editItemDialog item(this);
    item.pasteFields(project, dueDateString, priority, task, taskInfo);
    item.setWindowTitle("Bearbeite Eintrag");
    int rejectOrAcceptButton = item.exec();

    // Übernahme von Werten aus Formular für Zeile
    project = item.project();
    dueDate = item.dueDate();
    dueDateString = dueDate.toString("yyyy.MM.dd");
    priority = item.priority();
    task = item.task();
    taskInfo = item.taskInfo();

    // Möglichkeit zum Abbruch
    if (rejectOrAcceptButton == QDialog::Rejected)
    {
        return;

    // Werte in Zellen übernehmen
    } else {
        ui->tableTasks->setItem(selectedRow, PROJECT, new QTableWidgetItem(project));
        ui->tableTasks->setItem(selectedRow, DUEDATE, new QTableWidgetItem(dueDateString));
        ui->tableTasks->setItem(selectedRow, PRIORITY, new QTableWidgetItem(priority));
        ui->tableTasks->setItem(selectedRow, TASK, new QTableWidgetItem(task));
        ui->tableTasks->setItem(selectedRow, TASKINFO, new QTableWidgetItem(taskInfo));
    }
}

void MainWindow::on_buttonDeleteItem_clicked()
{
    // löschen von gewählten Zeilen

    disableSort();

    QList<QTableWidgetItem*> selectedItemsList;
    selectedItemsList = ui->tableTasks->selectedItems();

    // keine Zeilen/Zellen ausgewählt
    if (selectedItemsList.isEmpty())
    {
        return;
    }

    for (int i = 0; i < selectedItemsList.size(); ++i)
    {
        ui->tableTasks->removeRow(ui->tableTasks->row(selectedItemsList.at(i)));
    }
}

void MainWindow::on_buttonEnableSort_stateChanged(int arg1)
{
    // Spaltenweises sortieren ermöglichen oder nicht

    // wenn Checkbox ausgewählt (Haken gesetzt)
    if(arg1)
    {
        ui->tableTasks->setSortingEnabled(true);

    // wenn nicht ausgewählt (ohne Haken)
    } else {
        ui->tableTasks->setSortingEnabled(false);
    }
}

void MainWindow::on_buttonInsertItem_clicked()
{
    // Hinzufügen eines neuen Eintrags

    disableSort();

    int row;
    QString project;
    QDateTime dueDate;
    QString dueDateString;
    QString priority;
    QString task;
    QString taskInfo;

    newItemDialog item(this);
    item.setWindowTitle("Neue Aufgabe");
    int rejectOrAcceptButton = item.exec();

    // Übernahme von Werten aus Formular für Zeile
    project = item.project();
    dueDate = item.dueDate();
    dueDateString = dueDate.toString("yyyy.MM.dd");
    priority = item.priority();
    task = item.task();
    taskInfo = item.taskInfo();

    // Möglichkeit zum Abbruch
    if (rejectOrAcceptButton == QDialog::Rejected)
    {
        return;

    // Werte in Zellen übernehmen
    } else {
        ui->tableTasks->insertRow(ui->tableTasks->rowCount());
        row = ui->tableTasks->rowCount() - 1;
        ui->tableTasks->setItem(row, PROJECT, new QTableWidgetItem(project));
        ui->tableTasks->setItem(row, DUEDATE, new QTableWidgetItem(dueDateString));
        ui->tableTasks->setItem(row, PRIORITY, new QTableWidgetItem(priority));
        ui->tableTasks->setItem(row, TASK, new QTableWidgetItem(task));
        ui->tableTasks->setItem(row, TASKINFO, new QTableWidgetItem(taskInfo));
    }
}

void MainWindow::on_buttonFind_clicked()
{
    // Suchen von String der in Feld angegeben wird (Groß-/Kleinschreibung wird ignoriert)
    // Start der Suche nach aktueller Zellenauswahl

    QString searchString = ui->searchText->text().toLower();

    int rowTotalNumber = ui->tableTasks->rowCount();
    int columnTotalNumber = ui->tableTasks->columnCount();

    int currentRow = ui->tableTasks->currentRow();
    int currentColumn = ui->tableTasks->currentColumn()+1;

    // falls nichts ausgewählt ist, Suche in erster Reihe beginnen
    if (currentRow < 0)
        currentRow = 0;

    for (int i = currentRow; i < rowTotalNumber; i++)
    {
        for (int j = currentColumn; j < columnTotalNumber; j++)
        {
            // String in Zelle gefunden
            if (ui->tableTasks->item(i,j)->text().toLower().contains(searchString))
            {
                ui->tableTasks->clearSelection();
                ui->tableTasks->setCurrentCell(i, j);
                activateWindow();
                return;
            }

            ++currentColumn;
        }

        currentColumn = 0;
        ++currentRow;
    }

    // Tabellenende erreicht, Beginn von vorne ermöglichen
    if (currentRow == rowTotalNumber)
    {
        QApplication::beep();
        QMessageBox::information(this, "Suche", "Ende der Tabelle erreicht.");
        ui->tableTasks->setCurrentCell(0, 0);
        ui->tableTasks->clearSelection();
    }
}

void MainWindow::on_buttonSaveList_clicked()
{
    // Speichern der Liste, Dateipfad wird aus Feld übernommen

    disableSort();

    QString fileName = ui->pathToFile->text();
    QFile outputFile(fileName);

    // *.txt-Format ist obligat, daher überprüfung
    if (!fileName.endsWith(".txt"))
    {
        QApplication::beep();
        QMessageBox::critical(this, fileName,
                              "Datei konnte nicht gespeichert werden!\nDateiname muss auf *.txt enden.");
        return;
    }

    // falls die Datei schon existiert, gewünschte Überschreibung abfragen
    if (outputFile.exists(fileName))
    {
        QApplication::beep();
        QMessageBox::StandardButton overwriteYesOrNo;
        overwriteYesOrNo = QMessageBox::question(this, "Datei existiert bereits",
                                                 "Möchten Sie die vorhandene Datei ersetzen?", QMessageBox::Yes|QMessageBox::No);
        if (overwriteYesOrNo == QMessageBox::No)
            return;
    }

    // Datei öffnen
    outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream writeToFile(&outputFile);
    int columnTotalNumber = ui->tableTasks->columnCount();
    int rowTotalNumber = ui->tableTasks->rowCount();

    // Inhalt in Datei schreiben
    writeToFile << QString::fromUtf8("# Datei erstellt für myToDoList.pro\t");
    writeToFile << QDate::currentDate().toString("yyyy.MM.dd") << "\n\n";
    writeToFile << QString::fromUtf8("# Projekt\tFälligkeit\t\tAufgabe\tSiehe\n");

    for (int i = 0; i < rowTotalNumber; i++)
    {
        for (int j = 0; j < columnTotalNumber; j++)
        {
            QString textInCell = ui->tableTasks->item(i,j)->text();
            writeToFile << textInCell << "\t";
        }
        writeToFile << "\n";
    }

    outputFile.flush();
    outputFile.close();

    QMessageBox::information(this, fileName, "Liste erfolgreich gepeichert!");
}


