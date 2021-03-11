#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(7);
    csvModel->setHorizontalHeaderLabels(QStringList() << "Year" << "Region" << "Natural growth" << "Birth rate" << "Death rate"
                                        << "General dem. weight" << "Urbanization");
    ui->table_metric->setModel(csvModel);
}

MainWindow::~MainWindow(){
    delete ui;
}
/* Глобальные переменные */
QVector<QVector<QString>> data;
QVector<QString> col_names;

int MainWindow::read_csv_file(QString path){
    /* Считывает данные из файла и записывает в переменную data */
    std::ifstream file_csv(path.toStdString());
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        ui->label_result->setText("Не возможно открыть файл");
        return(0);
    }

    QTextStream in(&file);
    //Reads the data up to the end of file
    while (!in.atEnd()){
        QString line = in.readLine();
        // Adding to the model in line with the elements
        QList<QStandardItem *> standardItemsList;
        // consider that the line separated by semicolons into columns
        for (QString item : line.split(";")) {
            standardItemsList.append(new QStandardItem(item));
        }
        csvModel->insertRow(csvModel->rowCount(), standardItemsList);
    }
    file.close();
    return(1);
}

void MainWindow::on_btn_loadfile_clicked(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"));
    if (read_csv_file(filePath);

    ui->label_region->setText("");
    ui->label_col->setText("");

    ui->table_metric->clearSpans();
}


void MainWindow::on_btn_load_clicked(){
    QFile file(":/exampleTable.csv");
        if (!file.open(QFile::ReadOnly | QFile::Text) ) {
            qDebug() << "File not exists";
        } else {
            QTextStream in(&file);
            // Чтение до конца файла
            while (!in.atEnd())
            {
                QString line = in.readLine();
                // Adding to the model in line with the elements
                QList<QStandardItem *> standardItemsList;
                // consider that the line separated by semicolons into columns
                for (QString item : line.split(";")) {
                    standardItemsList.append(new QStandardItem(item));
                }
                csvModel->insertRow(csvModel->rowCount(), standardItemsList);
            }
            file.close();
        }
}
