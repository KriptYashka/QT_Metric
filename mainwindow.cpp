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
}

MainWindow::~MainWindow(){
    delete ui;
}
/* Глобальные переменные */
QStandardItemModel *general_model = new QStandardItemModel;
QVector<QString> col_names;

int read_csv_file(QString path, QStandardItemModel* model){
    /* Считывает данные из файла и записывает в модель
    Возвращает код ошибки*/
    std::ifstream file_csv(path.toStdString());
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return 0;
    }

    model->clear();
    model->setColumnCount(7);
    model->setHorizontalHeaderLabels(QStringList() << "Year" << "Region" << "Natural growth" << "Birth rate" << "Death rate"
                                        << "General dem. weight" << "Urbanization");

    QTextStream in(&file);
    if (!in.atEnd()){
        QString line = in.readLine();
    }
    while (!in.atEnd()){
        // Считывание из файла и перенос в таблицу
        QString line = in.readLine();
        QList<QStandardItem *> standardItemsList;
        for (QString item : line.split(",")) {
            standardItemsList.append(new QStandardItem(item));
        }
        model->insertRow(model->rowCount(), standardItemsList);
    }
    file.close();
    return 1;
}

void MainWindow::on_btn_loadfile_clicked(){
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"));
    int cod = read_csv_file(filePath, csvModel);
    general_model = new QStandardItemModel(csvModel);
    if (cod == 0){
        ui->label_result->setText("Не возможно открыть файл");
        ui->line_region->setText("");
        ui->line_col->setText("");
    }
}


void MainWindow::on_btn_load_clicked(){
    ui->table_metric->setModel(csvModel);
}

int check_column(QString col){
    bool flag = true;
    int res = col.toInt(&flag);
    if (flag)
        return res;
    return -1;
}

QList<QStandardItem *> get_row(QStandardItemModel* model, int row){
    QList<QStandardItem *> res;
    for (int i = 0; i < model->columnCount(); ++i){
        res.append(model->item(row, i));
    }
    return res;
}

void MainWindow::on_btn_metric_clicked()
{
    QString region = ui->line_region->text();
    QString column = ui->line_col->text();
    int col = check_column(column);
    if (col == -1 || col > csvModel->rowCount()){
        ui->label_result->setText("Переданы некорректные значения");
        return;
    }

    general_model->clear();
    general_model->setColumnCount(7);
    general_model->setHorizontalHeaderLabels(QStringList() << "Year" << "Region" << "Natural growth" << "Birth rate" << "Death rate"
                                        << "General dem. weight" << "Urbanization");
    double minimum = 0;
    double maximum = 0;
    double average = 0;
    // Выборка нужных записей
    for (int row = 0; row < csvModel->rowCount(); ++row){
        if (csvModel->item(row, 1)->text() == region || region == ""){
            QList<QStandardItem *> res;
            for (int i = 0; i < csvModel->columnCount(); ++i){
                res.append(csvModel->item(row, i));
            }
            general_model->insertRow(general_model->rowCount(), res);
        }
    }
    ui->table_metric->setModel(general_model);
}
